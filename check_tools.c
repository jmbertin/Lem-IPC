#include "includes/lemipc.h"

/**
 * Check if there's a free space on the game board.
 * @return	Returns 1 if a free space exists, 0 otherwise.
 */
int	check_space_onboard()
{
	for (int i = 0; i < MAP_SIZE; i++)
	{
		for (int j = 0; j < MAP_SIZE; j++)
		{
			if (game_data->board[i][j] == 0)
				return (1);
		}
	}
	return (0);
}

/**
 * Check if there's another team present on the game board.
 * @param	team	The team number to be checked.
 * @return	Returns 1 if another team is found, 0 otherwise.
 */
int	is_other_team(int team)
{
	for (int i = 0; i < MAP_SIZE; i++)
	{
		for (int j = 0; j < MAP_SIZE; j++)
		{
			if (game_data->board[i][j] != 0 && game_data->board[i][j] != team)
				return (1);
		}
	}
	return (0);
}

/**
 * Checks if a specified position on the game board is free.
 * @param	x	X-coordinate of the position.
 * @param	y	Y-coordinate of the position.
 * @return	Returns 1 if the position is free, 0 otherwise.
 */
int	is_position_free(int x, int y)
{
	if (game_data->board[x][y] == 0)
		return (1);
	return (0);
}

/**
 * Checks if the current player is the last remaining player.
 * @return	Returns 1 if the current player is the last one, 0 otherwise.
 */
int	is_last_player()
{
	if (game_data->player_count == 0)
		return (1);
	return (0);
}

/**
 * Checks if a player is surrounded and hence, dead.
 * @param	team	The team number of the player.
 * @param	x	X-coordinate of the player.
 * @param	y	Y-coordinate of the player.
 * @return	Returns 1 if the player is dead, 0 otherwise.
 */
int	is_player_dead(int team, int x, int y)
{
	int dx[] = {-1, 1, 0, 0, -1, 1, 1, -1};
	int dy[] = {0, 0, -1, 1, -1, 1, -1, 1};

	for (int i = 0; i < 8; i += 2)
	{
		int x1 = x + dx[i];
		int y1 = y + dy[i];
		int x2 = x + dx[i+1];
		int y2 = y + dy[i+1];

		if (x1 >= 0 && x1 < MAP_SIZE && y1 >= 0 && y1 < MAP_SIZE &&
			x2 >= 0 && x2 < MAP_SIZE && y2 >= 0 && y2 < MAP_SIZE &&
			game_data->board[x1][y1] != 0 && game_data->board[x1][y1] != team &&
			game_data->board[x2][y2] != 0 && game_data->board[x2][y2] == game_data->board[x1][y1])
		{
			game_data->board[x][y] = 0;
			game_data->player_count--;
			display_finalstatus(0);
			return (1);
		}
	}

	return (0);
}
