#include "includes/lemipc.h"

t_game_data*	game_data = NULL;
int				msgq_id = -1;
int				sem_id = -1;

/**
 * Initialize shared resources required for the game.
 * This includes setting up shared memory for game data, initializing the
 * message queue, and setting up semaphores for synchronization. If any of the
 * initializations fail, the program will exit with an error message.
 */
void	initialize_shared_resources()
{
	int shm_id = shmget(SHM_KEY, sizeof(t_game_data), IPC_CREAT | 0666);
	if (shm_id == -1)
	{
		perror("shmget failed in initialization");
		exit(1);
	}
	game_data = (t_game_data *)shmat(shm_id, NULL, 0);
	if (game_data == (t_game_data *)-1)
	{
		perror("shmat failed in initialization");
		exit(1);
	}

	ft_memset(game_data, 0, sizeof(t_game_data));

	game_data->game_status = WAITING;
	game_data->player_count = 1;

	msgq_id = msgget(MSGQ_KEY, IPC_CREAT | 0666);
	if (msgq_id == -1)
	{
		perror("msgget failed in initialization");
		exit(1);
	}

	sem_id = semget(SEM_KEY, 1, IPC_CREAT | 0666);
	if (sem_id == -1)
	{
		perror("semget failed in initialization");
		exit(1);
	}

	union semun
	{
		int val;
		struct semid_ds *buf;
		unsigned short *array;
	} sem_union;

	sem_union.val = 1;
	if (semctl(sem_id, 0, SETVAL, sem_union) == -1)
	{
		perror("semctl failed in initialization");
		exit(1);
	}
}

/**
 * Check if shared resources exist.
 * @return	Returns 1 if the shared resources exist and 0 otherwise.
 *          If there's an error other than non-existence, the program exits.
 */
int	resources_exist()
{
	int	shm_id;

	shm_id = shmget(SHM_KEY, 0, 0);

	if (shm_id == -1)
	{
		if (errno == ENOENT)
			return (0);
		else
		{
			perror("shmget failed");
			exit(1);
		}
	}
	return (1);
}

/**
 * Attach the game to already created shared resources.
 * The game attaches to the shared memory segment for game data, the message
 * queue, and the semaphore. If any of these attachments fail, the program
 * will exit with an error message.
 */
void	attach_to_shared_resources()
{
	int shm_id = shmget(SHM_KEY, sizeof(t_game_data), 0666);
	if (shm_id == -1)
	{
		perror("shmget failed in attach");
		exit(1);
	}

	game_data = (t_game_data *)shmat(shm_id, NULL, 0);
	if (game_data == (t_game_data *)-1)
	{
		perror("shmat failed");
		exit(1);
	}

	msgq_id = msgget(MSGQ_KEY, 0666);
	if (msgq_id == -1)
	{
		perror("msgget failed in attach");
		exit(1);
	}

	sem_id = semget(SEM_KEY, 1, 0666);
	if (sem_id == -1)
	{
		perror("semget failed in attach");
		exit(1);
	}
	lock_semaphore(sem_id);
	game_data->player_count++;
	game_data->game_status = is_other_team(current_player->team);
	if (!check_space_onboard())
	{
		unlock_semaphore(sem_id);
		ft_putstr_fd("No space left on board, exiting...\n", 2);
		exit(1);
	}
	unlock_semaphore(sem_id);
}

/**
 * Clean up and release the shared resources that have been allocated.
 * This involves detaching from the shared memory segment, deleting the message
 * queue, and deleting the semaphore. If there are any errors during this
 * process, they will simply be ignored since the cleanup is typically
 * performed at the end of the program's execution.
 */
void	cleanup_resources()
{
	if (game_data)
	{
		shmdt(game_data);
		int shm_id = shmget(SHM_KEY, 0, 0666);
		if (shm_id != -1)
			shmctl(shm_id, IPC_RMID, NULL);
	}

	if (msgq_id != -1)
		msgctl(msgq_id, IPC_RMID, NULL);

	if (sem_id != -1)
		semctl(sem_id, 0, IPC_RMID, NULL);
}
