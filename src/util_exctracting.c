/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_exctracting.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:54:13 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/10 13:35:56 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"
#include "builtins.h"
#include "t_token.h"
#include "env_variables.h"

char	*extract_until_not_alfanum(char *str)
{
	int		i;
	int		k;
	char	*word;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == ' ' || str[i] == '_'))
		i++;
	word = ft_substr(str, 0, i);
	if (!word)
		return (NULL);
	i = 0;
	k = 0;
	while (word[i])
	{
		if (word[i] == ' ')
		{
			word[k++] = ' ';
			while (word[i] == ' ')
				i++;
		}
		else
			word[k++] = word[i++];
	}
	word[k] = '\0';
	return (word);
}

static char	*extract_literal_case(char *str, int str_pos)
{
	char	*word;

	word = ft_calloc(str_pos + 1, sizeof(char));
	if (!word)
		return (NULL);
	ft_strncpy(word, str, str_pos);
	return (word);
}

static char	*extract_env_case(char *str, int str_pos,
		t_token *token, t_bau_args *miao_bau_miao)
{
	char	*preceding;
	char	*remaining;
	char	*result;
	char	*temp;
	int		var_len;

	preceding = ft_substr(str, 0, str_pos);
	if (!preceding)
		return (NULL);
	var_len = process_variable(&token, str, str_pos + 1, miao_bau_miao);
	if (var_len == -1)
		return (free(preceding), NULL);
	str_pos += var_len + 1;
	if (token->parsed->word)
	{
		temp = ft_strjoin(preceding, token->parsed->word);
		free(preceding);
		if (!temp)
			return (NULL);
		preceding = temp;
	}
	remaining = ft_substr(str, str_pos, ft_strlen(str) - str_pos);
	if (!remaining)
		return (free(preceding), NULL);
	result = ft_strjoin(preceding, remaining);
	free(preceding);
	free(remaining);
	return (result);
}

char	*extract_word_with_dollasign(char *str, char quote,
	t_token *token, t_bau_args *cazzo_miao)
{
	int		str_pos;

	str_pos = 0;
	while (str[str_pos] && str[str_pos] != quote && str[str_pos] != '$')
		str_pos++;
	if (str[str_pos] == '$')
		return (extract_env_case(str, str_pos, token, cazzo_miao));
	if (str[str_pos] == quote)
		return (extract_literal_case(str, str_pos));
	return (NULL);
}
