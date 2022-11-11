#include <unistd.h>

int	main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	write(STDOUT_FILENO, "server\n", 7);
	return (0);
}
