#pragma once
#include "c4_structs.h"
#include "c4_graphics.h"
namespace C4
{
	template<int board_width, int board_height>
	class input
	{
	protected:

		INPUT_COMMAND last_command;
		player_move_outcome last_move;
		bool last_move_valid;
	public:
		input() : last_command(INPUT_COMMAND_NONE), last_move_valid(false) {}
		//this should change whether the move is valid in an inherited child
		virtual void poll() = 0;
		virtual void poll_end() = 0;
		INPUT_COMMAND get_last_command() const
		{
			return last_command;
		}

		bool is_last_move_valid()
		{
			return last_move_valid;
		}

		player_move_outcome get_last_move() const
		{
			return last_move;
		}
	};

	template<int board_width, int board_height>
	class tigr_input : public input<board_width, board_height>
	{
	private:
		tigr_graphics<board_width, board_height>* graphics_object;

		int key_escape;
		int key_proceed;
		int key_move;
		int key_undo;
		int key_redo;
	public:
		tigr_input(tigr_graphics<board_width, board_height>* graphics_object)
			:graphics_object(graphics_object)
		{
			key_escape = static_cast<int>(TK_ESCAPE);
			key_proceed = static_cast<int>(TK_RETURN);
			key_move = static_cast<int>(TK_SPACE);
			key_undo = static_cast<int>('U');
			key_redo = static_cast<int>('R');
		}

		void poll()
		{
			last_command = INPUT_COMMAND_NONE;

			if (tigrClosed(graphics_object->get_window()) || tigrKeyDown(graphics_object->get_window(), key_escape))
			{
				last_command = INPUT_COMMAND_EXIT;
				return;
			}

			if (tigrKeyDown(graphics_object->get_window(), key_proceed))
			{
				last_command = INPUT_COMMAND_PROCEED;
				return;
			}

			if (tigrKeyDown(graphics_object->get_window(), key_move))
			{
				last_move_valid = false;
				last_command = INPUT_COMMAND_MOVE;
				graphics_object->get_xy_from_mouse(last_move.x, last_move.y);

				if (-1 < last_move.x && last_move.x < board_width && -1 < last_move.y &&
					last_move.y < board_height && graphics_object->column_top(last_move.x)<board_height)
				{
					last_move.y = graphics_object->column_top(last_move.x);
					last_move_valid = true;
				}
				return;
			}

			if (tigrKeyDown(graphics_object->get_window(), key_undo))
			{
				last_command = INPUT_COMMAND_UNDO;
				return;
			}
			if (tigrKeyDown(graphics_object->get_window(), key_redo))
			{
				last_command = INPUT_COMMAND_REDO;
				return;
			}


		}

		void poll_end()
		{
			last_command = INPUT_COMMAND_NONE;
			if (tigrClosed(graphics_object->get_window()) || tigrKeyDown(graphics_object->get_window(), key_escape))
			{
				last_command = INPUT_COMMAND_EXIT;
				return;
			}

			if (tigrKeyDown(graphics_object->get_window(), key_undo))
			{
				last_command = INPUT_COMMAND_UNDO;
				return;
			}
		}


	};
}