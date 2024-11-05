/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:32:00 by chrlomba          #+#    #+#             */
/*   Updated: 2024/11/05 13:29:58 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"
#include "handler.h"
#include "promt.h"

#define OPERATORS "<>|&-"

int handle_normal(t_token *token, char *str, int pos, t_state *state, char **env)
{
	(void)env;
    if (ft_isalnum(str[pos]))
        return process_token(token, str, pos, state);
    if (ft_isbuiltin(token->token))
    {
        *state = IN_BUILTIN;
        return 0;
    }
    if (str[pos] == '"' || str[pos] == '\'') // Quote found.
    {
        *state = IN_WORD;
        return 0;
    }
    if (ft_strchr(OPERATORS, str[pos])) // Operator found.
    {
        *state = IN_OPERATOR;
        return 0;
    }
    if (str[pos] == '$' && *state != IN_BUILTIN)
    {
        *state = IN_VARIABLE;
        return 0;
    }
    return 1; // Default increment
}

void tokenizer(char *str, t_token *token, char **env)
{
	int	string_position;
	t_state	state;
	t_state_handler_map *handler;
	int	increment;

	string_position = 0;
	state = SKIP_WHITESPACE;
    handler = init_handler();
 	while (str[string_position])
    {
        handler->handler = get_handler(handler, state);
        if (handler != NULL)
        {
            increment = handler->handler(token, str, string_position, &state, env);
            string_position += increment;
        }
        if (str[string_position] == '\0')
            break ;
        if(token->token != NULL)
        {
            token->next = reinit_token(token);
            token = token->next;
        }
    }
    free(handler);
    free_tokens_line(str, token, "test");
}

void read_line_from_user(t_token *token, char **env)
{
	char	*read_line;
	char	*promt;

	promt = get_promt(env);
	read_line = readline(promt);
	free(promt);
    promt = NULL;
	add_history(read_line);
	tokenizer(read_line, token, env);
}