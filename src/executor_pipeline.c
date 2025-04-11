/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipeline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 19:29:22 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/11 13:41:18 by chrlomba         ###   ########.fr       */
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

#define MAX_COMMANDS 1024

static int	get_fd(t_operator *op)
{
	if (op->fd_overwrite_output > 0)
		return (op->fd_overwrite_output);
	else if (op->fd_append_output > 0)
		return (op->fd_append_output);
	else if (op->fd_input > 0)
		return (op->fd_input);
	else
		return (STDOUT_FILENO);
}

// static void	wait_for_children(pid_t *pids, int count, int *status)
// {
// 	int	i;

// 	i = 0;
// 	while (i < count)
// 	{
// 		waitpid(pids[i], status, 0);
// 		i++;
// 	}
// }

static bool	is_builtin_output(t_token *current)
{
	if (current->arg && current->arg[0])
	{
		if (ft_strcmp(current->arg[0], "echo") == 0
			|| ft_strcmp(current->arg[0], "env") == 0
			|| ft_strcmp(current->arg[0], "export") == 0
			|| ft_strcmp(current->arg[0], "pwd") == 0)
			return (true);
	}
	return (false);
}

static int	run_builtin_output(t_token *current,
		bool is_piped, int *pipe_fd, int prev_fd)
{
	int	out_fd;

	out_fd = get_fd(current->operator);
	if (is_piped)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe failed");
			g_should_exit = 1;
			return (-1);
		}
		out_fd = pipe_fd[1];
	}
	ft_putstr_fd(current->arg[1], out_fd);
	if (is_piped)
	{
		close(pipe_fd[1]);
		return (pipe_fd[0]);
	}
	if (prev_fd != -1)
		close(prev_fd);
	return (-1);
}

int	execute_pipeline(t_token *job_start, t_token *job_end,
	char **env, t_env_list *envp)
	{
	t_token	*current;
	pid_t	child_pids[MAX_COMMANDS];
	int		child_count;
	int		status;
	int		out;
	int		pipe_fd[2];
	int		prev_fd;
	int i = 0;
	bool	is_piped;

	current = job_start;
	prev_fd = -1;
	child_count = 0;
	status = 0;
	out = 0;
	if (dup2(STDOUT_FILENO, out) == -1)
		exit(0);
	while (current != job_end->next)
	{
		is_piped = (current != job_end && current->operator
				&& current->operator->operator == '|');
		if (is_builtin_output(current))
		{
			prev_fd = run_builtin_output(current, is_piped, pipe_fd, prev_fd);
			current = current->next;
			continue ;
		}
		if (is_piped && pipe(pipe_fd) == -1)
		{
			perror("pipe failed");
			g_should_exit = 1;
		}
		child_pids[child_count++] = fork_and_exec(current,
				prev_fd, pipe_fd, is_piped, env, envp, out);
		if (prev_fd != -1)
			close(prev_fd);
		prev_fd = (is_piped) ? pipe_fd[0] : -1;
		current = current->next;
	}
	if (dup2(out, STDOUT_FILENO) == -1)
		exit(0);
	while (i < child_count)
	{
		waitpid(child_pids[i], &status, 0);
		i++;
	}
	return (status);
}
