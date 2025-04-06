/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_extracting.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 19:09:42 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/06 19:13:40 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"
#include "builtins.h"
#include "t_token.h"

static char	*extract_quoted_token(char *str)
{
	int		start;
	int		i;
	char	*token;

	i = 1;
	start = i;
	while (str[i] && str[i] != '\"')
		i++;
	token = ft_calloc(i - start + 1, sizeof(char));
	if (!token)
		return (NULL);
	ft_strncpy(token, &str[start], i - start);
	return (token);
}

static char	*extract_unquoted_token(char *str)
{
	int		i;
	char	*token;

	i = 0;
	while (str[i] && (ft_isalnum(str[i])
			|| str[i] == '.' || str[i] == '_'
			|| str[i] == '-'))
		i++;
	token = ft_calloc(i + 1, sizeof(char));
	if (!token)
		return (NULL);
	ft_strncpy(token, str, i);
	return (token);
}

char	*extract_file_token(char *str)
{
	if (!str)
		return (NULL);
	if (str[0] == '\"')
		return (extract_quoted_token(str));
	return (extract_unquoted_token(str));
}

char	*extract_token(char *str)
{
	int		string_position;
	char	*token;

	string_position = 0;
	while (str[string_position]
		&& (ft_isalnum(str[string_position]) || str[string_position] == '-'
			|| str[string_position] == '_' || str[string_position] == '.'))
		string_position++;
	token = ft_calloc(string_position + 1, sizeof(char));
	if (!token)
		return (NULL);
	ft_strncpy(token, str, string_position);
	return (token);
}

char	*extract_word(char *str, char quote)
{
	int		string_position;
	char	*word;
	bool	closed_quote;

	string_position = 0;
	closed_quote = false;
	while (str[string_position] && str[string_position] != quote)
		string_position++;
	if (str[string_position] == quote)
		closed_quote = true;
	if (!closed_quote)
		return (NULL);
	word = ft_calloc(string_position + 1, sizeof(char));
	if (!word)
		return (NULL);
	ft_strncpy(word, str, string_position);
	return (word);
}
