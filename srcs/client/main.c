#include "minitalk.h"
#include "libft.h"
#include <stdio.h>

#define MIN_PID 100
#define MAX_PID 99998
#define BIT_SHIFT_WIDTH 7

volatile sig_atomic_t	g_sig_val;
int	cnt = 0;

void	handler(int signo, siginfo_t *si, void *ucontext)
{
	(void)ucontext;
	(void)si;
	(void)signo;

	/* ft_putendl_fd("handler\n", STDOUT_FILENO); */
	/* printf("[handler]signo: %d, from_pid: %d\n", signo, si->si_pid); */
	/* fflush(stdout); */
	/* ft_putstr_fd("pid: ", STDOUT_FILENO); */
	/* ft_putnbr_fd(si->si_pid, STDOUT_FILENO); */
	/* ft_putstr_fd(", signo: ", STDOUT_FILENO); */
	/* ft_putnbr_fd(signo, STDOUT_FILENO); */
	/* ft_putchar_fd('\n', STDOUT_FILENO); */
}

pid_t	check_and_set_server_pid(char *pid_str)
{
	pid_t	server_pid;

	server_pid = ft_atoi(pid_str);
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

void	initialize_signal_handler(void (*handler)(int, siginfo_t *, void *))
{
	struct sigaction sa;

	ft_memset(&sa, 0, sizeof(struct sigaction));
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
		if (bit == 0)
			kill(server_pid, SIGUSR1);
		else
			kill(server_pid, SIGUSR2);
		printf("%d\n", cnt);
		pause();
		printf("%d\n", cnt);
		cnt++;
		i++;
	}
}

void	ping_to_server(pid_t server_pid)
{
	printf("g_sig_val: %d\n", g_sig_val);
	fflush(stdout);
	kill(server_pid, SIGUSR1);
	pause();
	write(STDOUT_FILENO, "here OK\n", 8);
	printf("g_sig_val: %d\n", g_sig_val);
	fflush(stdout);
	printf("connection established\n");
	fflush(stdout);
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

	string = argv[2];
	printf("server_pid: %d, string: %s\n", server_pid, string);
	fflush(stdout);

	while (*string)
	{
		send_bit(server_pid, *string);
		string++;
	}
	send_bit(server_pid, 0x0);
	exit(EXIT_SUCCESS);
}
