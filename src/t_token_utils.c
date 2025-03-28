/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_token_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:48:31 by chrlomba          #+#    #+#             */
/*   Updated: 2025/03/28 12:42:04 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "t_token.h"
t_token *init_token(void)
{
    t_token    *token;
    t_parse    *parsed;
    t_doc      *doc;
    t_doc       *original_ptr;
    t_operator *operator;

    token = (t_token *)malloc(sizeof(t_token));
    if (!token)
        ft_error("Failed to allocate memory for token.");
    token->exec = true;
    token->pipe = false;
    token->env_work = false;
    token->free = false;
    token->checker = false;
    token->last_exit_status = -1;
    token->arg = NULL;

    parsed = (t_parse *)malloc(sizeof(t_parse));
    if (!parsed)
        return free(token->arg), free(token), ft_error("Failed to allocate memory for t_parse."), NULL;
    parsed->token = NULL;
    parsed->word = NULL;

    operator = (t_operator *)malloc(sizeof(t_operator));
    if (!operator)
        return free(parsed), free(token->arg), free(token), ft_error("Failed to allocate memory for t_operator."), NULL;
    original_ptr = (t_doc *)malloc(sizeof(t_doc *));
    doc = (t_doc *)malloc(sizeof(t_doc));
    if (!doc)
        return free(operator), free(parsed), free(token->arg), free(token), ft_error("Failed to allocate memory for t_doc."), NULL;
    original_ptr = doc;
    token->op = original_ptr;
    doc->eof = NULL;
    doc->here_doc = false;
    doc->next = NULL;
    operator->fd_input = -1;
    operator->fd_overwrite_output = -1;
    operator->fd_append_output = -1;
    operator->fd_append_output = -1;
    operator->operator = 0;
    token->doc = doc;
    token->parsed = parsed;
    token->operator = operator;
    token->env = NULL;
    token->next = NULL;
    return token;
}


t_token *reinit_token(t_token *prev_token)
{
    t_token    *token;
    t_parse    *parsed;
    t_doc      *doc;
    t_operator *operator;

    token = (t_token *)malloc(sizeof(t_token));
    if (!token)
        ft_error("Failed to allocate memory for token.");
    token->exec = true;
    token->pipe = false;
    token->env_work = false;
    token->free = false;
    token->checker = false;
    token->last_exit_status = -1;
  // Reserve 3 slots if needed

    parsed = (t_parse *)malloc(sizeof(t_parse));
    if (!parsed)
        return free(token->arg), free(token), ft_error("Failed to allocate memory for t_parse."), NULL;
    parsed->token = NULL;
    parsed->word = NULL;

    operator = (t_operator *)malloc(sizeof(t_operator));
    if (!operator)
        return free(parsed), free(token->arg), free(token), ft_error("Failed to allocate memory for t_operator."), NULL;
    doc = (t_doc *)malloc(sizeof(t_doc));
    if (!doc)
        return free(operator), free(parsed), free(token->arg), free(token), ft_error("Failed to allocate memory for t_doc."), NULL;
    doc->eof = NULL;
    doc->here_doc = false;
    operator->fd_input = -1;
    operator->fd_overwrite_output = -1;
    operator->fd_append_output = -1;
    operator->fd_append_output = -1;
    operator->operator = 0;
    token->doc = doc;
    token->parsed = parsed;
    token->operator = operator;
    token->env = NULL;
    token->next = NULL;
    prev_token->next = token;
    return token;
}


void	free_tokens_line(char *str, t_token *token, char *error_message)
{
	token->exec = false;
	(void)str;
	write(2, error_message, ft_strlen(error_message));
}


void	free_token(t_token *token)
{
	t_token	*tmp;
    t_doc   *doccus;

    doccus = NULL;
	while (token)
	{
		tmp = token->next;
		if (token->parsed->token)
			free(token->parsed->token);
		if (token->parsed->word)
			free(token->parsed->word);
		free(token->parsed);
		if (token->arg)
            ft_free_mtx(token->arg);
        free(token->op);
        while (token->doc)
        {
            if (token->doc->next)
                doccus = token->doc->next;
            if (token->doc->eof)
                free(token->doc->eof);
            free(token->doc);
            token->doc = doccus;
            doccus = NULL;
        }

		if (token->operator->fd_append_output > 0)
            close(token->operator->fd_append_output);
		if (token->operator->fd_overwrite_output > 0)
            close(token->operator->fd_overwrite_output);
		if (token->operator->fd_input > 0)
            close(token->operator->fd_input);
		free(token->operator);
		free(token);
		token = tmp;
	}
}

void	free_inside_token(char *msg, char *cmd)
{
	write(2, msg, ft_strlen(msg));
	write(2, cmd, ft_strlen(cmd));
	write(1, "\n", 1);
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
