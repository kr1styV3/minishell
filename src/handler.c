/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:42:03 by chrlomba          #+#    #+#             */
/*   Updated: 2024/11/05 13:29:03 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/handler.h"
#include "../headers/parsing.h"
#include "../headers/builtins.h"
#include "../headers/env_variables.h"


int handle_skip_whitespace(t_token *token, char *str, int pos, t_state *state, char **env)
{
    (void)token;
    (void)env;
    return skip_whitespaces(&str[pos], state);
}

int handle_in_builtin(t_token *token, char *str, int pos, t_state *state, char **env)
{
    return process_builtin(token, str, pos, state, env);
}

int handle_in_variable(t_token *token, char *str, int pos, t_state *state, char **env)
{
    (void)state;
    return process_variable(token, str, pos + 1, env) + 1;
}

int handle_in_word(t_token *token, char *str, int pos, t_state *state, char **env)
{
    (void)env;
    return process_word(token, str, pos, state);
}

int handle_in_operator(t_token *token, char *str, int pos, t_state *state, char **env)
{
    (void)env;
    return process_operator(token, str, pos, state);
}
