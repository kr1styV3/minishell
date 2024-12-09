/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:24:50 by chrlomba          #+#    #+#             */
/*   Updated: 2024/12/06 17:18:13 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "env_variables.h"
#include "promt.h"
#include "read_line.h"
#include "t_token.h"

#define DELIMITERS " \t\r\n\a"

#define OPERATORS "<>|&-"

#define CHECKWORDARG " "

int	ft_checkwordarg(t_token **token, char *str, int i)
{
	int		len = 0;
	char	quote;

	if (ft_isbuiltin((*token)->parsed->token))
		return (0);
	if (!ft_strchr(CHECKWORDARG, str[i]) && str[i] != '\0')
	{
		while (!ft_strchr(CHECKWORDARG, str[i + len]) && str[i + len] != 0)
		{
			len++;
		}
		(*token)->arg = ft_recalloc((*token)->arg, 2 * sizeof(char *), 3 * sizeof(char *));
		(*token)->arg[1] = ft_substr(str, i, len);
	}
	if (str[i] == '\"' || str[i] == '\'')
	{
		len = 1;
		quote = str[i];
		while (str[i + len] != quote)
			len++;
		(*token)->arg = ft_recalloc((*token)->arg, 2 * sizeof(char *), 3 * sizeof(char *));
		(*token)->arg[1] = ft_substr(str, i + 1, len -1);
		len++;
	}
	printf("what i parsed is : %s \n", (*token)->arg[1]);
	return (len);
}

void	tokenizer(char *str, t_token *token, char **env)
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
				string_position += process_token(&token, str, string_position, &state);
			string_position += ft_checkwordarg(&token, str, string_position);
			if (ft_isbuiltin(token->parsed->token))
				state = IN_BUILTIN;
			if (str[string_position] == 34 || str[string_position] == 39)  // Quote found.
				state = IN_WORD;
			if (ft_strchr(OPERATORS, str[string_position]))  // Operator found.
				state = IN_OPERATOR;
			if (str[string_position] == '$' && !IN_BUILTIN)
				state = IN_VARIABLE;
		}
		if (state == IN_BUILTIN)
			string_position += process_builtin(&token, str, string_position, &state, env);
		if (state == IN_VARIABLE)
			string_position += process_variable(&token, str, string_position + 1, env) + 1;
		if (state == IN_WORD)
			string_position += process_word(&token, str, string_position, &state, env);
		if (state == IN_OPERATOR || ft_strchr(OPERATORS, str[string_position]))
			string_position += process_operator(&token, str, string_position, &state);
		if (ft_strchr(OPERATORS, str[string_position]))
			string_position += process_operator(&token, str, string_position, &state);
		if (str[string_position] == '\0' && state == FREE_TOKEN)
		{
			token->exec = false;
			return ;
		}
		else if (str[string_position] == '\0')
		{
			return ;
		}
		else
		{
			token->next = reinit_token(token);
			token = token->next;
		}
	}
}

void read_line_from_user(t_token **token, char **env)
{
	char	*read_line;
	char	*promt;

	promt = get_promt(env);
	read_line = readline(promt);
	free(promt);
	add_history(read_line);
	tokenizer(read_line, *token, env);
	free(read_line);
}
