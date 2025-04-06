/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 19:42:57 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/06 19:44:42 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>    // For file control operations
#include <stdbool.h>  // For bool type
#include "../headers/executor.h"
#include "../headers/t_token.h" // Include the updated header
#include "../headers/env_variables.h"

static void	redirect_append_output(t_token *current)
{
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

static void	redirect_output(t_token *current)
{
	if (current->operator && current->operator->fd_overwrite_output > 0)
	{
		if (dup2(current->operator->fd_overwrite_output, STDOUT_FILENO) == -1)
		{
			perror("dup2 fd_overwrite_output failed");
			should_exit = 1;
		}
		close(current->operator->fd_overwrite_output);
	}
}

static void	redirect_input(t_token *current)
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
}

void	setup_redirections(t_token *current)
{
	redirect_input(current);
	redirect_output(current);
	redirect_append_output(current);
}
