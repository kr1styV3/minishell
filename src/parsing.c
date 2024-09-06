#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

#define DELIMITERS " \t\r\n\a"


void read_line_from_user(t_token *token)
{
	char	*read_line;

	read_line = readline("Minishell > ");
	if (!read_line)
		ft_error("EOF");//not error but end of readind ctrl+d

}
