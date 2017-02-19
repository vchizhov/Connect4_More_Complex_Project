#pragma once
#include "c4_board.h"
namespace C4
{
	//making the first move shouldn't require this class(it won't work correctly)
	template<int board_width, int board_height, int depth>
	class mutable_game_state
	{
	private:
		standard_board<board_width, board_height> board;

		BOARD_VALUE current_player;
		move moves_stack[depth];
		int moves_stack_counter;
		

		//would if or switch here be faster?
		



	public:

		int points;

		void next_player()
		{
			current_player = static_cast<BOARD_VALUE>(-current_player);
		}

		mutable_game_state(PLAYER player)
			: current_player(static_cast<BOARD_VALUE>(player)), moves_stack_counter(depth), points(0)
		{

		}

		mutable_game_state() = delete;

		void add_move(player_move_outcome mv)
		{
			board.set(mv.x, mv.y, static_cast<BOARD_VALUE>(mv.player));
		}

		void erase_move(player_move_outcome mv)
		{
			board.set(mv.x, mv.y, BOARD_VALUE_EMPTY);
		}

		BOARD_VALUE get_current_player() const
		{
			return current_player;
		}

		move get_last_move() const
		{
			return moves_stack[moves_stack_counter];
		}

		int get_last_move_x() const
		{
			return moves_stack[moves_stack_counter].x;
		}

		int get_last_move_y() const
		{
			return moves_stack[moves_stack_counter].y;
		}

		BOARD_VALUE get(int x, int y) const
		{
			return board.get(x, y);
		}

		void make_move(int x, int y)
		{
			next_player();
			moves_stack[--moves_stack_counter].set(x, y);
			board.set(x, y, current_player);
			
		}

		void stack_move(int x, int y)
		{
			moves_stack[--moves_stack_counter].set(x, y);
		}

		void unstack_move()
		{
			++moves_stack_counter;
		}

		void undo_move()
		{
			next_player();
			board.set(moves_stack[moves_stack_counter++], BOARD_VALUE_EMPTY);	
		}

		int get_current_depth() const
		{
			return moves_stack_counter;
		}

		int get_column_top(int x)
		{
			return board.get_column_top(x);
		}

		void set_depth(int d)
		{
			moves_stack_counter = d;
		}

		void reset_depth()
		{
			moves_stack_counter = depth;
		}
	};
}