/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_token_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:48:31 by chrlomba          #+#    #+#             */
/*   Updated: 2024/12/06 17:06:03 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "t_token.h"

t_token	*init_token(void)
{
	t_token	*token;
	t_parse *parse;
	t_operator	*op;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		ft_error("Failed to allocate memory for token.");
	token->exec = true;
	token->arg = ft_calloc(2, sizeof(char *));
	if (!token->arg)
	{
		free(token);
		ft_error("Failed to allocate memory for args.");
	}
	parse = (t_parse *)malloc(sizeof(t_parse));
	if (!parse)
	{
		free(token->arg);
		free(token);
		ft_error("Failed to allocate memory for t_parse.");
	}
	parse->token = NULL;
	parse->word = NULL;
	token->parsed = parse;
	op = (t_operator *)malloc (sizeof(t_operator));
	if (!op)
	{
		free(token->arg);
		free(token->parsed);
		free(token);
		ft_error("Failed to allocate memory for t_operator.");
	}
	token->operator = op;
	token->next = NULL;
	return (token);
}

t_token	*reinit_token(t_token *prev_token)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		free_tokens_line(NULL, prev_token, "Failed to allocate memory for token.");
	token->arg = ft_calloc(3, sizeof(char *));
	if (!token->arg)
	{
		free(token);
		free_tokens_line(NULL, prev_token, "Failed to allocate memory for args.");
	}
	token->parsed = (t_parse *)malloc(sizeof(t_parse));
	if (!token->parsed)
	{
		free(token->arg);
		free(token);
		free_tokens_line(NULL, prev_token, "Failed to allocate memory for t_parse.");
	}
	token->operator = (t_operator *)malloc (sizeof(t_parse));
	if (!token->operator)
	{
		free(token->arg);
		free(token->parsed);
		free(token);
		free_tokens_line(NULL, prev_token, "Failed to allocate memory for t_operator.");
	}
	prev_token->next = token;
	return (token);
}

void	free_tokens_line(char *str, t_token *token, char *error_message)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token->next;
		if (token->parsed->token)
			free(token->parsed->token);
		if (token->parsed->word)
			free(token->parsed->word);
		free(token->parsed);
		free(token->operator);
		if (token->arg)
			ft_free_mtx(token->arg);
		free(token);
		token = tmp;
	}
	if (str)
		free(str);
	ft_error(error_message);
}


void	free_token(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token->next;
		if (token->parsed->token)
			free(token->parsed->token);
		if (token->parsed->word)
			free(token->parsed->word);
		free(token->parsed);
		free(token->operator);
		if (token->arg)
			ft_free_mtx(token->arg);
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
		if (token->parsed->token)
			free(token->parsed->token);
		if (token->parsed->word)
			free(token->parsed->word);
		free(token->parsed);
		free(token->operator);
		if (token->arg)
			ft_free_mtx(token->arg);
		free(token);
		token = tmp;
	}
	token = init_token();
}

void	inside_token_free(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token->next;
		if (token->parsed->token)
			free(token->parsed->token);
		if (token->parsed->word)
			free(token->parsed->word);
		free(token->parsed);
		free(token->operator);
		if (token->arg)
			ft_free_mtx(token->arg);
		free(token);
		token = tmp;
	}
	token = init_token();
}
