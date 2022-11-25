#include "minitalk.h"

void	print_server_info(pid_t server_pid)
{
	ft_putstr_fd("[server] PID: ", STDOUT_FILENO);
	ft_putnbr_fd(server_pid, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
}

void	puterr_exit(char *msg)
{
	ft_putendl_fd(msg, STDERR_FILENO);
	exit(EXIT_FAILURE);
}
