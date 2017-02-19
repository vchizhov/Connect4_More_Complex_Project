#pragma once
#include <vector>
#include "c4_graphics.h"
#include "c4_input.h"
#include "c4_core.h"
namespace C4
{
	template<template<int, int, int, int> class Player1, template<int, int, int, int> class Player2, int board_width, int board_height, int number_of_stones, int depth>
	class game
	{
	private:
		graphics<board_width, board_height>* graphics_object;
		input<board_width, board_height>* input_object;

		//keeps track of the moves since the beginning of the game
		std::vector<player_move_outcome> moves_history;
		//keeps track of the undo history
		std::vector<player_move_outcome> undo_history;

		core<board_width, board_height, number_of_stones, depth>* player1;
		core<board_width, board_height, number_of_stones, depth>* player2;

		core<board_width, board_height, number_of_stones, depth>* current_player_pointer;

		PLAYER current_player;

		//the ctor takes care of initiazlizing it to the right values
		player_move_outcome current_player_move;	


		core<board_width, board_height, number_of_stones, depth>* get_next_player()
		{
			switch (current_player)
			{
			case PLAYER_1:
				return player2;
				break;
			case PLAYER_2:
				return player1;
				break;
			}
		}

		void next_player()
		{
			current_player = static_cast<PLAYER>(-current_player);
			switch (current_player)
			{
			case PLAYER_1:
				current_player_pointer = player1;
				break;
			case PLAYER_2:
				current_player_pointer = player2;
				break;
			}
		}

		void process_command()
		{
			switch (input_object->get_last_command())
			{
			case INPUT_COMMAND_NONE:

				break;
				//if a human player issues this command we ignore it
			case INPUT_COMMAND_PROCEED:
				if (!current_player_pointer->is_human())
				{
					if (moves_history.empty())
					{
						//make the current player calculate the next move
						current_player_move = current_player_pointer->make_first_move();
					}
					else
					{
						//make the current player calculate the next move
						current_player_move = current_player_pointer->make_move();
					}
					//clear the undo hisotry and add it to the moves history
					undo_history.clear();
					moves_history.push_back(current_player_move);

					//update the graphics with the new move
					graphics_object->make_move(current_player_move);
					//switch the player

					next_player();
					//inform the other player about this move
					current_player_pointer->add_move(current_player_move);
				}
				else
				{
					//ignore the command for human players
				}
				
				break;
			case INPUT_COMMAND_EXIT:
				
				break;
				//use input to decide the next move - human players always go here
				//can be used for AI players to force a move upon them (works only for non automatic AI players)
			case INPUT_COMMAND_MOVE:
				//feed it the move that was recorded with INPUT_COMMAND_MOVE
				if(input_object->is_last_move_valid())
				{
					if (moves_history.empty())
					{
						current_player_move = current_player_pointer->make_first_move(input_object->get_last_move());
					}
					else
					{
						current_player_move = current_player_pointer->make_move(input_object->get_last_move());
					}
					
					//clear the undo history and add it to the moves history
					undo_history.clear();
					moves_history.push_back(current_player_move);

					//update the graphics with the new move
					graphics_object->make_move(current_player_move);

					//switch the player
					next_player();
					//inform the other player about this move
					current_player_pointer->add_move(current_player_move);
				}
				else
				{
					//if it was not a valid move ignore the command
				}
				
				break;
			case INPUT_COMMAND_UNDO:
				//check if undoable
				if (!moves_history.empty())
				{
					if (current_player_pointer->is_automatic() && get_next_player()->is_automatic())
					{
						//ignore the command if both players are automatic
					}//if the next player is automaitc we need to do a double undo
					else if (get_next_player()->is_automatic())
					{
						//we do not need to check if there are 2 moves, as if the history is non emtpy
						//and the other player is automatic there are bound to be at least 2 moves if the
						//moves history is non empty
						/*if (moves_history.size() == 2)
						{*/

						//erase the 2 moves from both players boards:
						player1->erase_move(moves_history.back());
						player2->erase_move(moves_history.back());
						graphics_object->erase_move(moves_history.back());
						//push the move into the undo queue
						undo_history.push_back(moves_history.back());
						//remove the move from the history
						moves_history.pop_back();

						player1->erase_move(moves_history.back());
						player2->erase_move(moves_history.back());
						graphics_object->erase_move(moves_history.back());
						//push the move into the undo queue
						undo_history.push_back(moves_history.back());
						//remove the move from the history
						moves_history.pop_back();
					}//if both players are non automatic
					else
					{
						//erase the the move from both players boards:
						player1->erase_move(moves_history.back());
						player2->erase_move(moves_history.back());
						graphics_object->erase_move(moves_history.back());
						//push the move into the undo queue
						undo_history.push_back(moves_history.back());
						//remove the move from the history
						moves_history.pop_back();
						//set the outcome to unfinished just in case
						current_player_move.outcome = OUTCOME_UNFINISHED;

						//switch the player
						next_player();
					}
				}
				else
				{
					//if the history is empty we cannot undo
				}
				
				break;
			case INPUT_COMMAND_REDO:
				//if the undo history is emtpy there's no move to redo
				if (!undo_history.empty())
				{
					//we do not need to check whether both players are automatic, as if they are
					//the undo queue would be empty because the undo command is ignored if there are 2 auto players
					//if the other is automatic we need to redo 2 moves
					if (get_next_player()->is_automatic())
					{
						//make the 2 moves for both players
						player1->add_move(undo_history.back());
						player2->add_move(undo_history.back());
						graphics_object->make_move(undo_history.back());
						//push the move into the history
						moves_history.push_back(undo_history.back());
						//remove the move from the history
						undo_history.pop_back();

						player1->add_move(undo_history.back());
						player2->add_move(undo_history.back());
						graphics_object->make_move(undo_history.back());
						//push the move into the history
						moves_history.push_back(undo_history.back());
						//remove the move from the history
						undo_history.pop_back();

					}//if both are non-automatic
					else
					{

						//make the move for both players
						player1->add_move(undo_history.back());
						player2->add_move(undo_history.back());
						graphics_object->make_move(undo_history.back());
						//set teh outcome to the move outcome just in case it's a win/draw
						current_player_move = undo_history.back();
						//push the move into the history
						moves_history.push_back(undo_history.back());
						
						//remove the move from the history
						undo_history.pop_back();
						next_player();
					}
				}
				else
				{
					//ignore command if cannot redo
				}
				
				break;
			}
		}


	public:

		game() = delete;

		game(graphics<board_width, board_height>* graphics_object, input<board_width, board_height>* input_object) : graphics_object(graphics_object), input_object(input_object), current_player(PLAYER_1)
		{
			player1 = new Player1<board_width, board_height, number_of_stones, depth>(PLAYER_1);
			player2 = new Player2<board_width, board_height, number_of_stones, depth>(PLAYER_2);
			current_player_pointer = player1;
		}

		~game()
		{
			delete player1;
			delete player2;
		}

		void Run()
		{

			//MAIN LOOP
			do
			{
				//GAME IN PROGRESS LOOP
				//while the user hasn't issued an exit command, or the game has not reached a conclusion
				while (input_object->get_last_command() != INPUT_COMMAND_EXIT && current_player_move.outcome == OUTCOME_UNFINISHED)
				{
					//render stuff
					graphics_object->render();

					//if the current AI player is initialized not to care about input just skip the poll
					//a human player cannot skip the poll
					if (!current_player_pointer->is_automatic())
						input_object->poll();

					//process the command from the previous poll(if there was no poll the command is INPUT_COMMAND_PROCEED
					process_command();
				}

				//GAME REACHED A CONCLUSION LOOP
				//we can issue an undo command from here if we want to
				while (input_object->get_last_command() != INPUT_COMMAND_EXIT && input_object->get_last_command() != INPUT_COMMAND_UNDO)
				{
					graphics_object->render_outcome();
					//returns only COMMAND_EXIT, COMMAND_UNDO
					input_object->poll_end();
					process_command();
				}

			} while (input_object->get_last_command() != INPUT_COMMAND_EXIT);
		}
	};
}