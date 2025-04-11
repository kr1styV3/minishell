/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:55:17 by chrlomba          #+#    #+#             */
/*   Updated: 2025/04/11 18:50:22 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env_variables.h"

int	get_dolpos(char *line)
{
	int		i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '$')
			return (i);
		i++;
	}
	return (-1);
}

int	heredoc_expansion(t_token **token, char *line,
		t_env_list *env_list, char **envp)
{
	int		dol_pos;
	int		var_length;
	char	*tmp;
	char	*tmp3;
	char	*tmp4;
	t_bau_args meow;

	meow.meow = env_list;
	meow.bau_bau = envp;
	dol_pos = get_dolpos(line);
	if (dol_pos > 0)
	{
		var_length = process_variable(token, line, dol_pos, &meow);
		tmp = ft_substr(line, 0, dol_pos);
		tmp3 = ft_substr(line, dol_pos + var_length, ft_strlen(line));
		free(line);
		line = ft_freejoin(tmp, (*token)->parsed->word);
		tmp4 = ft_freejoin(line, tmp3);
		free(tmp3);
		line = __ft_strdup_with_free(tmp4);
		free((*token)->parsed->word);
		(*token)->parsed->word = __ft_strdup_with_free(line);
		return (1);
	}
	return (0);
}
