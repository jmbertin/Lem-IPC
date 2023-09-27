#include "includes/lemipc.h"

t_player	*current_player = NULL;

/**
 * Handler function for SIGINT signal.
 * This function cleans up resources and exits the process.
 * @param sig The signal number.
 */
static void	sigint_handler(int sig)
{
	(void)sig;

	lock_semaphore(sem_id);

	if (game_data->player_count == 1)
	{
		unlock_semaphore(sem_id);
		cleanup_resources();
		exit(1);
	}
	else
	{
		game_data->player_count--;
		game_data->board[current_player->x][current_player->y] = 0;
		unlock_semaphore(sem_id);
		exit(1);
	}
}

/**
 * Counts the number of teams currently on the game board.
 * @return The number of teams on the game board.
 */
static int	count_teams()
{
	int	teams_on_map[9] = {0};
	int	team_count = 0;

	for (int i = 0; i < MAP_SIZE; i++)
	{
		for (int j = 0; j < MAP_SIZE; j++)
		{
			int team = game_data->board[i][j];
			if (team != 0 && teams_on_map[team - 1] == 0)
			{
				teams_on_map[team - 1] = 1;
				team_count++;
			}
		}
	}
	return (team_count);
}

/**
 * Checks if the game is still running.
 * This function checks the status of the game and determines whether it is still running
 * or has ended. It handles the shared memory and semaphore.
 * @return 1 if the game is running, 0 if the game has ended.
 */
static int	game_is_running()
{
	lock_semaphore(sem_id);
	if (!game_data)
	{
		int shm_id = shmget(SHM_KEY, SHM_SIZE, 0666);
		if (shm_id == -1)
		{
			perror("shmget failed in game_is_running");
			exit(1);
		}
		game_data = (t_game_data*)shmat(shm_id, NULL, 0);
		if (game_data == (t_game_data*)-1)
		{
			perror("shmat failed in game_is_running");
			exit(1);
		}
	}
	if (game_data->game_status == WAITING)
	{
		unlock_semaphore(sem_id);
		return (1);
	}
	else if (game_data->game_status == PLAYING)
	{
		int	team_count = count_teams();
		unlock_semaphore(sem_id);
		if (team_count > 1)
			return (1);
		else
		{
			game_data->game_status = FINISHED;
			return (0);
		}
	}
	game_data->game_status = FINISHED;
	unlock_semaphore(sem_id);
	return (0);
}

/**
 * Implements the player's thinking step by simulating a random delay.
 */
static void	think()
{
	double			random_factor = 0.5 + (rand() / (double)RAND_MAX);
	unsigned int	random_delay = REFRESH_DELAY * random_factor;
	usleep(random_delay);
}

/**
 * Runs the main loop of the game.
 * This function runs the main loop of the game until it ends.
 * It updates the game state, moves the player, and refreshes the display.
 * @return 0 if the game ended normally, 1 if the game is still running.
 */
int	loop()
{
	while (game_is_running())
	{
		lock_semaphore(sem_id);
		display_board();

		if (is_player_dead(current_player->team, current_player->x, current_player->y))
		{
			if (is_last_player())
			{
				display_board();
				cleanup_resources();
				unlock_semaphore(sem_id);
				return (0);
			}
			else
			{
				unlock_semaphore(sem_id);
				return (0);
			}
		}
		move_player();
		display_board();
		unlock_semaphore(sem_id);
		think();
	}
	return (1);
}

/**
 * Starts the game and runs the main loop.
 * Initializes game resources, spawns the player, and runs the main loop of the game.
 * @param team The team number of the player.
 * @return 0 when the game ends.
 */
int	ft_start_game(int team)
{
	t_player	player;

	player.x = 0;
	player.y = 0;
	player.team = team;
	current_player = &player;

	if (!resources_exist())
		initialize_shared_resources();
	else
		attach_to_shared_resources();

	spawn_player();
	signal(SIGINT, sigint_handler);

	if (!loop())
		return (0);

	lock_semaphore(sem_id);
	display_board();
	game_data->player_count--;
	display_finalstatus(1);
	if (is_last_player())
	{
		cleanup_resources();
		return (0);
	}
	unlock_semaphore(sem_id);
	return (0);
}
