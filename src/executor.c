/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:04:12 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/11 18:47:44 by chrlomba         ###   ########.fr       */
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

#define HERE_DOC_PROMT "\033[0;34mheredoc> \033[0m"
#define PIPE_DOC_PROMT "\033[0;36mpipe heredoc> \033[0m"

/* Function to set up redirections */
// void setup_redirections(t_token *current)
// {
//     if (current->operator && current->operator->fd_input > 0)
//     {
//         if (dup2(current->operator->fd_input, STDIN_FILENO) == -1)
//         {
//             perror("dup2 fd_input failed");
//             g_g_should_exit = 1;
//         }
//         close(current->operator->fd_input);
//     }
//     if (current->operator && current->operator->fd_overwrite_output > 0)
//     {
//         if (dup2(current->operator->fd_overwr
//             perror("dup2 fd_overwrite_output failed");
//             g_should_exit = 1;
//         }
//         close(current->operator->fd_overwrite_output);
//     }
//     if (current->operator && current->operator->fd_append_output > 0)
//     {
//         if (dup2(current->operator->fd_append_output, STDOUT_FILENO) == -1)
//         {
//             perror("dup2 fd_append_output failed");
//             g_should_exit = 1;
//         }
//         close(current->operator->fd_append_output);
//     }
// }

// void    pipe_here_doc()

// int execute_pipeline(t_token *job_start,
// {
//     int pipe_fd[2];
//     int prev_fd = -1;
//     pid_t pid;
//     pid_t child_pids[MAX_COMMANDS];
//     int child_count = 0;
//     int status = 0;
//     t_token *current = job_start;

//     while (current != job_end->next)
//     {
//         // Check if the current command is a builtin with output.
//         bool is_builtin_output = false;
//         if (current->arg && current->arg[0])
//         {
//             if (ft_strcmp(current->arg[0], "echo") == 0 ||
//                 ft_strcmp(current->arg[0], "env") == 0 ||
//                 ft_strcmp(current->arg[0], "export") == 0 ||
//                 ft_strcmp(current->arg[0], "pwd") == 0)
//             {
//                 is_builtin_output = true;
//             }
//         }

//         // Determine whether the current command is piped.
//         bool is_piped = (current != job_end && current->operator &&
//                          current->operator->operator == '|');

//         // If it's a builtin with output, p
//         {
//             int out_fd = get_fd(current->operator);
//             // If
//             if (is_piped)
//             {
//                 if (pipe(pipe_fd) == -1)
//                 {
//                     perror("pipe failed");
//                     g_should_exit = 1;
//                 }
//                 out_fd = pipe_fd[1];
//             }
//             // If we created a pipe, cld.
//             if (is_piped)
//             {
//                 close(pipe_fd[1]);
//                 prev_fd = pipe_fd[0];
//             }
//             else if (prev_fd != -1)
//             {
//                 close(prev_fd);
//                 prev_fd = -1;
//             }
//             current = current->next;
//             continue;
//         }

//         // For non-builtins, set up piping if needed.
//         if (is_piped)
//         {
//             if (pipe(pipe_fd) == -1)
//             {
//                 perror("pipe failed");
//                 g_should_exit = 1;
//             }
//         }

//         pid = fork();
//         if (pid == -1)
//         {
//             perror("fork failed");
//             g_should_exit = 1;
//         }
//         else if (pid == 0)
//         {
//             if (current->doc && current->doc->here_doc)
//             {
//                 int here_doc_fd = handle_here_docs(current, envp, env);
//                 if (here_doc_fd == -1)
//                 {
//                     g_should_exit = 1;
//                     exit(139);
//                 }
//                 if (dup2(here_doc_fd, STDIN_FILENO) == -1)
//                 {
//                     perror("dup2 failed for here_doc_fd");
//                     close(here_doc_fd);
//                     g_should_exit = 1;
//                     exit(139);
//                 }
//                 close(here_doc_fd);
//             }
//             // Otherwise, if there’s a previous pipe, use it for STDIN.
//             else if (prev_fd != -1)
//             {
//                 if (dup2(prev_fd, STDIN_FILENO) == -1)
//                 {
//                     perror("dup2 prev_fd failed");
//                     g_should_exit = 1;
//                     exit(1);
//                 }
//                 close(prev_fd);
//             }

//             // --- Setup STDOUT ---
//             {
//                 close(pipe_fd[0]); // Close unused read end.
//                 if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
//                 {
//                     perror("dup2 pipe_fd[1] failed");
//                     g_should_exit = 1;
//                     exit(1);
//                 }
//                 close(pipe_fd[1]);
//             }

//             // Execute the command.
//             if (current->arg)
//             {
//                 setup_redirections(current);
//                 execve(current->arg[0], current->arg, env);
//             }

//             g_should_exit = 1;
//         }
//         else
//         {
//             child_pids[child_count++] = pid;
//             if (prev_fd != -1)
//                 close(prev_fd);
//             if (is_piped)
//             {
//                 close(pipe_fd[1]);
//                 prev_fd = pipe_fd[0];
//             }
//             else
//             {
//                 prev_fd = -1;
//             }
//         }
//         current = current->next;
//     }

//     // Wait for all children to finish if not running in background.
//     for (int i = 0; i < child_count; i++)
//     {
//         if (!background)
//             waitpid(child_pids[i], &status, 0);
//     }

//     return status;
// }

static t_token	*get_job_end(t_token *start)
{
	t_token	*job_end;

	job_end = start;
	while (job_end->operator
		&& job_end->operator->operator == '|'
		&& job_end->next)
		job_end = job_end->next;
	return (job_end);
}

/* Main execution function */
int	execute(t_token **token_list, char **env, t_env_list *env_list)
{
	t_token	*current;
	t_token	*job_start;
	t_token	*job_end;
	int		status;

	current = *token_list;
	status = -1;
	while (current != NULL)
	{
		job_start = current;
		job_end = get_job_end(current);
		status = execute_pipeline(job_start, job_end,
				env, env_list);
		if (job_end->next)
			current = job_end->next;
		else
			current = NULL;
	}
	return (status);
}
