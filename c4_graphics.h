#pragma once
#include <string>
#include <algorithm>
#include "c4_structs.h"
#include "c4_game_state.h"
#include "tigr.h"
namespace C4
{

	template<int board_width, int board_height>
	class graphics
	{
	public:
		virtual void make_move(player_move_outcome) = 0;
		virtual void erase_move(player_move_outcome) = 0;
		virtual void render()= 0;
		virtual void render_outcome()= 0;
	};

	template<int board_width, int board_height>
	class tigr_graphics : public graphics<board_width, board_height>
	{
	private:
		class GraphicsBoard
		{
		private:
			const int x, y, w, h;
			Tigr* board;
		public:

			TPixel gridCol, p1, p2;

			GraphicsBoard(int x, int y, int width, int height, TPixel gridCol, TPixel p1, TPixel p2)
				:x(x), y(y), w(width+1), h(height+1), gridCol(gridCol), p1(p1), p2(p2)
			{
				board = tigrBitmap(w, h);
				tigrClear(board, tigrRGB(255, 255, 255));
				//tigrRect(board, 0, 0, w, h, gridCol);
				int dx = w / board_width;
				int dy = h / board_height;
				for (int i = 0; i <= w; i += dx)
				{
					tigrLine(board, i, 0, i, h, gridCol);
				}
				tigrLine(board, w-1, 0, w-1, h, gridCol);
				for (int j = 0; j <= h; j += dy)
				{
					tigrLine(board, 0, j, w, j, gridCol);
				}
				tigrLine(board, 0, h-1, w, h-1, gridCol);
			}

			void addP1(int x, int y)
			{
				int dx = w / board_width;
				int dy = h / board_height;
				tigrFill(board, x*dx + 1, (board_height - 1 - y)*dy + 1, dx - 1, dy - 1, p1);
			}

			void addP2(int x, int y)
			{
				int dx = w / board_width;
				int dy = h / board_height;
				tigrFill(board, x*dx + 1, (board_height - 1 - y)*dy + 1, dx - 1, dy - 1, p2);
			}

			void erase(int x, int y)
			{
				int dx = w / board_width;
				int dy = h / board_height;
				tigrFill(board, x*dx + 1, (board_height - 1 - y)*dy + 1, dx - 1, dy - 1, tigrRGB(255, 255, 255));
			}

			void draw(Tigr* bmp)
			{
				tigrBlit(bmp, board, x, y, 0, 0, w, h);
			}

			int get_width() const
			{
				return w;
			}

			int get_height() const
			{
				return h;
			}

			//convert from screen coordinates to grid coordinates
			void convertXY(int& x, int& y) const
			{
				float dx = w / board_width;
				float dy = h / board_height;
				x = floor((x-this->x)/dx);
				y = floor((y-this->y)/dy);	
			}
		};
	private:
		standard_board<board_width, board_height> brd;
		Tigr* screen;
		GraphicsBoard board;
		player_move_outcome last_move;
		std::string player1_win_caption;
		std::string player2_win_caption;
		std::string draw_caption;
	public:
		tigr_graphics(int window_width, int window_height, std::string app_title = std::string("Connect4"), int margin_px = 100, int margin_py = 100)
			:board(margin_px, margin_py, window_width - 2 * margin_px, window_height - 2 * margin_py,
				tigrRGB(0, 0, 0), tigrRGB(255, 0, 0), tigrRGB(0, 255, 0))
		{
			screen = tigrWindow(window_width, window_height, app_title.c_str(), 0);

			player1_win_caption = std::string("Player 1 wins!");
			player2_win_caption = std::string("Player 2 wins!");
			draw_caption = std::string("It's a draw!");
		}

		void make_move(player_move_outcome mv)
		{
			last_move = mv;
			brd.set(mv.x, mv.y, static_cast<BOARD_VALUE>(mv.player));
			switch (mv.player)
			{
			case PLAYER_1:
				board.addP1(mv.x, mv.y);
				break;
			case PLAYER_2:
				board.addP2(mv.x, mv.y);
				break;
			}
		}

		void erase_move(player_move_outcome mv)
		{
			brd.set(mv.x, mv.y, BOARD_VALUE_EMPTY);
			board.erase(mv.x, mv.y);
		}

		void render()
		{
			tigrClear(screen, tigrRGB(255, 255, 255));
			board.draw(screen);
			tigrUpdate(screen);
		}

		void render_outcome()
		{
			tigrClear(screen, tigrRGB(255, 255, 255));
			board.draw(screen);
			switch (last_move.outcome)
			{
			case OUTCOME_PLAYER_1_WIN:
				tigrPrint(screen, tfont, board.get_width() / 2 - 10, board.get_height() / 2 - 5, board.p1, player1_win_caption.c_str());
				break;
			case OUTCOME_PLAYER_2_WIN:
				tigrPrint(screen, tfont, board.get_width() / 2 - 10, board.get_height() / 2 - 5, board.p2, player2_win_caption.c_str());
				break;
			case OUTCOME_DRAW:
				tigrPrint(screen, tfont, board.get_width() / 2 - 10, board.get_height() / 2 - 5, board.gridCol, draw_caption.c_str());
				break;
			}
			tigrUpdate(screen);
		}

		void get_xy_from_mouse(int& x, int& y) const
		{
			int b;
			tigrMouse(screen, &x, &y, &b);
			
			board.convertXY(x, y);
		}

		Tigr* get_window()
		{
			return screen;
		}

		int column_top(int x) const
		{
			return brd.get_column_top(x);
		}

		~tigr_graphics()
		{
			tigrFree(screen);
		}
	};
}