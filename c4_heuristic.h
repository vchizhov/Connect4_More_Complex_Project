#pragma once

namespace C4
{
	template<template<int, int, int> class game_state_class, int board_width, int board_height, int depth, int number_of_stones>
	class basic_heuristic
	{
	public:
		static const int extreme_value = 2;

		static int evaluate_state(const game_state_class<board_width, board_height, depth>& game_state)
		{
			//function to compute the sign:
			//- corresponds to -1
			//+ corresponds to player 2
			int sign_based_on_player = static_cast<int>(game_state.get_current_player());
			int x = game_state.get_last_move_x();
			int y = game_state.get_last_move_y();
			BOARD_VALUE player = game_state.get_current_player();
			//a sum used to measure the number of pieces in a row
			int sum = 1;

			//check horizontally
			int t = x - 1;
			while (t > -1 && sum < number_of_stones && game_state.get(t, y) == player)
			{
				sum++;
				t -= 1;
			}

			t = x + 1;
			while (t < board_width && sum < number_of_stones && game_state.get(t, y) == player)
			{
				sum++;
				t += 1;
			}
			if (sum >= number_of_stones)
				return sign_based_on_player;

			//vertical
			sum = 1;
			t = y - 1;
			while (t > -1 && sum < number_of_stones && game_state.get(x, t) == player)
			{
				sum++;
				t -= 1;
			}

			t = y + 1;
			while (t < board_height && sum < number_of_stones && game_state.get(x, t) == player)
			{
				sum++;
				t += 1;
			}
			if (sum >= number_of_stones)
				return sign_based_on_player;

			//main diagonal
			sum = 1;
			t = 1;
			while (x - t > -1 && y - t > -1 && sum < number_of_stones && game_state.get(x - t, y - t) == player)
			{
				sum++;
				t += 1;
			}

			t = 1;
			while (x + t < board_width && y + t < board_height && sum < number_of_stones && game_state.get(x + t, y + t) == player)
			{
				sum++;
				t += 1;
			}
			if (sum >= number_of_stones)
				return sign_based_on_player;

			//secondary diagonal
			sum = 1;
			t = 1;
			while (x + t <board_width && y - t>-1 && sum < number_of_stones && game_state.get(x + t, y - t) == player)
			{
				sum++;
				t += 1;
			}

			t = 1;
			while (x - t > -1 && y + t < board_height && sum < number_of_stones && game_state.get(x - t, y + t) == player)
			{
				sum++;
				t += 1;
			}
			if (sum >= number_of_stones)
				return sign_based_on_player;

			return 0;
		};
	};
}