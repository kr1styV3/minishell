/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 15:14:48 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/07 20:28:05 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"
#include "builtins.h"
#include "t_token.h"

int	skip_whitespaces(char *str, t_state *state)
{
	int	string_position;

	string_position = 0;
	if (!str || str[0] == '\0')
		return (0);
	while (ft_isspace(str[string_position]))
		string_position++;
	if (state)
		*state = NORMAL;
	return (string_position);
}

int	process_token(t_token **token,
		char *str, int string_position, t_state *state)
{
	int	word_len;

	(void)state;
	(*token)->parsed->token = extract_token(&str[string_position]);
	if (!(*token)->parsed->token)
		return (free_tokens_line(str, *token, "memory allocation"), -1);
	word_len = ft_strlen((*token)->parsed->token);
	word_len += skip_whitespaces(&str[string_position + word_len], NULL);
	(*token)->checker = true;
	return (word_len);
}

int	process_word(t_token **token, char *str,
		int string_position, t_env_list *env)
{
	char	quote;
	int		len;
	int		j;

	len = 0;
	quote = str[string_position];
	(*token)->parsed->word = extract_word(&str[string_position + 1], quote);
	if (!(*token)->parsed->word)
		return (free_tokens_line(str, *token, "memory allocation"), -1);
	if ((*token)->parsed->token)
	{
		(*token)->parsed->token = ft_freejoin((*token)->parsed->token,
				(*token)->parsed->word);
		len = ft_strlen((*token)->parsed->word) + 2;
		free((*token)->parsed->word);
		(*token)->parsed->word = NULL;
	}
	if (ft_isbuiltin((*token)->parsed->token))
	{
		j = 0;
		while (str[string_position + 1 + j] != ' ')
			j++;
		j = process_builtin(token, str, string_position + 1 + j, NULL, env, NULL);
	}
	return (len);
}
