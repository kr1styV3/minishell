#include "minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define YELLOW "\x1b[33m"
#define RED "\x1b[31m"
#define RESET "\x1b[0m"

int main(int ac, char **av, char **envp)
{
	t_token		*token;

	token = init_token();
	(void)envp;
	(void)ac;
	(void)av;
	while (1)
	{
		read_line_from_user(token);
		//executor();
	}
	return 0;
}
