#ifndef LEMIPC_H
# define LEMIPC_H

# include <sys/ipc.h>
# include <sys/shm.h>
# include <sys/msg.h>
# include <sys/sem.h>
# include <stdio.h>
# include <time.h>
# include <signal.h>
# include "libft.h"

/*
** Game settings
*/
# define REFRESH_DELAY 200000  // 200 ms
# define MAP_SIZE 10

/*
** IPC keys
*/
# define SHM_KEY 424242
# define MSGQ_KEY 212121
# define SEM_KEY 987654

/*
** IPC flags
*/
# define SHM_SIZE sizeof(t_game_data)
# define GAME_STATUS_FLAG_INDEX 0

/*
** Game status
*/
# define WAITING 0
# define PLAYING 1
# define FINISHED 2

/*
** Game colors
*/
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;36m"
# define WHITE "\033[0;39m"
# define CLEAR "\033[H\033[J"

/*
** Game structure
* - game_status: 0 = waiting, 1 = playing, 2 = finished
* - player_count: number of players in the game
* - board: the game board
*/
typedef struct	s_game_data
{
	int	game_status;
	int	player_count;
	int	board[MAP_SIZE][MAP_SIZE];
}				t_game_data;

/*
** Coordinates structure
*/
typedef struct	t_point
{
	int	x;
	int	y;
}				s_point;

/*
** Player structure
*/
typedef struct	t_player
{
	int	x;
	int	y;
	int team;
}				t_player;

extern t_game_data	*game_data;
extern int			msgq_id;
extern int			sem_id;
extern t_player		*current_player;

// Display
void	display_finalstatus(int res);
void	display_board();

// Display tools
void	draw_emptyline(int width);
void	draw_line(int width, char c);
void	draw_frame_center(int width, const char *str, char color);
void	draw_header(int width);
void	draw_outro(int width);
void	draw_board();

// Move
void	move_player();
void	spawn_player();

// Check tools
int		check_space_onboard();
int		is_other_team(int team);
int		is_position_free(int x, int y);
int		is_last_player();
int		is_player_dead(int team, int x, int y);

// Semaphore
void	lock_semaphore(int sem_id);
void	unlock_semaphore(int sem_id);

// Ressources
void	initialize_shared_resources();
int		resources_exist();
void	attach_to_shared_resources();
void	cleanup_resources();

int		ft_start_game(int team);

#endif
