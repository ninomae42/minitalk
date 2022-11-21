#include "minitalk.h"
#include "libft.h"

#define BIT_SHIFT_WIDTH 7

volatile sig_atomic_t g_sig_val;
int	cnt = 0;

void	print_server_infomation(pid_t server_pid)
{
	ft_putstr_fd("[server] PID: ", STDOUT_FILENO);
	ft_putnbr_fd(server_pid, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
}

void	handler(int signo, siginfo_t *si, void *ucontext)
{
	(void)ucontext;

	/* ft_putstr_fd("pid: ", STDOUT_FILENO); */
	/* ft_putnbr_fd(si->si_pid, STDOUT_FILENO); */
	/* ft_putstr_fd(", signo: ", STDOUT_FILENO); */
	/* ft_putnbr_fd(signo, STDOUT_FILENO); */
	/* ft_putchar_fd('\n', STDOUT_FILENO); */
	if (g_sig_val == -1)
		g_sig_val = si->si_pid;
	else
		g_sig_val = signo;
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

pid_t	establish_connection(void)
{
	pid_t	client_pid;

	pause();
	client_pid = g_sig_val;
	kill(client_pid, SIGUSR1);
	return (client_pid);
}

/* unsigned int	expect_uft8_byte_len(unsigned char c) */
/* { */
/* 	unsigned char	bit; */
/* 	unsigned char	i; */
/* 	unsigned char	uc; */
/*  */
/* 	uc = (unsigned char)c; */
/* 	i = 0; */
/* 	bit = 0; */
/* 	while (i <= BIT_SHIFT_WIDTH) */
/* 	{ */
/* 		bit = (uc >> (BIT_SHIFT_WIDTH - i) ) & 0x1; */
/* 		if (bit == 0) */
/* 			break ; */
/* 		i++; */
/* 	} */
/* 	return (i); */
/* } */

unsigned char	receive_single_char(pid_t client_pid)
{
	unsigned char	uc;
	unsigned char	i;

	uc = 0;
	i = 0;
	while (i <= BIT_SHIFT_WIDTH)
	{
		pause();
		if (g_sig_val == SIGUSR2)
			uc = uc | (0x1 << (BIT_SHIFT_WIDTH - i));
		i++;
		printf("%d\n", cnt);
		usleep(1000);
		kill(client_pid, g_sig_val);
		printf("%d\n", cnt);
		cnt++;
	}
	return (uc);
}

/* void	receive_string(pid_t client_pid) */
/* { */
/* 	unsigned char	uc; */
/*  */
/* 	uc = 0x1; */
/* 	while (uc != 0x0) */
/* 	{ */
/* 		uc = receive_single_char(client_pid); */
/* 		if (uc == 0x0) */
/* 			break ; */
/* 		write(STDOUT_FILENO, &uc, sizeof(unsigned char)); */
/* 	} */
/* } */

void	receive_message(void)
{
	pid_t	client_pid;
	unsigned char	uc;

	while (1)
	{
		uc = 0x1;
		client_pid = establish_connection();
		ft_putstr_fd("\nclient_pid: ", STDOUT_FILENO);
		ft_putnbr_fd(client_pid, STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		while (uc != 0x0)
		{
			uc = receive_single_char(client_pid);
			if (uc == 0x0)
				break ;
			/* write(STDOUT_FILENO, &uc, sizeof(unsigned char)); */
			/* kill(client_pid, SIGUSR1); */
		}
		g_sig_val = -1;
	}
}

int	main(void)
{
	pid_t	server_pid;

	g_sig_val = -1;
	initialize_signal_handler(handler);
	server_pid = getpid();
	print_server_infomation(server_pid);

	receive_message();

	exit(EXIT_SUCCESS);
}
