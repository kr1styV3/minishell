/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:04:12 by chrlomba          #+#    #+#             */
/*   Updated: 2025/03/19 16:35:52 by chrlomba         ###   ########.fr       */
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
#define HERE_DOC_PROMT "\033[0;35mheredoc> \033[0m"

/* Function to set up redirections */
void setup_redirections(t_token *current)
{
    if (current->operator && current->operator->fd_input > 0)
    {
        if (dup2(current->operator->fd_input, STDIN_FILENO) == -1)
        {
            perror("dup2 fd_input failed");
            should_exit = 1;
        }
        close(current->operator->fd_input);
    }
    if (current->operator && current->operator->fd_overwrite_output > 0)
    {
        if (dup2(current->operator->fd_overwrite_output, STDOUT_FILENO) == -1)
        {
            perror("dup2 fd_overwrite_output failed");
            should_exit = 1;
        }
        close(current->operator->fd_overwrite_output);
    }
    if (current->operator && current->operator->fd_append_output > 0)
    {
        if (dup2(current->operator->fd_append_output, STDOUT_FILENO) == -1)
        {
            perror("dup2 fd_append_output failed");
            should_exit = 1;
        }
        close(current->operator->fd_append_output);
    }
}

/* Function to handle here_doc */
int handle_here_doc(t_token *current)
{
    int pipe_fd[2];
    char    *line;

    line = NULL;
    if (pipe(pipe_fd) == -1)
    {
        perror("pipe failed for here_doc");
        should_exit = 1;
    }
    while (1)
    {
        line = readline(HERE_DOC_PROMT);
        if (ft_strcmp(line, current->doc->eof) == 0)
            break;
        line = ft_strjoin(line, "\n");
        if (write(pipe_fd[1], line, ft_strlen(line)) == -1)
        {
            perror("write failed for here_doc");
            free(line);
            should_exit = 1;
        }
        free(line);
        line = NULL;
    }
    free(line);
    close(pipe_fd[1]); // Close write end of the pipe
    return pipe_fd[0]; // Return read end of the pipe
}

static int get_fd(t_operator *op)
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

int execute_pipeline(t_token *job_start, t_token *job_end, char **env, bool background)
{
    int pipe_fd[2];
    int prev_fd = -1;
    pid_t pid;
    pid_t child_pids[MAX_COMMANDS];
    int child_count = 0;
    int status = 0;
    t_token *current = job_start;

    while (current != job_end->next)
    {
        // Check if the current command is a builtin with output.
        bool is_builtin_output = false;
        if (current->arg && current->arg[0])
        {
            if (ft_strcmp(current->arg[0], "echo") == 0 ||
                ft_strcmp(current->arg[0], "env") == 0 ||
                ft_strcmp(current->arg[0], "export") == 0 ||
                ft_strcmp(current->arg[0], "pwd") == 0)
            {
                is_builtin_output = true;
            }
        }

        // Determine whether the current command is piped.
        bool is_piped = (current != job_end && current->operator &&
                         current->operator->operator == '|');

        // If it's a builtin with output, print its output in the main process.
        if (is_builtin_output)
        {
            int out_fd = get_fd(current->operator);
            // If the command is piped, create a pipe and write to its write end.
            if (is_piped)
            {
                if (pipe(pipe_fd) == -1)
                {
                    perror("pipe failed");
                    should_exit = 1;
                }
                out_fd = pipe_fd[1];
            }
            ft_putstr_fd(current->arg[1], out_fd);
            // If we created a pipe, close its write end and pass along the read end.
            if (is_piped)
            {
                close(pipe_fd[1]);
                prev_fd = pipe_fd[0];
            }
            else if (prev_fd != -1)
            {
                close(prev_fd);
                prev_fd = -1;
            }
            current = current->next;
            continue;
        }

        // For non-builtins, set up piping if needed.
        if (is_piped)
        {
            if (pipe(pipe_fd) == -1)
            {
                perror("pipe failed");
                should_exit = 1;
            }
        }

        pid = fork();
        if (pid == -1)
        {
            perror("fork failed");
            should_exit = 1;
        }
        else if (pid == 0)
        {
            // If there is a previous pipe, redirect STDIN from it.
            if (prev_fd != -1)
            {
                if (dup2(prev_fd, STDIN_FILENO) == -1)
                {
                    perror("dup2 prev_fd failed");
                    should_exit = 1;
                }
                close(prev_fd);
            }
            // If the current command is piped, redirect STDOUT to the pipe.
            if (is_piped)
            {
                close(pipe_fd[0]);
                if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
                {
                    perror("dup2 pipe_fd[1] failed");
                    should_exit = 1;
                }
                close(pipe_fd[1]);
            }
            // Handle here_doc if present.
            if (current->doc->here_doc)
            {
                int here_doc_fd = handle_here_doc(current);
                if (dup2(here_doc_fd, STDIN_FILENO) == -1)
                {
                    perror("dup2 here_doc_fd failed");
                    should_exit = 1;
                }
                close(here_doc_fd);
            }
            dprintf(STDERR_FILENO, "Executing command: %s\n", current->arg[0]);


            setup_redirections(current);
            execve(current->arg[0], current->arg, env);

            perror("execve failed");
            should_exit = 1;
            exit(EXIT_FAILURE);
        }
        else
        {
            child_pids[child_count++] = pid;
            if (prev_fd != -1)
                close(prev_fd);
            if (is_piped)
            {
                close(pipe_fd[1]);
                prev_fd = pipe_fd[0];
            }
            else
            {
                prev_fd = -1;
            }
        }
        current = current->next;
    }

    // Wait for all children to finish if not running in background.
    for (int i = 0; i < child_count; i++)
    {
        if (!background)
            waitpid(child_pids[i], &status, 0);
    }

    return status;
}



/* Main execution function */
int execute(t_token **token_list, char **env, t_env_list *env_list)
{
    t_token *current = *token_list;
    int     status = -1;
    (void)env_list;

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
