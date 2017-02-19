#include "c4_game.h"
#include <memory>
#pragma comment( lib, "d3d9.lib")

int main()
{
	const int board_width = 10;
	const int board_height = 10;
	const int number_of_stones = 4;
	const int depth = 5;
	const int window_width = 800;
	const int window_height = 800;
	std::unique_ptr<C4::tigr_graphics<board_width, board_height>> graphics_object(new C4::tigr_graphics<board_width, board_height>(800, 800));
	std::unique_ptr<C4::tigr_input<board_width, board_height >> input_object(new C4::tigr_input<board_width, board_height >(graphics_object.get()));
	C4::game<C4::human_core, C4::fast_AI_core, board_width, board_height, number_of_stones, depth> gm(graphics_object.get(), input_object.get());
	gm.Run();
	return 0;
}