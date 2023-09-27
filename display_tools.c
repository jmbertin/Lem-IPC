#include "includes/lemipc.h"

/**
 * Change the terminal color based on the given character.
 * @param	c	Character representing the color to be changed to.
 */
static void change_term_color(char c)
{
	if (c == 'r')
		ft_printf(RED);
	else if (c == 'g')
		ft_printf(GREEN);
	else if (c == 'b')
		ft_printf(BLUE);
	else if (c == 'y')
		ft_printf(YELLOW);
	else if (c == 'w')
		ft_printf(WHITE);
}

/**
 * Draw a frame with a string left align.
 * @param	width	Width of the frame.
 * @param	str		String to be displayed in the frame.
 * @param	color	Character representing the color of the string.
 */
static void draw_frame_left(int width, const char *str, char color)
{
	ft_printf("| ");
	change_term_color(color);
	ft_printf("%s", str);
	change_term_color('w');
	for (int i = 0; i < (width - (int)ft_strlen(str) - 2) - 1; i++)
		ft_printf(" ");
	ft_printf("|\n");
}

/**
 * Draw an empty line of a specified width.
 * @param	width	Width of the line.
 */
void draw_emptyline(int width)
{
	ft_printf("|");
	for (int i = 0; i < width - 2; i++)
		ft_printf(" ");
	ft_printf("|\n");
}

/**
 * Draw a line filled with a specific character of a specified width.
 * @param	width	Width of the line.
 * @param	c		Character with which the line is filled.
 */
void draw_line(int width, char c)
{
	for (int i = 0; i < width; i++)
		ft_printf("%c", c);
	ft_printf("\n");
}

/**
 * Draw a frame with a string center align.
 * @param	width	Width of the frame.
 * @param	str	String to be displayed in the frame.
 * @param	color	Character representing the color of the string.
 */
void draw_frame_center(int width, const char *str, char color)
{
	ft_printf("|");
	for (int i = 0; i < ((width - (int)ft_strlen(str)) / 2) - 1; i++)
		ft_printf(" ");

	change_term_color(color);
	ft_printf("%s", str);
	change_term_color('w');

	if (width % 2 != 0)
		ft_printf(" ");
	for (int i = 0; i < ((width - (int)ft_strlen(str)) / 2) - 1; i++)
		ft_printf(" ");
	ft_printf("|\n");
}

/**
 * Draw the introduction header of the game.
 * @param	width	Width of the frame.
 */
void draw_header(int width)
{
	draw_line(width, '-');
	draw_frame_center(width, "42 Nice - jbertin ", 'w');
	draw_line(width, '-');
	draw_frame_center(width, "LEM-IPC V1.0", 'b');
	draw_line(width, '-');
}

/**
 * Draw the outro frame showing the game status.
 * @param	width	Width of the frame.
 */
void draw_outro(int width)
{
	draw_line(width, '-');
	if (game_data->game_status == WAITING)
		draw_frame_left(width, "Status : Waiting....", 'y');
	else if (game_data->game_status == PLAYING)
		draw_frame_left(width, "Status : Playing....", 'g');
	else if (game_data->game_status == FINISHED)
		draw_frame_left(width, "Status : Finished...", 'r');
	draw_line(width, '-');
}

/**
 * Draw the game board.
 */
void draw_board()
{
	for (int i = 0; i < MAP_SIZE; i++)
	{
		ft_printf("|  ");
		for (int j = 0; j < MAP_SIZE; j++)
		{
			if (current_player->x == i && current_player->y == j)
			{
				change_term_color('b');
				ft_printf("%d ", game_data->board[i][j]);
			}
			else if (current_player->team == game_data->board[i][j])
			{
				change_term_color('g');
				ft_printf("%d ", game_data->board[i][j]);
			}
			else if (game_data->board[i][j] == 0)
			{
				change_term_color('w');
				ft_printf("%d ", game_data->board[i][j]);
			}
			else
			{
				change_term_color('r');
				ft_printf("%d ", game_data->board[i][j]);
			}
			change_term_color('w');
		}
		ft_printf(" |\n");
	}
}
