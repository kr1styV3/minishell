/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_alloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 18:24:19 by chrlomba          #+#    #+#             */
/*   Updated: 2024/12/22 21:08:30 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/env_variables.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------------------------------------------------------
   A helper to "extract" the value from `line` starting at *p_i.
   For demonstration, it:
     - Skips leading spaces/tabs.
     - If there's a double-quote, it reads until the closing quote.
     - Otherwise, it reads until the next whitespace or end of string.
   Adjust this to your real grammar (handling escaped quotes, etc.).
   --------------------------------------------------------- */
char *extract_value(const char *line, int *p_i)
{
    int i;
    int start;
    int end;
    int quoted;
    int length;
    char *val;

    i = *p_i;      /* pull out the caller’s index */
    quoted = 0;    /* 0 = not in quotes, 1 = in quotes */

    /* Skip leading spaces/tabs */
	if (line[i] != '=')
		return NULL;
	i++;
    while (line[i] == ' ' || line[i] == '\t')
    {
        i++;
    }

    /* Check if the value is in quotes */
    if (line[i] == '"')
    {
        quoted = 1;
        i++;
    }

    start = i;

    if (quoted == 1)
    {
        /* read until next quote or end of string */
        while (line[i] != '\0' && line[i] != '"')
        {
            i++;
        }
        end = i;
        if (line[i] == '"')
        {
            i++;  /* skip the closing quote */
        }
    }
    else
    {
        /* read until space/tab or end of string */
        while (line[i] != '\0' && line[i] != ' ' && line[i] != '\t')
        {
            i++;
        }
        end = i;
    }

    length = end - start;
    val = (char *)malloc(length + 1);
    if (val == NULL)
    {
        /* error handling (return NULL) */
        return NULL;
    }
    /* Copy the substring into `val` */
    memcpy(val, &line[start], length);
    val[length] = '\0';

    /* Update the caller's index */
    *p_i = i;

    return val;
}

/* ---------------------------------------------------------
   Find if var_name already exists in env.

   Returns the index if found, or -1 if not found.
   Uses a while loop (not a for).
   --------------------------------------------------------- */
int env_find_var(char **env, const char *var_name)
{
    int i;
    size_t name_len;

    i = 0;
    if (env == NULL)
    {
        return -1;
    }
    if (var_name == NULL)
    {
        return -1;
    }

    name_len = strlen(var_name);

    while (env[i] != NULL)
    {
        /* We look for "var_name=" as a prefix */
        if (strncmp(env[i], var_name, name_len) == 0 && env[i][name_len + 1] == '=')
        {
            return i; /* found */
        }
        i++;
    }
    return -1; /* not found */
}

/* ---------------------------------------------------------
   Replace an existing env entry at position idx with new_entry.
   We free the old string, then strdup the new one.
   --------------------------------------------------------- */
int env_replace_entry(char **env, int idx, const char *new_entry)
{
    char *tmp;

    if (env == NULL)
    {
        return -1;
    }
    if (idx < 0)
    {
        return -1;
    }
    free(env[idx]);
    tmp = strdup(new_entry);
    if (tmp == NULL)
    {
        return -1;
    }
    env[idx] = tmp;
    return 0;
}

/* ---------------------------------------------------------
   Reallocate env to add a new entry `new_entry`.
   env is NULL-terminated. The new array is also NULL-terminated.
   Returns the pointer to the new array, or NULL on error.
   --------------------------------------------------------- */
/*
** env_add_entry_no_realloc:
**   - env:       existing NULL-terminated array of strings (the environment)
**   - new_entry: string to add (e.g., "NAME=value")
**
** Steps:
**   1) Count how many entries in env.
**   2) Allocate a new array of (count+2) pointers.
**   3) Deep-copy all old strings into new_env[i] with strdup().
**   4) strdup() the new_entry and assign it as the last entry.
**   5) NULL-terminate new_env.
**   6) Free each old env[i], then free(env).
**   7) Return new_env.
**
** Returns:
**   - Pointer to newly allocated environment array on success.
**   - NULL on allocation failure (partially allocated memory is freed).
*/

#include <stdlib.h>   /* malloc, free */
#include <string.h>   /* strdup */

char **env_add_entry(char **env, const char *new_entry)
{
	char **new_env;
	char *copy;
	int  count;
	int  i;

	count = 0;
	if (env != NULL)
	{
		while (env[count] != NULL)
			count++;
	}
	new_env = (char **)malloc((count + 2) * sizeof(char *));
	if (new_env == NULL)
		return NULL;
	i = 0;
	while (i < count)
	{
		new_env[i] = env[i];
		if (new_env[i] == NULL)
			return(ft_free_mtx(new_env), NULL);
		i++;
	}
	copy = strdup(new_entry);
	if (copy == NULL)
		return(ft_free_mtx(new_env), NULL);
	new_env[i] = copy;
	i++;
	new_env[i] = NULL;
	env = NULL;
	return new_env;
}


/* ---------------------------------------------------------
   check_var:
   - `str` might be something like "gigi="
   - we read `line` at index `i` to get the new value (e.g., "lalal")
   - combine to "gigi=lalal"
   - see if "gigi" already in (*env_ptr); if yes, replace, else add.
   - Because we may reallocate env, we accept `char ***env_ptr`.
	 We'll do `*env_ptr = new_env` if we expand it.
   --------------------------------------------------------- */
int check_var(char *str, char *line, int i, char **env_ptr)
{
	char *var_value;
	char *full_entry;
	size_t total_len;
	int existing_idx;
	char **new_env;

	/* Extract the value from line (e.g., "lalal") at index i */
	var_value = extract_value(line, &i);
	if (var_value == NULL)
	{
		return -1;
	}

	/*
	 * Build "NAME=VALUE"
	 * e.g., if str = "gigi" and var_value = "lalal",
	 * then full_entry = "gigi=lalal"
	 */
	total_len = strlen(str) + 1 + strlen(var_value) + 1; /* +1 for '=' and +1 for '\0' */
	full_entry = (char *)malloc(total_len);
	if (full_entry == NULL)
	{
		free(var_value);
		return -1;
	}
	sprintf(full_entry, "%s%s", str, var_value);

	/* Check if str (the variable name) already exists in (*env_ptr) */
	existing_idx = env_find_var(env_ptr, str);
	if (existing_idx >= 0)
	{
		/* If found, replace in place */
		if (env_replace_entry(env_ptr, existing_idx, full_entry) < 0)
		{
			fprintf(stderr, "Failed to replace env entry\n");
			/* Minimal error handling here */
		}
	}
	else
	{
		/* If not found, add a new entry (realloc env) */
		new_env = env_add_entry(env_ptr, full_entry);
		if (new_env == NULL)
		{
			fprintf(stderr, "Failed to reallocate env\n");
			/* Minimal error handling */
			free(full_entry);
			free(var_value);
			return -1;
		}
		/* Update the caller's env pointer */
		env_ptr = new_env;
	}

	/* Clean up temporary strings */
	free(full_entry);
	free(var_value);

	return 0; /* success */
}
