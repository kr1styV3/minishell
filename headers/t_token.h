/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_token.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coca <coca@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 10:56:31 by chrlomba          #+#    #+#             */
/*   Updated: 2025/02/15 09:57:11 by coca             ###   ########.fr       */
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

typedef struct s_token
{
	bool				env_work;
	bool				exec;
	bool				free;
	bool				here_doc;
	char				*echo;
	bool		pipe;
	char				*eof;
	char				**arg;
	unsigned int		last_exit_status;
	struct s_operator	*operator;
	struct s_parse		*parsed;
	struct s_env_list	*env_ptr;
	struct s_token		*next;
}	t_token;

#endif
