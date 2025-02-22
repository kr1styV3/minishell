/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_token_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coca <coca@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:48:31 by chrlomba          #+#    #+#             */
/*   Updated: 2025/02/15 09:54:13 by coca             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "t_token.h"

t_token *init_token(void)
{
	t_token *token;
	t_parse *parsed;
	t_operator *operator;


	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		ft_error("Failed to allocate memory for token.");
	token->exec = true;
	token->pipe = false;
	token->arg = ft_calloc(2, sizeof(char *));
	if (!token->arg)
		return free(token), ft_error("Failed to allocate memory for args."), NULL;
	parsed = (t_parse *)malloc(sizeof(t_parse));
	if (!parsed)
		return free(token->arg), free(token), ft_error("Failed to allocate memory for t_parse."), NULL;
	parsed->token = NULL;
	parsed->word = NULL;
	operator = (t_operator *)malloc(sizeof(t_operator));
	if (!operator)
		return free(parsed), free(token->arg), free(token), ft_error("Failed to allocate memory for t_operator."), NULL;
	operator->fd_input = -1;
	operator->fd_overwrite_output = -1;
	operator->fd_append_output = -1;
	operator->operator = 0;
	token->eof = NULL;
	token->env_work = false;
	token->env_ptr = NULL;
	token->last_exit_status = -1;
	token->here_doc = false;
	token->parsed = parsed;
	token->operator = operator;
	token->next = NULL;
	return token;
}


t_token	*reinit_token(t_token *prev_token)
{
	t_token *token;

	token = (t_token *)malloc(sizeof(t_token));
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
	token->last_exit_status = prev_token->last_exit_status;
	token->here_doc = false;
	token->eof = NULL;
	prev_token->next = token;
	return (token);
}

void	free_tokens_line(char *str, t_token *token, char *error_message)
{
	token->exec = false;
	if (str)
		free(str);
	write(2, error_message, ft_strlen(error_message));
}


void	free_token(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token->next;
		if (token->parsed->token || token->parsed->token[0] == '\0')
			free(token->parsed->token);
		if (token->parsed->word)
			free(token->parsed->word);
		free(token->parsed);
		free(token->operator);
		if (token->arg)
			ft_free_mtx(token->arg);
		if (token->eof)
			free(token->eof);
		if (token->operator->fd_append_output > 0)
			close(token->operator->fd_append_output);
		if (token->operator->fd_overwrite_output > 0)
			close(token->operator->fd_overwrite_output);
		if (token->operator->fd_input > 0)
			close(token->operator->fd_input);
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
