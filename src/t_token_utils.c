/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_token_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:48:31 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/06 20:53:10 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "t_token.h"
// t_token *init_token(void)
// {
//     t_token    *token;
//     t_parse    *parsed;
//     t_doc      *doc;
//     t_doc       **original_ptr;
//     t_operator *operator;

//     token = (t_token *)malloc(sizeof(t_token));
//     if (!token)
//         ft_error("Failed to allocate memory for token.");
//     token->exec = true;
//     token->env_work = false;
//     token->free = false;
//     token->checker = false;
//     token->last_exit_status = -1;
//     token->arg = NULL;

//     parsed = (t_parse *)malloc(sizeof(t_parse));
//     if (!parsed)
//         return fred to allocate memory for t_parse."), NULL;
//     parsed->token = NULL;
//     parsed->word = NULL;

//     operator = (t_operator *)malloc(sizeof(t_operator));
//     if (!operator)
//    ), ft_error("Failed to allocate memory for t_operator."), NULL;
//     original_ptr = (t_doc **)malloc(sizeof(t_doc *));
//     doc = (t_doc *)malloc(sizeof(t_doc));
//     if (!doc)
//         return free(operator), free(par
//     token->op = original_ptr;
//     doc->eof = NULL;
//     doc->here_doc = false;
//     doc->next = NULL;
//     operator->fd_input = -1;
//     operator->fd_overwrite_output = -1;
//     operator->fd_append_output = -1;
//     operator->fd_append_output = -1;
//     operator->operator = 0;
//     token->doc = doc;
//     token->parsed = parsed;
//     token->operator = operator;
//     token->env = NULL;
//     token->next = NULL;
//     return token;
// }

void	free_tokens_line(char *str, t_token *token, char *error_message)
{
	token->exec = false;
	(void)str;
	write(2, error_message, ft_strlen(error_message));
}

void	free_inside_token(char *msg, char *cmd)
{
	write(2, msg, ft_strlen(msg));
	write(2, cmd, ft_strlen(cmd));
	write(1, "\n", 1);
}
