#include "minishell.h"
#include "parsing.h"
#include "builtins.h"

#define DELIMITERS " \t\r\n\a"

#define OPERATORS "<>|&"


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
		return NULL;
	ft_strncpy(token, str, string_position);
	return token;
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

int process_token(t_token *token, char *str, int string_position, t_state *state)
{
	int	word_len;

	token->token = extract_token(&str[string_position]); // Extract alphanumeric token.
	if (!token->token)
		return (free_tokens_line(str, token, "memory allocation"), -1);  // Memory allocation error.
	word_len = ft_strlen(token->token);  // Skip whitespaces after token.
	*state = SKIP_WHITESPACE;
	return (word_len);  // Return the length of the token processed.
}

int process_word(t_token *token, char *str, int string_position, t_state *state)
{
	char quote;  // Either ' or ".

	quote = str[string_position];
	token->word = extract_word(&str[string_position + 1], quote);  // Extract everything inside the quotes.
	if (!token->word)
		return (free_tokens_line(str, token, "memory allocation"), -1); // Memory allocation error.
	*state = SKIP_WHITESPACE;
	return (ft_strlen(token->word) + 2);  // Account for opening and closing quotes.
}

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
		}
		if (state == IN_BUILTIN)
			string_position += process_builtin(token, str, string_position, &state, env);
		if (state == IN_WORD)
			string_position += process_word(token, str, string_position, &state);
		if (state == IN_OPERATOR)
			string_position += process_operator(token, str, string_position, &state);
		token->next = init_token();
		token = token->next;
	}
}

void read_line_from_user(t_token *token, char **env)
{
	char	*read_line;

	read_line = readline("Minishell > ");
	if (!read_line)
		ft_error("EOF");//not error but end of readind ctrl+d
	add_history(read_line);
	tokenizer(read_line, token, env);
	print_tokens(token);

}
