/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:44:47 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/07 20:08:43 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env_variables.h"

static void	read_and_discard_input(char *eof)
{
	char	*input;

	while (1)
	{
		input = readline(">");
		if (!input)
			break ;
		if (ft_strcmp(input, eof) == 0)
		{
			free(input);
			break ;
		}
		free(input);
	}
}

static int	init_heredoc_pipe(int *pipe_fd)
{
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe failed for here_doc");
		g_should_exit = 1;
		return (-1);
	}
	return (0);
}

static int	write_heredoc_lines(int write_fd, t_token *token,
		t_env_list *env_list, char **envp)
{
	char	*input;
	char	*line;

	while (1)
	{
		input = readline(">");
		if (!input)
			break ;
		if (ft_strcmp(input, token->doc->eof) == 0)
		{
			free(input);
			break ;
		}
		line = ft_freejoin(input, "\n");
		if (heredoc_expansion(&token, line, env_list, envp))
			line = ft_strdup(token->parsed->word);
		if (write(write_fd, line, ft_strlen(line)) == -1)
		{
			perror("write failed for here_doc");
			free(line);
			return (-1);
		}
		free(line);
	}
	return (0);
}

static int	write_last_heredoc(t_token **token,
		t_env_list *env_list, char **envp)
{
	int		pipe_fd[2];
	int		result;

	if (init_heredoc_pipe(pipe_fd) == -1)
		return (-1);
	result = write_heredoc_lines(pipe_fd[1], *token, env_list, envp);
	close(pipe_fd[1]);
	if (result == -1)
	{
		close(pipe_fd[0]);
		g_should_exit = 1;
		return (-1);
	}
	return (pipe_fd[0]);
}

int	handle_here_docs(t_token *current, t_env_list *env, char **envp)
{
	t_doc	*doc;
	int		final_fd;

	doc = (*current->op);
	final_fd = -1;
	while (doc)
	{
		if (doc->next)
			read_and_discard_input(doc->eof);
		else
		{
			current->doc = doc;
			final_fd = write_last_heredoc(&current, env, envp);
			if (final_fd == -1)
			{
				perror("here_doc error");
				g_should_exit = 1;
			}
		}
		doc = doc->next;
	}
	return (final_fd);
}
