/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:04:12 by chrlomba          #+#    #+#             */
/*   Updated: 2025/01/04 15:10:09 by chrlomba         ###   ########.fr       */
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

#define MAX_COMMANDS 1024  // Adjust as neededq

/* Function to set up redirections */
void setup_redirections(t_token *current)
{
    if (current->operator && current->operator->fd_input > 0)
    {
        if (dup2(current->operator->fd_input, STDIN_FILENO) == -1)
        {
            perror("dup2 fd_input failed");
            exit(EXIT_FAILURE);
        }
        close(current->operator->fd_input);
    }
    if (current->operator && current->operator->fd_overwrite_output > 0)
    {
        if (dup2(current->operator->fd_overwrite_output, STDOUT_FILENO) == -1)
        {
            perror("dup2 fd_overwrite_output failed");
            exit(EXIT_FAILURE);
        }
        close(current->operator->fd_overwrite_output);
    }
    if (current->operator && current->operator->fd_append_output > 0)
    {
        if (dup2(current->operator->fd_append_output, STDOUT_FILENO) == -1)
        {
            perror("dup2 fd_append_output failed");
            exit(EXIT_FAILURE);
        }
        close(current->operator->fd_append_output);
    }
}

/*
 * Function to execute a pipeline (one or more commands).
 * `job_start` is the first command in the pipeline.
 * `job_end` is the last command in the pipeline.
 * `background` indicates if the whole pipeline should run in background.
 */
int execute_pipeline(t_token *job_start, t_token *job_end, char **env, bool background)
{
    int pipe_fd[2];
    int prev_fd = -1;
    pid_t pid;
    pid_t child_pids[MAX_COMMANDS];
    int child_count = 0;
    int status = 0;

    t_token *current = job_start;

    // Iterate over commands in the pipeline
    while (current != job_end->next)
    {
        bool is_piped = (current != job_end && current->operator && current->operator->operator == '|');

        if (is_piped)
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
            // Child process
            if (prev_fd != -1)
            {
                if (dup2(prev_fd, STDIN_FILENO) == -1)
                {
                    perror("dup2 prev_fd failed");
                    exit(EXIT_FAILURE);
                }
                close(prev_fd);
            }
            if (is_piped)
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

            execve(current->arg[0], current->arg, env);
            perror("execve failed");
            should_exit = 1;
        }
        else
        {
            // Parent process
            child_pids[child_count++] = pid;
            if (prev_fd != -1)
                close(prev_fd); // Close previous read-end

            if (is_piped)
            {
                close(pipe_fd[1]);     // Close unused write-end in parent
                prev_fd = pipe_fd[0]; // Next command reads from this
            }
            else
            {
                prev_fd = -1;
            }

            // Close operator file descriptors in the parent
            if (current->operator && current->operator->fd_overwrite_output > 0)
                close(current->operator->fd_overwrite_output);
            if (current->operator && current->operator->fd_append_output > 0)
                close(current->operator->fd_append_output);
            if (current->operator && current->operator->fd_input > 0)
                close(current->operator->fd_input);
        }

        current = current->next;
    }

    // If background, do not wait for completion
    // Otherwise, wait for all children to finish
    if (!background)
    {
        for (int i = 0; i < child_count; i++)
        {
            waitpid(child_pids[i], &status, 0);
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
    return status;
}

/* Main execution function */
int execute(t_token **token_list, char **env)
{
    t_token *current = *token_list;
    int     status = 0;

    while (current != NULL)
    {
        t_token *job_start = current;
        t_token *job_end = current;

        // Build a pipeline: continue while next operator is '|'
        while (job_end->operator && job_end->operator->operator == '|' && job_end->next)
        {
            job_end = job_end->next;
        }

        // Check if job_end operator is '&' for background
        bool background = false;
        if (job_end->operator && job_end->operator->operator == '&')
        {
            background = true;
        }

        // Execute the pipeline from job_start to job_end
        status = execute_pipeline(job_start, job_end, env, background);

        // Move to the next command after job_end
        if (job_end->next)
            current = job_end->next;
        else
            current = NULL;
    }
    return status;
}
