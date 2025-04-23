/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_exec_capture.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 20:10:31 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/23 13:41:48 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/parsing.h"
#include "../headers/builtins.h"
#include "../headers/executor.h"
#include "../my_libft/headers/get_next_line.h"
#include "../my_libft/headers/libft.h"
#include "../headers/read_line.h"
#include "env_variables.h"

static void	handle_fork_error(int *pipefd)
{
	close(pipefd[0]);
	close(pipefd[1]);
}

static char	*read_pipe_output(int pipe_read_fd)
{
	char	*line;
	char	*result;
	char	*tmp;

	result = NULL;
	tmp = ft_strdup("");
	while (1)
	{
		line = get_next_line(pipe_read_fd);
		if (!line)
			break ;
		if (result)
		{
			tmp = ft_strdup(result);
			if (!tmp)
				return (free(result), NULL);
			free(result);
		}
		result = ft_strjoin_until_nl(tmp, line);
		free(tmp);
		free(line);
	}
	return (result);
}

static void	handle_child_execution(int *pipefd, char *command,
	t_env_list *env, char **envp)
{
	t_token	*token;
	t_token	*start;

	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
	token = init_token();
	start = token;
	tokenizer(command, token, env, envp);
	while (token)
	{
		if (token->checker == true)
		{
			if (checker(&token, env) == 1)
			{
				free_inside_token("minishell: command not found: ",
					token->parsed->token);
				g_should_exit = 2;
			}
		}
		token = token->next;
	}
	token = start;
	execute(&token, envp, env);
	free_token(token);
	exit(EXIT_SUCCESS);
}

char	*execute_and_capture_output(char *command,
	t_env_list *env, char **envp)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;
	char	*output;

	if (pipe(pipefd) == -1)
		return (NULL);
	pid = fork();
	if (pid == -1)
	{
		handle_fork_error(pipefd);
		return (NULL);
	}
	else if (pid == 0)
		handle_child_execution(pipefd, command, env, envp);
	close(pipefd[1]);
	output = read_pipe_output(pipefd[0]);
	if (!output)
		return (NULL);
	close(pipefd[0]);
	waitpid(pid, &status, 0);
	return (output);
}
