#include "minitalk.h"
#include <string.h>

#define BIT_SHIFT_WIDTH 7

volatile sig_atomic_t g_sig_val;

void	handler(int signo, siginfo_t *si, void *ucontext)
{
	(void)ucontext;
	/* (void)si; */

	printf("[handler]signo: %d, from_pid: %d\n", signo, si->si_pid);
	fflush(stdout);
	if (g_sig_val == -1)
		g_sig_val = si->si_pid;
	else if (signo == SIGUSR1)
		g_sig_val = 1;
	else if (signo == SIGUSR2)
		g_sig_val = 2;
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

int	main(void)
{
	pid_t	pid;
	pid_t	client_pid;

	g_sig_val = -1;
	initialize_signal_handler(handler);

	pid = getpid();
	printf("[server] pid: %d\n", pid);

	pause();
	client_pid = g_sig_val;
	kill(client_pid, SIGUSR1);
	printf("new connection established with pid: %d\n", client_pid);

	exit(EXIT_SUCCESS);
}
