/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_token.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 10:56:31 by chrlomba          #+#    #+#             */
/*   Updated: 2025/01/05 22:12:24 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_TOKEN_H
# define T_TOKEN_H

typedef struct s_operator
{
	char	operator;
	int		fd_overwrite_output;// command > file /*
	int		fd_append_output;// command >> file	  * fd da dove prendere il file per eseguzione
	int		fd_input;// command < file	          /*
}	t_operator;

typedef struct s_parse
{
	char	*token;
	char	*word;
}	t_parse;

typedef struct s_doc
{
	char	*eof;
	bool	here_doc;

}	t_doc;

typedef struct s_env
{
	void	*env_ptr;
	bool	env_work;
}	t_env;
typedef struct s_token
{
	bool				free;
	char				**arg;
	struct s_token		*next;
}	t_token;

typedef struct s_mini
{
	bool				exec;
	unsigned int		last_exit_status;
	struct s_env		*env;
	struct s_doc		*doc;
	struct s_operator	*operator;
	struct s_parse		*parsed;
	struct t_token		*token;
}	t_mini;

#endif
