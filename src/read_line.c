/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:24:50 by chrlomba          #+#    #+#             */
/*   Updated: 2024/11/05 14:25:29 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void tokenizer(char *str, t_token *token, char **env)
{
	int string_position;
	t_state state;

	string_position = 0;
	state = SKIP_WHITESPACE;
	while (str[string_position])
	{
		if (state == SKIP_WHITESPACE)
			string_position += skip_whitespaces(&str[string_position], &state);
		if (state == NORMAL)
		{
			if (ft_isalnum(str[string_position]))
				string_position += process_token(token, str, string_position, &state);
			if (ft_isbuiltin(token->token))
				state = IN_BUILTIN;
			if (str[string_position] == 34 || str[string_position] == 39)  // Quote found.
				state = IN_WORD;
			if (ft_strchr(OPERATORS, str[string_position]))  // Operator found.
				state = IN_OPERATOR;
			if (str[string_position] == '$' && !IN_BUILTIN)
				state = IN_VARIABLE;
		}
		if (state == IN_BUILTIN)
			string_position += process_builtin(token, str, string_position, &state, env);
		if (state == IN_VARIABLE)
			string_position += process_variable(token, str, string_position + 1, env) + 1;
		if (state == IN_WORD)
			string_position += process_word(token, str, string_position, &state);
		if (state == IN_OPERATOR)
			string_position += process_operator(token, str, string_position, &state);
		if (str[string_position] == '\0')
			return ;
		token->next = reinit_token(token);
		token = token->next;
	}
}

void read_line_from_user(t_token *token, char **env)
{
	char	*read_line;
	char	*promt;

	promt = get_promt(env);
	read_line = readline(promt);
	free(promt);
	add_history(read_line);
	tokenizer(read_line, token, env);
}
