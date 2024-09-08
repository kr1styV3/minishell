#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

#define DELIMITERS " \t\r\n\a"

#define OPERATORS "<>|&"


int skip_whitespaces(char *str)
{
	int i = 0;
	while (ft_isspace(str[i]))
		i++;
	return i;
}

char *extract_token(char *str)
{
	int i = 0;
	char *token;

	// Continue as long as it's alphanumeric.
	while (str[i] && ft_isalnum(str[i]))
		i++;

	token = ft_calloc(i + 1, sizeof(char));
	if (!token)
		return NULL;

	// Copy the valid token.
	ft_strncpy(token, str, i);
	return token;
}

char *extract_word(char *str)
{
	int i = 0;
	char *word;

	while (str[i] && str[i] != 34 && str[i] != 39) // Stop at closing quote.
		i++;

	word = ft_calloc(i + 1, sizeof(char));
	if (!word)
		return NULL;

	ft_strncpy(word, str, i);
	return word;
}

int process_token(t_token *token, char *str)
{
	token->token = extract_token(str); // Extract alphanumeric token.
	if (!token->token)
		return -1;  // Memory allocation error.

	return ft_strlen(token->token);  // Return the length of the token processed.
}

int process_word(t_token *token, char *str)
{
	char quote = str[0];  // Either ' or ".
	token->word = extract_word(&str[1]);  // Extract everything inside the quotes.
	(void)quote;
	if (!token->word)
		return -1;

	return ft_strlen(token->word) + 2;  // Account for opening and closing quotes.
}

int process_operator(t_token *token, char *str)
{
	token->operator = str[0];  // Save the operator (e.g., |, >, <).

	// Special case for ">>" operator.
	if (str[0] == '>' && str[1] == '>')
		return 2;

	return 1;
}

void tokenizer(char *str, t_token *token)
{
	int string_position;
	t_state state;

	string_position = 0;
	state = NORMAL;
	while (str[string_position])
	{
		if (state == SKIP_WHITESPACE)
		{
			string_position += skip_whitespaces(&str[string_position]);
			state = NORMAL;
		}

		else if (state == NORMAL)
		{
			if (ft_isalnum(str[string_position]))
			{
				string_position += process_token(token, &str[string_position]);
				state = SKIP_WHITESPACE;
			}
			else if (str[string_position] == 34 || str[string_position] == 39)  // Quote found.
			{
				state = IN_WORD;
			}
			else if (ft_strchr(OPERATORS, str[string_position]))  // Operator found.
			{
				state = IN_OPERATOR;
			}
			else
			{
				string_position++;  // Move forward for unknown characters.
			}
		}
		else if (state == IN_WORD)
		{
			string_position += process_word(token, &str[string_position]);
			state = SKIP_WHITESPACE;
		}
		else if (state == IN_OPERATOR)
		{
			string_position += process_operator(token, &str[string_position]);
			state = SKIP_WHITESPACE;
		}
		token->next = init_token();
		token = token->next;
	}
}

void read_line_from_user(t_token *token)
{
	char	*read_line;

	read_line = readline("Minishell > ");
	if (!read_line)
		ft_error("EOF");//not error but end of readind ctrl+d
	tokenizer(read_line, token);
	print_tokens(token);

}
