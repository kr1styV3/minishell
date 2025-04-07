/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_exec_capture.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 20:10:31 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/07 20:09:18 by chrlomba         ###   ########.fr       */
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

static char	*safe_strdup(char *src)
{
	if (src)
		return (ft_strdup(src));
	return (ft_strdup(""));
}

static char	*read_pipe_output(int pipe_read_fd)
{
	char	*line;
	char	*result;
	char	*tmp;

	result = NULL;
	while (1)
	{
		line = get_next_line(pipe_read_fd);
		if (!line)
			break ;
		tmp = safe_strdup(result);
		free(result);
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

	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
	token = init_token();
	tokenizer(command, token, env, envp);
	if (token->checker)
	{
		if (checker(&token, env) == 1)
		{
			ft_putstr_fd("invalid command expansion : ", 2);
			ft_putstr_fd(token->parsed->token, 2);
			free_token(token);
			g_should_exit = 1;
			exit(EXIT_FAILURE);
		}
	}
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
	close(pipefd[0]);
	waitpid(pid, &status, 0);
	return (output);
}
