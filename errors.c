#include <stdio.h>
#include <stdlib.h>

void	terminate(char *error_message)
{
	perror(error_message);
	exit(1);
}
