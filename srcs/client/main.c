#include "minitalk.h"

#define BIT_SHIFT_WIDTH 7

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len] != '\0')
		len++;
	return (len);
}

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, sizeof(char));
}

void	ft_putendl_fd(const char *s, int fd)
{
	write(fd, s, ft_strlen(s));
	ft_putchar_fd('\n', fd);
}

void	send_bit1(pid_t pid, char c)
{
	int	bit;
	int	i;
	unsigned char	uc;

	uc = (unsigned char)c;
	i = 7;
	while (0 <= i)
	{
		usleep(100);
		bit = (uc >> i) & 0x01;
		if (bit == 0)
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		i--;
	}
	puts("\n");
}

void	send_bit(pid_t pid, char c)
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
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		usleep(100);
		i++;
	}
}

int	main(int argc, char **argv)
{
	pid_t	server_pid;
	char	*string;

	if (argc != 3)
	{
		printf("usage: ./client [server PID] [strings to send]\n");
		exit(EXIT_FAILURE);
	}
	server_pid = atoi(argv[1]);
	if (kill(server_pid, 0) == -1)
	{
		perror("cannot send null signal to server");
		exit(EXIT_FAILURE);
	}
	string = argv[2];
	printf("server_pid: %d, string: %s\n", server_pid, string);
	ft_putendl_fd("sending signals", STDOUT_FILENO);

	while (*string)
	{
		send_bit(server_pid, *string);
		string++;
	}
	send_bit(server_pid, 0x04);
	exit(EXIT_SUCCESS);
}
