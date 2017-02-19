#pragma once
#include <vector>
#include "c4_structs.h"
#include "c4_game_state.h"
#include "c4_heuristic.h"
#include "c4_comparator.h"
//UNFINISHED

namespace C4
{



	template<int board_width,int board_height,int number_of_stones,int depth>
	class core
	{
	protected:
		PLAYER player;
	public:
		core(PLAYER player) : player(player) {}

		virtual void add_move(player_move_outcome mv) = 0;

		virtual void erase_move(player_move_outcome mv) = 0;

		virtual player_move_outcome make_first_move(player_move_outcome mv = player_move_outcome(-1,-1, PLAYER_1, OUTCOME_UNFINISHED)) = 0;

		virtual player_move_outcome make_move(player_move_outcome mv = player_move_outcome(-1, -1, PLAYER_1, OUTCOME_UNFINISHED)) = 0;

		virtual bool is_automatic() const = 0;
		virtual bool is_human() const = 0;
	};

	template<int board_width, int board_height, int number_of_stones, int depth>
	class human_core : public core<board_width, board_height, number_of_stones, depth>
	{
	private:
		mutable_game_state<board_width, board_height, depth> game_state;

		bool check_win() const
		{
			if (basic_heuristic<mutable_game_state, board_width, board_height, depth, number_of_stones>::evaluate_state(game_state) != 0)
				return true;
			else
				return false;
		}

		bool check_draw() const
		{
			for (int x = 0; x < board_width; ++x)
			{
				if (game_state.get(x, board_height - 1) == BOARD_VALUE_EMPTY)
					return false;
			}
			return true;
		}

		OUTCOME evaluate_outcome() const
		{
			if (check_win())
				return static_cast<OUTCOME>(player);
			else
			{
				if (check_draw())
				{
					return OUTCOME_DRAW;
				}
				else
				{
					//it may actually be the other player's win, but we don't need to check for that
					return OUTCOME_UNFINISHED;
				}
			}
		}

	public:
		human_core(PLAYER player) : core(player), game_state(player) {}

		void add_move(player_move_outcome mv)
		{
			game_state.add_move(mv);
		}

		void erase_move(player_move_outcome mv)
		{
			game_state.erase_move(mv);
		}

		player_move_outcome make_move(player_move_outcome mv = player_move_outcome(-1, -1, PLAYER_1, OUTCOME_UNFINISHED))
		{
			//this way the move gets stacked, and can be accessed though get last move
			game_state.add_move(player_move_outcome(mv.x, mv.y, player, OUTCOME_UNFINISHED));
			game_state.stack_move(mv.x, mv.y);
			//after we have a last move available we can evaluate it
			OUTCOME result = evaluate_outcome();
			// we need to unstack the move for the game_state to function properly
			game_state.unstack_move();
			//and just write it to the board

			return player_move_outcome(mv.x, mv.y, player, result);
		}

		player_move_outcome make_first_move(player_move_outcome mv = player_move_outcome(-1, -1, PLAYER_1, OUTCOME_UNFINISHED))
		{
			return make_move(mv);
		}

		bool is_automatic() const
		{
			return false;
		}

		bool is_human() const
		{
			return true;
		}
	};

	//add the negamax
	template<int board_width, int board_height, int number_of_stones, int depth>
	class fast_AI_core : public core<board_width, board_height, number_of_stones, depth>
	{
	private:
		mutable_game_state<board_width, board_height, depth> game_state;

		void generate_moves(std::vector<player_move_outcome>& output)
		{
			int y;
			player_move_outcome temp;
			temp.player = player;
			for (int x = 0; x < board_width; ++x)
			{
				if ((y = game_state.get_column_top(x)) < board_height)
				{
					temp.x = x;
					temp.y = y;
					output.push_back(temp);
				}
			}
		}


		//can't use std::sort because compare is non static

		void order_moves(std::vector<player_move_outcome>& output)
		{
			std::sort(output.begin(), output.end(), count_surrounding_comparator<mutable_game_state, board_width, board_height, depth>(game_state));
		}

		int negamax(int alpha, int beta)
		{
			//depth end reached? or we actually hit a win/lose condition?
			if (game_state.get_current_depth() == 0 || game_state.points != 0)
			{

				return -static_cast<int>(game_state.get_current_player())*game_state.points*(game_state.get_current_depth()+1);
			}

			
			bool empty = true;
			int bestValue = -basic_heuristic<mutable_game_state, board_width, board_height, depth, number_of_stones>::extreme_value*(depth+1);
			int v;
			for (int x = 0;x<board_width;++x)
			{
				if ((v = game_state.get_column_top(x)) < board_height)
				{
					empty = false;
					game_state.make_move(x, v);
					game_state.points = basic_heuristic<mutable_game_state, board_width, board_height, depth, number_of_stones>::evaluate_state(game_state);
					v = -negamax(-beta, -alpha);
					game_state.undo_move();
					bestValue = std::max(bestValue, v);
					alpha = std::max(alpha, v);
					if (alpha >= beta)
						break;
				}
			}
			//no possible moves - then it's a terminal state
			if (empty)
			{
				return -static_cast<int>(game_state.get_current_player())*game_state.points*(game_state.get_current_depth() + 1);
			}
			return bestValue;
		}

		bool check_win() const
		{
			if (basic_heuristic<mutable_game_state, board_width, board_height, depth, number_of_stones>::evaluate_state(game_state) != 0)
				return true;
			else
				return false;
		}

		bool check_draw() const
		{
			for (int x = 0; x < board_width; ++x)
			{
				if (game_state.get(x, board_height - 1) == BOARD_VALUE_EMPTY)
					return false;
			}
			return true;
		}

		OUTCOME evaluate_outcome() const
		{
			if (check_win())
				return static_cast<OUTCOME>(player);
			else
			{
				if (check_draw())
				{
					return OUTCOME_DRAW;
				}
				else
				{
					//it may actually be the other player's win, but we don't need to check for that
					return OUTCOME_UNFINISHED;
				}
			}
		}

		player_move_outcome find_best_move()
		{
			std::vector<player_move_outcome> moves;
			generate_moves(moves);
			order_moves(moves);
			game_state.next_player();
			player_move_outcome result;
			int extreme_points = basic_heuristic<mutable_game_state, board_width, board_height, depth, number_of_stones>::extreme_value*(depth+1);
			int bestValue = -extreme_points;
			int v;
			int dp = depth;
			//
			do
			{
				
				game_state.set_depth(dp);
				for (player_move_outcome& current_move : moves)
				{
					game_state.make_move(current_move.x, current_move.y);
					game_state.points = basic_heuristic<mutable_game_state, board_width, board_height, depth, number_of_stones>::evaluate_state(game_state);
					v = -negamax(-extreme_points, extreme_points);
					game_state.undo_move();
					if (bestValue < v)
					{
						bestValue = v;
						result.x = current_move.x;
						result.y = current_move.y;
					}
				}
				--dp;
			} while (bestValue < 0 && dp > 1);
			game_state.reset_depth();

			game_state.next_player();
			result.player = player;
			game_state.add_move(result);
			game_state.stack_move(result.x, result.y);
			//after we have a last move available we can evaluate it
			result.outcome = evaluate_outcome();
			// we need to unstack the move for the game_state to function properly
			game_state.unstack_move();
			return result;
		}

	public:
		fast_AI_core(PLAYER player) : core(player), game_state(player) {}

		void add_move(player_move_outcome mv)
		{
			game_state.add_move(mv);
		}

		void erase_move(player_move_outcome mv)
		{
			game_state.erase_move(mv);
		}

		player_move_outcome make_move(player_move_outcome mv = player_move_outcome(-1, -1, PLAYER_1, OUTCOME_UNFINISHED))
		{
			if (mv.x != -1)
			{
				//this way the move gets stacked, and can be accessed though get last move
				game_state.add_move(player_move_outcome(mv.x, mv.y, player, OUTCOME_UNFINISHED));
				game_state.stack_move(mv.x, mv.y);
				//after we have a last move available we can evaluate it
				OUTCOME result = evaluate_outcome();
				// we need to unstack the move for the game_state to function properly
				game_state.unstack_move();
				//and just write it to the board

				return player_move_outcome(mv.x, mv.y, player, result);
			}
			else
			{
				return find_best_move();
			}
		}

		player_move_outcome make_first_move(player_move_outcome mv = player_move_outcome(-1, -1, PLAYER_1, OUTCOME_UNFINISHED))
		{
			if (mv.x != -1)
			{
				return make_move(mv);
			}
			else
			{
				//default first move based on bs
				//example
				mv.x = board_width/2;
				mv.y = 0;
				return make_move(mv);
			}
		}

		

		bool is_automatic() const
		{
			return false;
		}

		bool is_human() const
		{
			return false;
		}
	};
}