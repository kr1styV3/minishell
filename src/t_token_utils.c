/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_token_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:48:31 by chrlomba          #+#    #+#             */
/*   Updated: 2024/12/10 20:04:40 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "t_token.h"

t_token *init_token(void)
{
    t_token *token = (t_token *)malloc(sizeof(t_token));
    if (!token)
        ft_error("Failed to allocate memory for token.");

    token->exec = true;
    token->arg = ft_calloc(2, sizeof(char *));
    if (!token->arg)
        return free(token), ft_error("Failed to allocate memory for args."), NULL;

    token->parsed = (t_parse *)malloc(sizeof(t_parse));
    if (!token->parsed)
        return free(token->arg), free(token), ft_error("Failed to allocate memory for t_parse."), NULL;

    token->parsed->token = NULL;
    token->parsed->word = NULL;

    token->operator = (t_operator *)malloc(sizeof(t_operator));
    if (!token->operator)
        return free(token->parsed), free(token->arg), free(token), ft_error("Failed to allocate memory for t_operator."), NULL;

    token->next = NULL;
    return token;
}


t_token	*reinit_token(t_token *prev_token)
{
    t_token *token = (t_token *)malloc(sizeof(t_token));
    if (!token)
        ft_error("Failed to allocate memory for token.");

    token->exec = true;
    token->arg = ft_calloc(2, sizeof(char *));
    if (!token->arg)
        return free(token), ft_error("Failed to allocate memory for args."), NULL;

    token->parsed = (t_parse *)malloc(sizeof(t_parse));
    if (!token->parsed)
        return free(token->arg), free(token), ft_error("Failed to allocate memory for t_parse."), NULL;

    token->parsed->token = NULL;
    token->parsed->word = NULL;

    token->operator = (t_operator *)malloc(sizeof(t_operator));
    if (!token->operator)
        return free(token->parsed), free(token->arg), free(token), ft_error("Failed to allocate memory for t_operator."), NULL;

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
}
