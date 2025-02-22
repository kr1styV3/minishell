/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:26:55 by chrlomba          #+#    #+#             */
/*   Updated: 2025/01/05 22:29:28 by chrlomba         ###   ########.fr       */
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

t_mini	*t_mini_setup(void)
{
	t_mini	*mini;

	mini = (t_mini *)malloc(sizeof(t_mini));
	if (!mini)
		ft_error("Failed to allocate memory for mini.");
	mini->exec = true;
	mini->last_exit_status = -1;
	mini->env = (t_env *)malloc(sizeof(t_env));
	if (!mini->env)
		ft_error("Failed to allocate memory for env.");
	mini->env->env_ptr = NULL;
	mini->env->env_work = false;
	mini->doc = (t_doc *)malloc(sizeof(t_doc));
	if (!mini->doc)
		ft_error("Failed to allocate memory for doc.");
	mini->doc->eof = NULL;
	mini->doc->here_doc = false;
	mini->operator = (t_operator *)malloc(sizeof(t_operator));
	if (!mini->operator)
		ft_error("Failed to allocate memory for operator.");
	mini->parsed = (t_parse *)malloc(sizeof(t_parse));
	if (!mini->parsed)
		ft_error("Failed to allocate memory for parsed.");
	mini->parsed->token = NULL;
	mini->parsed->word = NULL;
	mini->token = NULL;
	return (mini);
}

int main(int ac, char **av, char **envp)
{
	t_mini		*mini;
	t_token		*token;
	char		**_env_ptr;
	int status = -1;

	_env_ptr = env_copy(envp);
	mini = t_mini_setup();
	// setup_signal_handling();
	(void)ac;
	(void)av;
	while (!should_exit)
	{
		token = init_token();
		mini->token = token;
		printf("last exitn status: %d\n", status);
		mini->last_exit_status = status;
		read_line_from_user(&mini, _env_ptr);
		if (mini->env->env_work == true)
		{
			_env_ptr = (char **)(mini->env->env_ptr);
		}
		if (mini->exec == true || should_exit)
		{
			while (token)
			{
				if (mini->parsed->token)
				{
					if (checker(&token, envp) == 1)
					{
						free_inside_token("minishell: command not found: ", mini->parsed->token);
						mini->exec = false;
					}
				}
				token = token->next;
			}
			token = mini->token;
			if (mini->exec == true)
				status = execute(mini, envp);
		}
		if (should_exit == 2)
			should_exit = 0;
		free_token(token);
	}

	rl_clear_history();
	return 0;
}
