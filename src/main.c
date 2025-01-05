/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:26:55 by chrlomba          #+#    #+#             */
/*   Updated: 2025/01/05 19:17:05 by chrlomba         ###   ########.fr       */
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

void handle_sigint(int sig)
{
	(void)sig;
	should_exit = 2;
}

void handle_sigterm(int sig)
{
	(void)sig;
	should_exit = 1;
}

void handle_sigquit(int sig)
{
	(void)sig;
}

void setup_signal_handling()
{
	struct sigaction sa_int;
	struct sigaction sa_term;
	struct sigaction sa_quit;

	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sa_term.sa_handler = handle_sigterm;
	sigemptyset(&sa_term.sa_mask);
	sa_term.sa_flags = 0;
	sigaction(SIGTERM, &sa_term, NULL);
	sa_quit.sa_handler = handle_sigquit;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
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

char	**env_copy(char **_env)
{
	char	**env;
	int		i;

	i = 0;
	while (_env[i])
		i++;
	env = (char **)malloc(sizeof(char *) * (i + 1));
	if (!env)
		ft_error("Failed to allocate memory for env.");
	i = 0;
	while (_env[i])
	{
		env[i] = ft_strdup(_env[i]);
		i++;
	}
	env[i] = NULL;
	return (env);
}

int main(int ac, char **av, char **envp)
{
	t_token		**head;
	t_token		*token;
	char		**_env_ptr;
	int status = -1;

	_env_ptr = env_copy(envp);
	head = (t_token **)malloc(sizeof(t_token *));
	if (!head)
		ft_error("Failed to allocate memory for token.");
	// setup_signal_handling();
	(void)ac;
	(void)av;
	while (!should_exit)
	{
		*head = init_token();
		token = *head;
		printf("last exitn status: %d\n", status);
		token->last_exit_status = status;
		read_line_from_user(&(*head), _env_ptr);
		if ((*head)->env_work == true)
		{
			_env_ptr = (char **)((*head)->env_ptr);
		}
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
