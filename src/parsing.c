#include "minishell.h"

void read_line_from_user()
{
    char *command;


    command = readline("minishell> ");
    printf("the command is >%s", command);
}