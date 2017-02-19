#pragma once

namespace C4
{

	enum BOARD_VALUE
	{
		BOARD_VALUE_PLAYER_2 = -1,
		BOARD_VALUE_EMPTY = 0,
		BOARD_VALUE_PLAYER_1 = 1,
		BOARD_VALUE_UNDEFINED = 3
	};

	struct move
	{
		int x;
		int y;
		move() : x(-1), y(-1) {}
		move(int x, int y) : x(x), y(y) {}
		void set(int x, int y)
		{
			this->x = x;
			this->y = y;
		}
	};

	enum PLAYER
	{
		PLAYER_2 = -1,
		PLAYER_1 = 1

	};

	struct player_move
	{
		int x;
		int y;
		PLAYER player;
		player_move() : x(-1), y(-1), player(PLAYER_1) {}
		player_move(int x, int y, PLAYER player) : x(x), y(y), player(player) {}
	};

	enum OUTCOME
	{
		OUTCOME_PLAYER_2_WIN = -1,
		OUTCOME_DRAW = 0,
		OUTCOME_PLAYER_1_WIN = 1,
		OUTCOME_UNFINISHED = 2
	};

	struct player_move_outcome
	{
		int x;
		int y;
		PLAYER player;
		OUTCOME outcome;
		player_move_outcome() : x(-1), y(-1), player(PLAYER_1), outcome(OUTCOME_UNFINISHED) {}
		player_move_outcome(int x, int y, PLAYER player, OUTCOME outcome) : x(x), y(y), player(player), outcome(outcome) {}
	};

	enum INPUT_COMMAND
	{
		INPUT_COMMAND_NONE = -1,
		INPUT_COMMAND_PROCEED = 0,
		INPUT_COMMAND_EXIT = 1,
		INPUT_COMMAND_MOVE = 2,
		INPUT_COMMAND_UNDO = 3,
		INPUT_COMMAND_REDO = 4
	};
}