#ifndef MINITALK_H
# define MINITALK_H
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include "libft.h"
# define BIT_SHIFT_WIDTH 7
# define MIN_PID 100
# define MAX_PID 99998

# define SERVER_WAIT_MSG "[server] waiting for message from client"

typedef struct s_info
{
	volatile sig_atomic_t	client_pid;
	volatile sig_atomic_t	sigval;
}	t_info;

// server/utils.c
void	print_server_info(pid_t server_pid);
void	puterr_exit(char *msg);

// client/utils.c
void	print_client_info(pid_t server_pid, char *message);

#endif
