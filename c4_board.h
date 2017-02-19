#pragma once
#include <bitset>
#include "c4_structs.h"

/*
A board should include the following methods:
1) BOARD_VALUE get(int x, int y) const
2) void set(int x, int y, BOARD_VALUE value)
3) BOARD_VALUE get_column_top(int x) const
*/

namespace C4
{

	//the memory of both boards are organized by columns to enable
	//fast execution of get_column_top - reading sequential memory is faster

	//faster but takes more memory
	template<int board_width, int board_height>
	class standard_board
	{
	private:
		BOARD_VALUE arr[board_width][board_height];
	public:
		standard_board()
		{
			memset(arr, BOARD_VALUE_EMPTY, sizeof(BOARD_VALUE)*board_width*board_height);
		}

		standard_board(const standard_board& other)
		{
			memcpy(arr, other.arr, sizeof(BOARD_VALUE)*board_width*board_height);
		}

		BOARD_VALUE get(int x, int y) const
		{
			return arr[x][y];
		}

		void set(int x, int y, BOARD_VALUE value)
		{
			arr[x][y] = value;
		}

		void set(move mv, BOARD_VALUE value)
		{
			arr[mv.x][mv.y] = value;
		}

		int get_column_top(int x) const
		{
			for (int y = 0; y < board_height; ++y)
			{
				if (get(x, y) == BOARD_VALUE_EMPTY)
					return y;
			}
			return board_height;
		}
	};

	//slower but takes less memory
	template<int board_width, int board_height>
	class bitset_board
	{
	private:
		std::bitset<2 * board_height*board_width> arr;
	public:
		//ATTENTION! returns BOARD_VALUE_UNDEFINED as BOARD_VALUE_PLAYER_1
		BOARD_VALUE get(int x, int y) const
		{
			switch (arr.test(2 * (x*board_height + y)))
			{
			case false:
				switch (arr.test(2 * (x*board_height + y) + 1))
				{
				case false:
					return BOARD_VALUE_EMPTY;
				case true:
					return BOARD_VALUE_PLAYER_2;
				}
			case true:
				return BOARD_VALUE_PLAYER_1;
			}
		}

		void set(int x, int y, BOARD_VALUE value)
		{
			switch (value)
			{
			case BOARD_VALUE_EMPTY:
				arr.set(2 * (x*board_height + y), false);
				arr.set(2 * (x*board_height + y) + 1, false);
				break;
			case BOARD_VALUE_PLAYER_1:
				arr.set(2 * (x*board_height + y), true);
				arr.set(2 * (x*board_height + y) + 1, false);
				break;
			case BOARD_VALUE_PLAYER_2:
				arr.set(2 * (x*board_height + y), false);
				arr.set(2 * (x*board_height + y) + 1, true);
				break;
			case BOARD_VALUE_UNDEFINED:
				arr.set(2 * (x*board_height + y), true);
				arr.set(2 * (x*board_height + y) + 1, true);
				break;
			}
		}

		int get_column_top(int x) const
		{
			for (int y = 0; y < board_height; ++y)
			{
				if (get(x, y) == BOARD_VALUE_EMPTY)
					return y;
			}
			return board_height;
		}
	};

}