/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_exctract_files.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 16:01:54 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/10 13:28:24 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_file_cmd(t_token *token, char *str, int i)
{
	int		length;
	char	*file;

	length = i;
	file = extract_bash_file(&str[length]);
	if (!file)
		return (free_tokens_line(str, token, "malloc error"),
			ft_strlen(str) - i);
	length += ft_strlen(file);
	if (token->arg == NULL)
		token->arg = (char **)ft_calloc(2, sizeof(char *));
	token->arg[0] = ft_strdup(file);
	if (!token->arg[0])
		return (free(file), free_tokens_line(str, token, "malloc error"),
			ft_strlen(str) - i);
	free(file);
	token->checker = false;
	return (length);
}

char	*extract_bash_file(char *str)
{
	int		i;
	int		start;

	if (!str)
		return (NULL);
	i = 0;
	if (str[i] == '\"')
	{
		i++;
		start = i;
		while (str[i] && str[i] != '\"')
			i++;
		return (ft_strndup(&str[start], i - start));
	}
	start = i;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '.' || str[i] == '/'))
		i++;
	return (ft_strndup(str, i));
}

int	skip_assignment_whitespace(const char *line, int i)
{
	if (line[i] != '=')
		return (-1);
	i++;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	return (i);
}

int	var_cleanup(char *full_entry, char *var_value, char *str)
{
	write(2, str, ft_strlen(str));
	free(full_entry);
	free(var_value);
	return (-1);
}
