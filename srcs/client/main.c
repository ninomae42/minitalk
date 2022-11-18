#include "minitalk.h"
#include <string.h>

#define MIN_PID 100
#define MAX_PID 99998

volatile sig_atomic_t	g_sig_val;

void	handler(int signo, siginfo_t *si, void *ucontext)
{
	(void)ucontext;
	/* (void)si; */

	printf("[handler]signo: %d, from_pid: %d\n", signo, si->si_pid);
	fflush(stdout);
	if (signo == SIGUSR1)
	{
		g_sig_val = 1;
	}
	else if (signo == SIGUSR2)
	{
		g_sig_val = 2;
	}
}

pid_t	check_and_set_server_pid(char *pid_str)
{
	pid_t	server_pid;

	server_pid = atoi(pid_str);
	if (server_pid < MIN_PID || MAX_PID < server_pid)
	{
		printf("server PID not valid\n");
		exit(EXIT_FAILURE);
	}
	if (kill(server_pid, 0) == -1)
	{
		perror("server PID not valid");
		exit(EXIT_FAILURE);
	}
	return (server_pid);
}

void	ping_to_server(pid_t pid)
{
	kill(pid, SIGUSR1);
	pause();
}

void	initialize_signal_handler(void (*handler)(int, siginfo_t *, void *))
{
	struct sigaction sa;

	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGUSR1);
	sigaddset(&sa.sa_mask, SIGUSR2);
	sa.sa_sigaction = handler;
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
	{
		perror("sigaction1");
		exit(EXIT_FAILURE);
	}
	if (sigaction(SIGUSR2, &sa, NULL) == -1)
	{
		perror("sigaction2");
		exit(EXIT_FAILURE);
	}
}

int	main(int argc, char **argv)
{
	pid_t	server_pid;
	char	*string;

	if (argc != 3)
	{
		printf("Usage: ./client [server PID] [string to send]\n");
		exit(EXIT_FAILURE);
	}
	server_pid = check_and_set_server_pid(argv[1]);

	g_sig_val = 0;
	initialize_signal_handler(handler);
	ping_to_server(server_pid);
	printf("connection established\n");

	string = argv[2];
	printf("server_pid: %d, string: %s\n", server_pid, string);

	exit(EXIT_SUCCESS);
}
