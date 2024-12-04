/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 15:14:48 by chrlomba          #+#    #+#             */
/*   Updated: 2024/12/04 14:31:31 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"
#include "builtins.h"
#include "t_token.h"


int skip_whitespaces(char *str, t_state *state)
{
	int string_position;

	string_position = 0;
	while (ft_isspace(str[string_position]))
		string_position++;
	if (state)
		*state = NORMAL;
	return (string_position);
}

	char *extract_token(char *str)
	{
		int string_position;
		char *token;

		string_position = 0;
		while (str[string_position] && ft_isalnum(str[string_position]))
			string_position++;
		token = ft_calloc(string_position + 1, sizeof(char));
		if (!token)
			return (NULL);
		ft_strncpy(token, str, string_position);
		return (token);
	}

char *extract_word(char *str, char quote)
{
	int string_position;
	char *word;
	bool closed_quote;

	string_position = 0;
	closed_quote = false;
	while (str[string_position] && str[string_position] != quote) // Stop at closing quote.
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

int process_token(t_token **token, char *str, int string_position, t_state *state)
{
	int	word_len;


	(*token)->parsed->token = extract_token(&str[string_position]); // Extract alphanumeric token.
	if (!(*token)->parsed->token)
		return (free_tokens_line(str, *token, "memory allocation"), -1);  // Memory allocation error.
	word_len = ft_strlen((*token)->parsed->token);  // Skip whitespaces after token.
	*state = SKIP_WHITESPACE;
	word_len += skip_whitespaces(&str[string_position + word_len], state);

	return (word_len);  // Return the length of the token processed.
}

int	process_word(t_token *token, char *str, int string_position, t_state *state, char **env)
{
	char quote;
	int len = 0;  // Either ' or ".

	quote = str[string_position];
	token->parsed->word = extract_word(&str[string_position + 1], quote);  // Extract everything inside the quotes.
	if (!token->parsed->word)
		return (free_tokens_line(str, token, "memory allocation"), -1); // Memory allocation error.
	*state = SKIP_WHITESPACE;
	if (token->parsed->token)
	{
		token->parsed->token = ft_freejoin(token->parsed->token, token->parsed->word);
		len = ft_strlen(token->parsed->word) + 2;
		free(token->parsed->word);
		token->parsed->word = NULL;
	}
	if (ft_isbuiltin(token->parsed->token))
	{
		int j = 0;
		while (str[string_position + 1 + j] != ' ')
			j++;
		j = process_builtin(token, str, string_position + 1 + j, state, env);
	}
	return (len);  // Account for opening and closing quotes.
}
