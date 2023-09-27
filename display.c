#include "includes/lemipc.h"

/**
 * Display the final status of the game.
 * @param	res	Result of the game: 1 if the team wins, 0 otherwise.
 */
void display_finalstatus(int res)
{
	if (res)
		draw_frame_center((MAP_SIZE * 2) -1 + 6, "YOUR TEAM WINS !", 'g');
	else
		draw_frame_center((MAP_SIZE * 2) -1 + 6, " YOU LOOSE - GAME OVER", 'r');
	draw_line((MAP_SIZE * 2) -1 + 6, '-');
}

/**
 * Display the entire game board with the header and outro.
 */
void display_board()
{
	int	width = (MAP_SIZE * 2) -1 + 6;
	ft_printf(CLEAR);
	draw_header(width);
	draw_emptyline(width);
	draw_board();
	draw_emptyline(width);
	draw_outro(width);
}
