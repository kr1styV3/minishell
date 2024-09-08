#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

#define DELIMITERS " \t\r\n\a"

#define OPERATORS "<>|&"



int	skip_whitespaces(char *c)
{
	int	i;

	i = 0;
	if(ft_isspace(*c) == 1)
	{
		while (ft_isspace(c[i]) == 1)
			i++;
		return (i);
	}
	return (0);
}

char	*load_token(char *str)
{
	int		i;
	char	*token;

	i = 0;
	while (str[i] && ft_isalnum(str[i]) == 1)
		i++;
	token = ft_calloc(sizeof(char) , i + 1);
	if (!token)
		return (NULL);
	i = 0;
	while (str[i] && ft_isalnum(str[i]) == 1)
	{
		token[i] = str[i];
		i++;
	}
	return (token);
}

void	free_tokens_line(char *str, t_token *token, char *error_message)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token->next;
		if (token->token)
			free(token->token);
		if (token->word)
			free(token->word);
		free(token);
		token = tmp;
	}
	if (str)
		free(str);
	ft_error(error_message);
}

char	*load_word(char *str)
{
	int		i;
	char	*word;

	i = 0;
	while (str[i] && str[i] != 34 && str[i] != 39)
		i++;
	word = ft_calloc(sizeof(char) , i + 1);
	if (!word)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != 34 && str[i] != 39)
	{
		word[i] = str[i];
		i++;
	}
	return (word);
}

int	understand_load_token(char *str, t_token *token)
{
	int	i;

	i = 0;
	if (ft_isalnum(str[i]) == 1)
	{
		token->token = load_token(&str[i]);
		if (!token->token)
			free_tokens_line(str, token, "Failed to allocate memory for token value.");
		i += ft_strlen(token->token);
	}
	i += skip_whitespaces(&str[i]);
	if (str[i] == 34 || str[i] == 39)
	{
		token->word = load_word(&str[i + 1]);
		if (!token->word)
			free_tokens_line(str, token, "Failed to allocate memory for word value.");
		i += ft_strlen(token->word) + 2;
	}
	// if (ft_strchr(OPERATORS, str[i]))
	// {
	// 	if (str[i] == '<')
	// 		check_input_fd(token);
	// 	else if (str[i] == '>' && str[i + 1] == '>')
	// 		check_append_fd(token, &str[i]);
	// 	else if (str[i] == '>')
	// 		check_output_fd(token, &str[i]);
	// 	else if (str[i] == '|')
	// 		token->pipe = 1;
	// 	else if (str[i] == '&')
	// 		token->background = 1;
	// }
	return (i);
}

void	tokenizer(char *str, t_token *token)
{
	int		string_position;
	t_token	*initial_token;

	initial_token = token;
	(void)initial_token;
	string_position = 0;
	while (str && str[string_position])
	{
		if (ft_isspace(str[string_position]) == 1)
			string_position += skip_whitespaces(&str[string_position]);
		string_position += understand_load_token(&str[string_position], token);
		token->next = init_token();
		token = token->next;
	}
}

void	print_tokens(t_token *token)
{
	t_token	*tmp;

	tmp = token;
	while (tmp)
	{
		if (tmp->token)
			printf("Token: %s\n", tmp->token);
		if (tmp->word)
			printf("Word: %s\n", tmp->word);
		tmp = tmp->next;
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
