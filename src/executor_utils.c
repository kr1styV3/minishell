/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 19:30:26 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/07 20:09:00 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>    // For file control operations
#include <stdbool.h>  // For bool type
#include "../headers/executor.h"
#include "../headers/t_token.h" // Include the updated header
#include "../headers/env_variables.h"

static void	setup_child_io(t_token *current, int prev_fd, int *pipe_fd,
	bool is_piped, char **env, t_env_list *envp)
{
	int	here_doc_fd;

	if (current->doc && current->doc->here_doc)
	{
		here_doc_fd = handle_here_docs(current, envp, env);
		if (here_doc_fd == -1 || dup2(here_doc_fd, STDIN_FILENO) == -1)
		{
			perror("here_doc setup failed");
			g_should_exit = 1;
			exit(139);
		}
		close(here_doc_fd);
	}
	else if (prev_fd != -1)
	{
		if (dup2(prev_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 prev_fd failed");
			g_should_exit = 1;
			exit(1);
		}
		close(prev_fd);
	}
	if (is_piped)
	{
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 pipe_fd[1] failed");
			g_should_exit = 1;
			exit(1);
		}
		close(pipe_fd[1]);
	}
}

pid_t	fork_and_exec(t_token *current, int prev_fd, int *pipe_fd,
	bool is_piped, char **env, t_env_list *envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		g_should_exit = 1;
		return (-1);
	}
	if (pid == 0)
	{
		setup_child_io(current, prev_fd, pipe_fd, is_piped, env, envp);
		setup_redirections(current);
		if (current->arg)
			execve(current->arg[0], current->arg, env);
		g_should_exit = 1;
		exit(1);
	}
	return (pid);
}
