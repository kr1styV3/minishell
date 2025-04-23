/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_process_variable.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 20:04:17 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/14 16:02:38 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/parsing.h"
#include "../headers/builtins.h"
#include "../headers/executor.h"
#include "../my_libft/headers/get_next_line.h"
#include "../my_libft/headers/libft.h"
#include "../headers/read_line.h"
#include "env_variables.h"

static int	process_simple_variable(t_token **token,
	char *str, int string_position, t_env_list *env)
{
	char	*variable;
	int		len;

	variable = extract_until_not_alfanum(&str[string_position]);
	if (!variable)
		return (free_tokens_line(str, *token, "malloc error"), 0);
	(*token)->parsed->word = ft_getenv(variable, env);
	len = ft_strlen(variable);
	free(variable);
	return (len);
}

static int	process_exit_code_variable(t_token **token)
{
	ft_putnbr_fd((*token)->last_exit_status, STDOUT_FILENO);
	return (1);
}

static int	process_braced_variable(t_token **token,
		char *str, int string_position, t_env_list *env)
{
	char	*variable;
	int		len;

	string_position++;
	variable = extract_until_not_alfanum(&str[string_position]);
	if (!variable)
		return (free_tokens_line(str, *token, "malloc error"), 0);
	if (str[string_position + ft_strlen(variable)] != '}')
	{
		free(variable);
		return (free_tokens_line(str, *token, "missing '}'"), 0);
	}
	(*token)->parsed->word = ft_getenv(variable, env);
	len = ft_strlen(variable) + 2;
	free(variable);
	return (len);
}

static int	process_subshell_variable(t_token **token,
	char *str, int string_position, t_bau_args *meow_norm)
{
	char	*command;
	char	*command_output;
	char	*trimmed;

	command = extract_word(&str[string_position + 1], ')');
	if (!command)
		return (free_tokens_line(str, *token, "error\n"), 0);
	command_output = execute_and_capture_output(command,
			meow_norm->meow, meow_norm->bau_bau);
	free(command);
	if (!command_output)
		return (free_tokens_line(str, *token, "command execution error"), 0);
	trimmed = ft_strtrim(command_output, " ");
	free(command_output);
	if (!trimmed)
		return (free_tokens_line(str, *token, "malloc error"), 0);
	(*token)->parsed->word = ft_strdup(trimmed);
	free(trimmed);
	if (!(*token)->parsed->word)
		return (free_tokens_line(str, *token, "malloc error"), 0);
	return (ft_strlen(command) + 2);
}

int	process_variable(t_token **token, char *str, int string_position,
	t_bau_args *miao_diobon)
{
	if (str[string_position] == '{')
		return (process_braced_variable(token, str,
				string_position, miao_diobon->meow));
	else if (str[string_position] == '(')
		return (process_subshell_variable(token,
				str, string_position, miao_diobon));
	else if (str[string_position] == '?')
		return (process_exit_code_variable(token));
	else
		return (process_simple_variable(token,
				str, string_position, miao_diobon->meow));
}
