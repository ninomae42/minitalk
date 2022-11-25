/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tashimiz <tashimiz@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 16:58:40 by tashimiz          #+#    #+#             */
/*   Updated: 2022/11/25 16:58:42 by tashimiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

volatile t_info	g_info;

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
	(void)ucontext;
	if (g_info.sigval == -1)
		g_info.client_pid = si->si_pid;
	if (si->si_pid == g_info.client_pid)
		g_info.sigval = signo;
}

unsigned char	receive_single_char(pid_t client_pid)
{
	unsigned char	uc;
	unsigned char	i;

	uc = 0;
	i = 0;
	while (i <= BIT_SHIFT_WIDTH)
	{
		pause();
		if (g_info.sigval == SIGUSR2)
			uc = uc | (0x1 << (BIT_SHIFT_WIDTH - i));
		g_info.sigval = 0;
		if (i != 7)
		{
			usleep(100);
			kill(client_pid, SIGUSR1);
		}
		i++;
	}
	return (uc);
}

int	main(void)
{
	pid_t			server_pid;
	unsigned char	uc;

	g_info.sigval = -1;
	initialize_signal_handler(handler);
	server_pid = getpid();
	while (1)
	{
		uc = 0x1;
		print_server_info(server_pid);
		ft_putendl_fd(SERVER_WAIT_MSG, STDOUT_FILENO);
		pause();
		usleep(100);
		kill(g_info.client_pid, SIGUSR1);
		while (g_info.sigval != -1 && uc != 0x0)
		{
			uc = receive_single_char(g_info.client_pid);
			write(STDOUT_FILENO, &uc, sizeof(unsigned char));
			usleep(100);
			kill(g_info.client_pid, SIGUSR1);
		}
		g_info.sigval = -1;
		ft_putchar_fd('\n', STDOUT_FILENO);
	}
	exit(EXIT_SUCCESS);
}
