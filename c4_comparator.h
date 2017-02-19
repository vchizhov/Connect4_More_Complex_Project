#pragma once
#include "c4_structs.h"

namespace C4
{

	template<template<int,int,int> class game_state_class, int board_width ,int board_height, int depth>
	struct count_surrounding_comparator
	{
	private:
		const game_state_class<board_width, board_height, depth>& game_state;

		int count_surrounding_non_empty_cells(int x, int y) const
		{
			int arr_x[] = { -1,-1,-1, 0,1,1,1,0 };
			int arr_y[] = { -1, 0, 1, 1, 1, 0, -1, -1 };
			int acc = 0;
			for (int i = 0; i < 8; ++i)
			{
				if (-1 < x + arr_x[i] && x + arr_x[i] < board_width &&
					-1 < y + arr_y[i] && y + arr_y[i] < board_height &&
					game_state.get(x + arr_x[i], y + arr_y[i]) != BOARD_VALUE_EMPTY)
				{
					++acc;
				}
			}
			return acc;
		}

	public:
		count_surrounding_comparator(const game_state_class<board_width, board_height, depth>& game_state)
			:game_state(game_state)
		{

		}

		bool operator()(const player_move_outcome& a, const player_move_outcome& b) const
		{
			return count_surrounding_non_empty_cells(a.x, a.y) > count_surrounding_non_empty_cells(b.x, b.y);
		}
	};
}