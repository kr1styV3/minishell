/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:26:55 by chrlomba          #+#    #+#             */
/*   Updated: 2025/01/04 15:13:29 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"
#include "builtins.h"
#include "executor.h"
#include "read_line.h"
#include "t_token.h"
#include "../my_libft/headers/libft.h"
volatile sig_atomic_t should_exit = 0;

// Signal handler for SIGINT (Ctrl-C)
void handle_sigint(int sig)
{
	(void)sig;
	should_exit = 2;
}

// Signal handler for SIGTERM (Ctrl-D)
void handle_sigterm(int sig)
{
	(void)sig;
    should_exit = 1;  // Set flag to exit the shell
}

// Signal handler for SIGQUIT (Ctrl-\) - No action needed
void handle_sigquit(int sig)
{
	(void)sig;
    // Do nothing
}

// Function to set up signal handling
void setup_signal_handling()
{
    struct sigaction sa_int, sa_term, sa_quit;

    // Set up SIGINT handler (Ctrl-C)
    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;  // No special flags
    sigaction(SIGINT, &sa_int, NULL);

    // Set up SIGTERM handler (Ctrl-D)
    sa_term.sa_handler = handle_sigterm;
    sigemptyset(&sa_term.sa_mask);
    sa_term.sa_flags = 0;  // No special flags
    sigaction(SIGTERM, &sa_term, NULL);

    // Set up SIGQUIT handler (Ctrl-\) - Do nothing
    sa_quit.sa_handler = handle_sigquit;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;  // No special flags
    sigaction(SIGQUIT, &sa_quit, NULL);
}

int	checker(t_token **token, char **envp)
{
	char	*path;
	char	**paths;
	char	*part_path;
	int		i;

	i = 0;
	path = ft_getenv("PATH", envp);
	paths = ft_split(path, ':');
	free(path);
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, (*token)->parsed->token);
		free(part_path);
		if (access(path, X_OK) == 0)
		{
			(*token)->arg[0] = ft_strdup(path);
			ft_free_mtx(paths);
			return (0);
		}
		free(path);
		i++;
	}
	ft_free_mtx(paths);
	return (1);
}
void print_with_visible_whitespace(const char *str) {
    for (; *str; str++) {
        if (isspace((unsigned char)*str)) {
            // For example, print a dot instead of whitespace
            write(1,".", 1);
        } else {
            write(1, &(*str), 1);
        }
    }
	write(1, "\n", 1);
}
int main(int ac, char **av, char **envp)
{
	t_token		**head;
	t_token		*token;
	int status = -1;

	head = (t_token **)malloc(sizeof(t_token *));
	// setup_signal_handling();
	(void)ac;
	(void)av;
	while (!should_exit)
	{
		*head = init_token();
		token = *head;
		printf("last exitn status: %d\n", status);
		token->last_exit_status = status;
		read_line_from_user(&(*head), envp);
		if (token->exec == true || should_exit)
		{
			while (token)
			{
				if (token->parsed->token)
				{
					if (checker(&token, envp) == 1)
					{
						free_inside_token("minishell: command not found: ", token->parsed->token);
						token->exec = false;
					}
				}
				// for (int i = 0; token->arg[i]; i++)
				// 	print_with_visible_whitespace(token->arg[i]);
				token = token->next;
			}
			token = *head;
			if (token->exec == true)
				status = execute(head, envp);
		}
		if (should_exit == 2)
			should_exit = 0;
		free_token(*head);
	}
	free(head);
	rl_clear_history();
	return 0;
}
