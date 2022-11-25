#include "minitalk.h"

void	initialize_signal_handler(void (*handler)(int, siginfo_t *, void *))
{
	struct sigaction	action;

	action.sa_flags = SA_SIGINFO;
	action.sa_sigaction = handler;
	sigemptyset(&action.sa_mask);
	sigaddset(&action.sa_mask, SIGUSR1);
	sigaddset(&action.sa_mask, SIGUSR2);
	if (sigaction(SIGUSR1, &action, NULL) == -1)
		puterr_exit("Error: sigaction with SIGUSR1");
	if (sigaction(SIGUSR2, &action, NULL) == -1)
		puterr_exit("Error: sigaction with SIGUSR2");
}

void	handler(int signo, siginfo_t *si, void *ucontext)
{
	(void)signo;
	(void)si;
	(void)ucontext;
}

void	send_bit(pid_t server_pid, char c)
{
	unsigned char	bit;
	unsigned char	i;
	unsigned char	uc;

	uc = (unsigned char)c;
	i = 0;
	bit = 0;
	while (i <= BIT_SHIFT_WIDTH)
	{
		bit = (uc >> (BIT_SHIFT_WIDTH - i)) & 0x1;
		usleep(100);
		if (bit == 0)
			kill(server_pid, SIGUSR1);
		else
			kill(server_pid, SIGUSR2);
		pause();
		i++;
	}
}

int	main(int argc, char **argv)
{
	pid_t	server_pid;
	char	*string;

	if (argc != 3)
		puterr_exit("Usage: ./client [server_pid] [string]");
	server_pid = ft_atoi(argv[1]);
	if (server_pid < MIN_PID || MAX_PID < server_pid
		|| kill(server_pid, 0) == -1)
		puterr_exit("Error: server_pid not valid");
	string = argv[2];
	initialize_signal_handler(handler);
	print_client_info(server_pid, string);
	kill(server_pid, SIGUSR1);
	pause();
	ft_putendl_fd("[client] sending message...", STDOUT_FILENO);
	while (*string)
	{
		send_bit(server_pid, *string);
		string++;
	}
	send_bit(server_pid, 0x0);
	ft_putendl_fd("[client] message sent!", STDOUT_FILENO);
	exit(EXIT_SUCCESS);
}
