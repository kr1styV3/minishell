#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

#define DELIMITERS " \t\r\n\a"

// Custom function to check if a character is a delimiter
int is_delimiter(char c)
{
    return strchr(DELIMITERS, c) != NULL;
}

void ft_error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

// Custom tokenizer function
char **tokenize_input(char *input)
{
    int bufsize = 64;
    int position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;
    int start = 0, end = 0, length = strlen(input);

    if (!tokens)
        ft_error("allocation error");

    while (end <= length)
    {
        // Check if the current character is a delimiter or end of string
        if (is_delimiter(input[end]) || end == length)
        {
            if (end > start)
            {
                token = malloc((end - start + 1) * sizeof(char));
                if (!token)
                    ft_error("allocation error");
                strncpy(token, &input[start], end - start);
                token[end - start] = '\0';
                tokens[position] = token;
                position++;

                if (position >= bufsize)
                {
                    bufsize += 64;
                    tokens = realloc(tokens, bufsize * sizeof(char *));
                    if (!tokens)
                        ft_error("allocation error");
                }
            }
            start = end + 1;
        }
        // Check for redirection operators
        else if (input[end] == '<' || input[end] == '>')
        {
            if (end > start)
            {
                token = malloc((end - start + 1) * sizeof(char));
                if (!token)
                    ft_error("allocation error");
                strncpy(token, &input[start], end - start);
                token[end - start] = '\0';
                tokens[position] = token;
                position++;

                if (position >= bufsize)
                {
                    bufsize += 64;
                    tokens = realloc(tokens, bufsize * sizeof(char *));
                    if (!tokens)
                        ft_error("allocation error");
                }
            }

            // Handle << and >>
            if (input[end + 1] == input[end])
            {
                token = malloc(3 * sizeof(char));
                if (!token)
                    ft_error("allocation error");
                token[0] = input[end];
                token[1] = input[end];
                token[2] = '\0';
                end++;
            }
            else
            {
                token = malloc(2 * sizeof(char));
                if (!token)
                    ft_error("allocation error");
                token[0] = input[end];
                token[1] = '\0';
            }

            tokens[position] = token;
            position++;

            if (position >= bufsize)
            {
                bufsize += 64;
                tokens = realloc(tokens, bufsize * sizeof(char *));
                if (!tokens)
                    ft_error("allocation error");
            }
            start = end + 1;
        }
        end++;
    }
    tokens[position] = NULL;
    return tokens;
}


void read_line_from_user()
{
    char command[100];
    char **tokens;

    if (fgets(command, sizeof(command), stdin) == NULL)
        ft_error("fgets");

    // Tokenize the input command using the custom tokenizer
    tokens = tokenize_input(command);

    // Print tokens for verification
    for (int i = 0; tokens[i] != NULL; i++)
    {
        printf("Token: %s\n", tokens[i]);
        free(tokens[i]); // Free each token after use
    }

    free(tokens); // Free the array of tokens
}