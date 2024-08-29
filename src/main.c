#include "minishell.h"

int main(int ac, char **av, char **envp)
{
    (void)envp;
    (void)ac;
    (void)av;
    read_line_from_user();
}