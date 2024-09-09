/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_token_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:48:31 by chrlomba          #+#    #+#             */
/*   Updated: 2024/09/09 05:31:11 by chrlomba         ###   ########.fr       */
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
		free(token);
		token = tmp;
	}
	if (str)
		free(str);
	ft_error(error_message);
}

void	print_tokens(t_token *token)
{
	t_token	*tmp;

	tmp = token;
	while (tmp)
	{
		if (tmp->token)
			printf("Token: %s\n", tmp->token);
		if (tmp->word)
			printf("Word: %s\n", tmp->word);
		if (tmp->here_doc)
			printf("Here_doc: %s\n", tmp->here_doc);
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
		free(token);
		token = tmp;
	}
}
