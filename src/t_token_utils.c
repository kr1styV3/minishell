/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_token_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr> >        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:48:31 by chrlomba          #+#    #+#             */
/*   Updated: 2024/10/01 18:26:05 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

t_token	*init_token(void)
{
	t_token *token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		ft_error("Failed to allocate memory for token.");
	token->token = NULL;
	token->word = NULL;
	token->fd_overwrite_output = 0;
	token->fd_append_output = 0;
	token->fd_input = 0;
	token->background = false;
	token->here_doc = NULL;
	token->next_is_pipe = false;
	token->next = NULL;
	token->prev = NULL;
	token->args = ft_calloc(2, sizeof(char *));
	return (token);
}

t_token	*reinit_token(t_token *prev_token)
{
	t_token *token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		ft_error("Failed to allocate memory for token.");
	token->token = NULL;
	token->word = NULL;
	token->fd_overwrite_output = 0;
	token->fd_append_output = 0;
	token->fd_input = 0;
	token->background = false;
	token->here_doc = NULL;
	token->next_is_pipe = false;
	token->next = NULL;
	token->prev = prev_token;
	token->args = ft_calloc(2, sizeof(char *));
	return (token);
}

void	free_tokens_line(char *str, t_token *token, char *error_message)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token->next;
		if (token->token)
			free(token->token);
		if (token->word)
			free(token->word);
		if (token->here_doc)
			free(token->here_doc);
		if (token->args)
			ft_free_mtx(token->args);
		free(token);
		token = tmp;
	}
	if (str)
		free(str);
	ft_error(error_message);
}

void return_to_head(t_token *token)
{
	while (token->prev != NULL)
	{
		token = token->prev;
	}
}

void	print_tokens(t_token *token)
{
	t_token	*tmp;

	tmp = token;
	while (tmp)
	{
		if (tmp->word)
			printf("Word: %s\n", tmp->word);
		if (tmp->here_doc)
			printf("Here_doc: %s\n", tmp->here_doc);
		if (tmp->operator == '|')
			printf("Operator: %c\n", tmp->operator);
		if (tmp->args)
		{
			for (int i = 0; tmp->args[i]; i++)
			{
				if (tmp->args[i])
					printf("Args[%d]: %s\n", i, tmp->args[i]);
			}
		}
		tmp = tmp->next;
	}
}

void	free_token(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token->next;
		if (token->token)
			free(token->token);
		if (token->word)
			free(token->word);
		if (token->here_doc)
			free(token->here_doc);
		if (token->args)
			ft_free_mtx(token->args);
		free(token);
		token = tmp;
	}
}

void	free_inside_token(t_token *token, char *msg, char *cmd)
{
	t_token	*tmp;

	write(2, msg, ft_strlen(msg));
	write(2, cmd, ft_strlen(cmd));
	write(1, "\n", 1);
	while (token)
	{
		tmp = token->next;
		if (token->token)
			free(token->token);
		if (token->word)
			free(token->word);
		if (token->here_doc)
			free(token->here_doc);
		if (token->args)
			ft_free_mtx(token->args);
		free(token);
		token = tmp;
	}
	token = init_token();
}
