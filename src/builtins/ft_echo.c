/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 05:13:40 by chrlomba          #+#    #+#             */
/*   Updated: 2024/09/09 05:40:17 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/builtins.h"
#include "../../headers/parsing.h"

char	*extract_until_not_alfanum(char *str)
{
	int		i;
	int		k;
	char	*word;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == ' '))
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

int	ft_echo(t_token *token, char *str, int i)
{
	int		len;
	bool	flag;
	char	quote;

	len = skip_whitespaces(&str[i], NULL);
	flag = false;
	char	*word;
	if (str[i + len] == '-' && str[i + len + 1] == 'n')
	{
		flag = true;
		len += 2;
	}
	len += skip_whitespaces(&str[i + len], NULL);
	quote = str[i + len];
	if (quote == '\'' || quote == '\"')
	{
		word = extract_word(&str[i + len + 1], quote);
		len += 2;
	}
	else
		word = extract_until_not_alfanum(&str[i + len]);
	if (!word)
		return (free_tokens_line(str, token, "malloc error"), -1);
	len += ft_strlen(word);
	ft_putstr_fd(word, 1);
	if (!flag)
		ft_putchar_fd('\n', 1);
	free(word);
	return (len);
}
