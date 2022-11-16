#include "minitalk.h"
#include <string.h>

#define BIT_SHIFT_WIDTH 7

volatile sig_atomic_t g_sig_val;

void	handler(int signo, siginfo_t *si, void *ucontext)
{
	(void)ucontext;
	(void)si;

	if (signo == SIGUSR1)
	{
		g_sig_val = 1;
	}
	else if (signo == SIGUSR2)
	{
		g_sig_val = 2;
	}
}

unsigned char	receive_char_base(void)
{
	unsigned char	uc;
	unsigned char	i;

	uc = 0;
	i = 0;
	while (i <= BIT_SHIFT_WIDTH)
	{
		pause();
		if (g_sig_val != 0)
		{
			if (g_sig_val == 1)
				g_sig_val = 0;
			else
			{
				uc = uc | (0x1 << (BIT_SHIFT_WIDTH - i));
				g_sig_val = 0;
			}
			i++;
		}
	}
	return (uc);
}

unsigned int	get_byte_length(unsigned char c)
{
	unsigned char	bit;
	unsigned char	i;
	unsigned char	uc;

	uc = (unsigned char)c;
	i = 0;
	bit = 0;
	while (i <= BIT_SHIFT_WIDTH)
	{
		bit = (uc >> (BIT_SHIFT_WIDTH - i) ) & 0x1;
		if (bit == 0)
			break ;
		i++;
	}
	return (i);
}

void	receive_message(void)
{
	unsigned char	uc;
	unsigned char	buf[4];
	unsigned int	byte_length;
	unsigned int	buf_cnt;

	uc = 0;
	buf_cnt = 0;
	g_sig_val = 0;
	byte_length = 0;
	while (1)
	{
		/* puts("waiting for signals"); */
		uc = receive_char_base();
		byte_length = get_byte_length(uc & 0xf0);
		if (byte_length == 0)
		{
			write(STDOUT_FILENO, &uc, sizeof(unsigned char));
			continue ;
		}
		else
		{
			buf[buf_cnt] = uc;
			buf_cnt++;
			while (buf_cnt < byte_length)
			{
				uc = receive_char_base();
				buf[buf_cnt] = uc;
				buf_cnt++;
			}
			write(STDOUT_FILENO, buf, sizeof(unsigned char) * (byte_length));
			buf_cnt = 0;
		}
		/* printf("byte_length: %u\n", byte_length); */
		/* if (byte_length == 0) */
		/* { */
		/* 	puts("write start"); */
		/* 	write(STDOUT_FILENO, buf, sizeof(unsigned char)); */
		/* 	puts(""); */
		/* 	puts("\nwrite end"); */
		/* 	buf_cnt = 0; */
		/* } */
		/* else if (buf_cnt == byte_length) */
		/* if (buf_cnt == byte_length) */
		/* { */
			/* puts("write start"); */
			/* write(STDOUT_FILENO, buf, sizeof(unsigned char) * (byte_length)); */
			/* puts(""); */
			/* puts("\nwrite end"); */
		/* 	buf_cnt = 0; */
		/* } */
	}
}

int	main(void)
{
	pid_t	pid;
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
	g_sig_val = 0;

	write(STDOUT_FILENO, "server\n", 7);
	pid = getpid();
	printf("pid: %d\n", pid);

	receive_message();
	exit(EXIT_SUCCESS);
}

//void	receive_char(void)
//{
//	unsigned char	uc;
//	unsigned char	i;
//	unsigned char	buf[4];
//	size_t			buf_cnt;
//
//	uc = 0;
//	i = 0;
//	buf_cnt = 0;
//	g_sig_val = 0;
//	while (1)
//	{
//		puts("waiting for signals");
//		while (i < 8)
//		{
//			pause();
//			if (g_sig_val != 0)
//			{
//				if (g_sig_val == 1)
//					g_sig_val = 0;
//				else if (g_sig_val == 2)
//				{
//					uc = uc | (0x1 << (BIT_SHIFT_WIDTH - i));
//					g_sig_val = 0;
//				}
//				printf("uc: %u, i: %u\n", uc, i);
//				i++;
//			}
//		}
//		if (i == 8)
//		{
//			buf[buf_cnt] = uc;
//			uc = 0;
//			i = 0;
//			buf_cnt++;
//		}
//		if (buf_cnt == 3)
//		{
//			puts("write start");
//			write(STDOUT_FILENO, buf, sizeof(unsigned char) * (buf_cnt + 1));
//			puts("");
//			puts("\nwrite end");
//			buf_cnt = 0;
//		}
//	}
//}
