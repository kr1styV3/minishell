/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_token.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coca <coca@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 10:56:31 by chrlomba          #+#    #+#             */
/*   Updated: 2025/03/13 04:38:11 by coca             ###   ########.fr       */
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

typedef struct s_env_list
{
	char				*value;
	struct s_env_list	*next;
}	t_env_list;

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
	bool				env_work;
	bool				exec;
	bool				free;
	bool				pipe;
	bool				checker;
	unsigned int		last_exit_status;
	char				**arg;
	struct s_token		*next;
	struct s_operator	*operator;
	struct s_parse		*parsed;
	struct s_env		*env;
	struct s_doc		*doc;
}	t_token;



#endif
