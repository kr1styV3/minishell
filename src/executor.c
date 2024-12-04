/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:04:12 by chrlomba          #+#    #+#             */
/*   Updated: 2024/11/28 13:40:17 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>    // For file control operations
#include <stdbool.h>  // For bool type
#include "../headers/executor.h"

#define MAX_COMMANDS 1024  // Adjust as needed

/* Function to set up redirections */
void setup_redirections(t_token *current)
{
	if (current->fd_input > 0)
	{
		if (dup2(current->fd_input, STDIN_FILENO) == -1)
		{
			perror("dup2 fd_input failed");
			exit(EXIT_FAILURE);
		}
		close(current->fd_input);
	}
	if (current->fd_overwrite_output > 0)
	{
		if (dup2(current->fd_overwrite_output, STDOUT_FILENO) == -1)
		{
			perror("dup2 fd_overwrite_output failed");
			exit(EXIT_FAILURE);
		}
		close(current->fd_overwrite_output);
	}
	if (current->fd_append_output > 0)
	{
		if (dup2(current->fd_append_output, STDOUT_FILENO) == -1)
		{
			perror("dup2 fd_append_output failed");
			exit(EXIT_FAILURE);
		}
		close(current->fd_append_output);
	}
}

/* Function to handle here-document (<<) */
void handle_here_doc(t_token *current)
{
	if (current->here_doc != NULL)
	{
		int pipe_fd[2];
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe failed for here_doc");
			exit(EXIT_FAILURE);
		}
		write(pipe_fd[1], current->here_doc, strlen(current->here_doc));
		close(pipe_fd[1]);
		if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		{
			perror("dup2 failed for here_doc");
			exit(EXIT_FAILURE);
		}
		close(pipe_fd[0]);
	}
}

/* Function to execute a pipeline of commands */
void execute_pipeline(t_token *job_start, t_token *job_end, char **env)
{
	int pipe_fd[2];
	int prev_fd = -1;
	pid_t pid;
	pid_t child_pids[MAX_COMMANDS];
	int child_count = 0;

	t_token *current = job_start;
	while (current != job_end->next)
	{
		if (current->next_is_pipe && current != job_end)
		{
			if (pipe(pipe_fd) == -1)
			{
				perror("pipe failed");
				exit(EXIT_FAILURE);
			}
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork failed");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			if (prev_fd != -1)
			{
				if (dup2(prev_fd, STDIN_FILENO) == -1)
				{
					perror("dup2 prev_fd failed");
					exit(EXIT_FAILURE);
				}
				close(prev_fd);
			}
			if (current->next_is_pipe && current != job_end)
			{
				close(pipe_fd[0]);
				if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
				{
					perror("dup2 pipe_fd[1] failed");
					exit(EXIT_FAILURE);
				}
				close(pipe_fd[1]);
			}
			setup_redirections(current);
			handle_here_doc(current);
			execve(current->args[0], current->args, env);
			perror("execve failed");
			exit(EXIT_FAILURE);
		}
		else
		{
			child_pids[child_count++] = pid;
			if (prev_fd != -1)
				close(prev_fd); // Close previous read-end
			if (current->next_is_pipe && current != job_end)
			{
				close(pipe_fd[1]);     // Close unused write-end
				prev_fd = pipe_fd[0];   // Save read-end for next command
			}
			else
			{
				prev_fd = -1;
			}
			if (current->fd_overwrite_output > 0)
				close(current->fd_overwrite_output);
			if (current->fd_append_output > 0)
				close(current->fd_append_output);
			if (current->fd_input > 0)
				close(current->fd_input);
		}
		current = current->next;
	}
	if (!job_end->background)
	{
		for (int i = 0; i < child_count; i++)
		{
			waitpid(child_pids[i], NULL, 0);
		}
	}
	else
	{
		printf("Started background job with PIDs:");
		for (int i = 0; i < child_count; i++)
		{
			printf(" %d", child_pids[i]);
		}
		printf("\n");
	}
}

/* Main execution function */
void execute(t_token *token_list, char **env)
{
	t_token *current = token_list;

	while (current != NULL)
	{
		t_token *job_start = current;
		t_token *job_end = current;

		while (job_end->next_is_pipe && job_end->next != NULL)
		{
			job_end = job_end->next;
		}
		execute_pipeline(job_start, job_end, env);
		current = job_end->next;
	}
}
