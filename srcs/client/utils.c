#include "minitalk.h"

void	puterr_exit(char *msg)
{
	ft_putendl_fd(msg, STDERR_FILENO);
	exit(EXIT_FAILURE);
}

void	print_client_info(pid_t server_pid, char *message)
{
	ft_putstr_fd("[client] Target PID: ", STDOUT_FILENO);
	ft_putnbr_fd(server_pid, STDOUT_FILENO);
	ft_putstr_fd(", Message: ", STDOUT_FILENO);
	ft_putstr_fd(message, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
}
