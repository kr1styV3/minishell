/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr> >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:04:12 by chrlomba          #+#    #+#             */
/*   Updated: 2024/10/01 18:31:36 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/executor.h"

void execute_pipeline(t_token *job_start, t_token *job_end)
{
    int pipe_fd[2];
    int prev_fd = -1;
    pid_t pid;

    t_token *current = job_start;
    while (current != job_end->next)
    {
        if (current->next_is_pipe && current != job_end)
        {
            pipe(pipe_fd);
        }

        pid = fork();
        if (pid == 0)
        {
            // Child process

            // If there's a previous pipe, set its read-end as input
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            // If there's a next pipe, set up the write-end
            if (current->next_is_pipe && current != job_end)
            {
                close(pipe_fd[0]); // Close unused read-end
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }

            // Handle redirections
            if (current->fd_overwrite_output > 0)
            {
                dup2(current->fd_overwrite_output, STDOUT_FILENO);
                close(current->fd_overwrite_output);
            }
            if (current->fd_append_output > 0)
            {
                dup2(current->fd_append_output, STDOUT_FILENO);
                close(current->fd_append_output);
            }
            if (current->fd_input > 0)
            {
                dup2(current->fd_input, STDIN_FILENO);
                close(current->fd_input);
            }
            execve(current->args[0], current->args, NULL);
            perror("execve failed");
            exit(EXIT_FAILURE);
        }
        else
        {
            // Parent process

            if (prev_fd != -1)
                close(prev_fd); // Close previous read-end

            if (current->next_is_pipe && current != job_end)
            {
                close(pipe_fd[1]); // Close unused write-end
                prev_fd = pipe_fd[0]; // Save read-end for next command
            }
            else
            {
                prev_fd = -1;
            }

            // Wait for the child if this is the last command in a foreground job
            if (!job_end->background)
            {
                waitpid(pid, NULL, 0);
            }
        }

        current = current->next;
    }

    // If it's a foreground job, wait for all child processes
    if (!job_end->background)
    {
        while (wait(NULL) > 0);
    }
}

void execute(t_token *token_list)
{
    t_token *current = token_list;

    while (current != NULL)
    {
        // Start of a new job
        t_token *job_start = current;
        t_token *job_end = current;
        bool job_background = false;

        // Determine the end of the job
        while (job_end->next_is_pipe && job_end->next != NULL)
        {
            job_end = job_end->next;
        }

        // Check if the job should be executed in the background
        if (job_end->background)
        {
            job_background = true;
        }

        // If it's a background job, wait for previous foreground jobs
        if (job_background)
        {
            // Wait for all foreground processes
            while (waitpid(-1, NULL, WNOHANG) > 0);
        }

        pid_t pid = fork();
        if (pid == 0)
        {
            // Child process executes the pipeline
            execute_pipeline(job_start, job_end);
            exit(EXIT_SUCCESS);
        }
        else
        {
            if (!job_background)
            {
                // Parent process waits for the job to finish
                waitpid(pid, NULL, 0);
            }
            else
            {
                // Parent does not wait for background job
                printf("Started background job with PID %d\n", pid);
            }
        }

        // Move to the next command after the current job
        current = job_end->next;
    }
}

