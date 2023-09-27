#include "includes/lemipc.h"

/**
 * Lock the semaphore to ensure mutual exclusion.
 * This function decrements the value of the semaphore. If the semaphore's value
 * goes below zero, the calling process will block and wait until the value
 * becomes greater than or equal to zero.
 * @param sem_id The identifier of the semaphore to be locked.
 */
void	lock_semaphore(int sem_id)
{
	struct sembuf	op;

	op.sem_num = 0;
	op.sem_op = -1;
	op.sem_flg = 0;
	if (semop(sem_id, &op, 1) == -1)
	{
		perror("semop P (lock) failed");
		exit(1);
	}
}

/**
 * Unlock the semaphore, allowing other processes to access shared resources.
 * This function increments the value of the semaphore. If there are processes
 * waiting on the semaphore, one of them will be unblocked.
 * @param sem_id The identifier of the semaphore to be unlocked.
 */
void	unlock_semaphore(int sem_id)
{
	struct sembuf	op;

	op.sem_num = 0;
	op.sem_op = 1;
	op.sem_flg = 0;
	if (semop(sem_id, &op, 1) == -1)
	{
		perror("semop V (unlock) failed");
		exit(1);
	}
}
