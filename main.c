#include "includes/lemipc.h"

/**
 * Main entry point for the LEM-IPC game.
 * Players must be started by providing a team number (1-9) as an argument.
 * @return	Returns 1 on error or if usage is not respected, 0 upon successful execution.
 */
int main (int ac, char **av)
{
	int	team;

	if (ac != 2)
	{
		ft_putstr_fd("Usage: ./lemipc [team_number 1-9]\n", 2);
		return (1);
	}
	team = ft_atoi(av[1]);
	if (team < 1 || team > 9 || !ft_isdigit(av[1][0]) || ft_strlen(av[1]) > 1)
	{
		ft_putstr_fd("Error: wrong argument ", 2);
		ft_putstr_fd(av[1], 2);
		ft_putstr_fd("\nTeam number must be between 1 and 9\n", 2);
		ft_putstr_fd("Usage: ./lemipc [team_number 1-9]\n", 2);
		return (1);
	}
	ft_start_game(team);

	return (0);
}
