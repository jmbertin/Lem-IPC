#include "includes/lemipc.h"

/**
 * Find the closest enemy to a given player on the board.
 * @param	p	Pointer to the player for which we want to find the closest enemy.
 * @return	Returns the position (coordinates) of the closest enemy on the board.
 */
static s_point	find_closest_enemy(t_player *p)
{
	int		min_distance = (MAP_SIZE * 2) + 1;
	s_point	closest_enemy = {-1, -1};

	for (int i = 0; i < MAP_SIZE; i++)
	{
		for (int j = 0; j < MAP_SIZE; j++)
		{
			if (game_data->board[i][j] != 0 && game_data->board[i][j] != p->team)
			{
				int distance = abs(p->x - i) + abs(p->y - j);
				if (distance < min_distance)
				{
					min_distance = distance;
					closest_enemy.x = i;
					closest_enemy.y = j;
				}
			}
		}
	}
	return (closest_enemy);
}

/**
 * Try to surround an enemy by moving to a free position.
 * @param	enemy	Position (coordinates) of the enemy to surround.
 * @param	player	Pointer to the current player trying to surround the enemy.
 * @param	dx		Array containing potential X-axis movement values.
 * @param	dy		Array containing potential Y-axis movement values.
 * @return	Returns 1 if the player successfully surrounds the enemy, 0 otherwise.
 */
static int	try_surround_ennemy(s_point enemy, t_player *player, int *dx, int *dy)
{
	for (int i = 0; i < 4; i++)
	{
		int	nx = player->x + dx[i];
		int	ny = player->y + dy[i];

		if (nx >= 0 && nx < MAP_SIZE && ny >= 0 && ny < MAP_SIZE && game_data->board[nx][ny] == 0)
		{
			int	opposite_x = player->x - dx[i];
			int	opposite_y = player->y - dy[i];

			if (opposite_x == enemy.x && opposite_y == enemy.y && game_data->board[opposite_x][opposite_y] != player->team)
			{
				game_data->board[nx][ny] = player->team;
				game_data->board[player->x][player->y] = 0;
				player->x = nx;
				player->y = ny;
				return (1);
			}
		}
	}
	return (0);
}

/**
 * Move the player closer to a specified enemy.
 * @param	enemy	Position (coordinates) of the enemy to approach.
 * @param	player	Pointer to the current player that needs to move closer.
 * @param	dx		Array containing potential X-axis movement values.
 * @param	dy		Array containing potential Y-axis movement values.
 */
static void	move_closer(s_point enemy, t_player *player, int *dx, int *dy)
{
	int	best_dir = -1;
	int	min_distance = (MAP_SIZE * 2) + 1;

	for (int i = 0; i < 4; i++)
	{
		int nx = player->x + dx[i];
		int ny = player->y + dy[i];

		if (nx >= 0 && nx < MAP_SIZE && ny >= 0 && ny < MAP_SIZE && game_data->board[nx][ny] == 0)
		{
			int	distance = abs(nx - enemy.x) + abs(ny - enemy.y);
			if (distance < min_distance)
			{
				min_distance = distance;
				best_dir = i;
			}
		}
	}

	if (best_dir != -1)
	{
		game_data->board[player->x][player->y] = 0;
		player->x += dx[best_dir];
		player->y += dy[best_dir];
		game_data->board[player->x][player->y] = player->team;
	}
}

/**
 * Handle the logic to move a player. It can either surround an enemy or just move closer to them.
 */
void	move_player()
{
	t_player	*player = current_player;
	int			dx[] = {-1, 0, 1, 0};
	int			dy[] = {0, 1, 0, -1};
	s_point		enemy = find_closest_enemy(player);

	if ((enemy.x == -1 && enemy.y == -1))
		return;

	if (try_surround_ennemy(enemy, player, dx, dy))
		return;

	move_closer(enemy, player, dx, dy);
}

/**
 * Spawn a player on the board in a random and free position.
 * The function ensures that the player does not spawn on a taken position.
 */
void	spawn_player()
{
	lock_semaphore(sem_id);
	int	x = 0;
	int	y = 0;

	x = rand() % MAP_SIZE;
	y = rand() % MAP_SIZE;

	srand(time(NULL));

	while (!is_position_free(x, y))
	{
		x = rand() % MAP_SIZE;
		y = rand() % MAP_SIZE;
	}

	current_player->x = x;
	current_player->y = y;

	game_data->board[x][y] = current_player->team;
	unlock_semaphore(sem_id);
}
