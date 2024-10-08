#include "minishell.h"
#include "parsing.h"
#include "builtins.h"
#include "executor.h"
#include "read_line.h"

volatile sig_atomic_t should_exit = 0;

// Signal handler for SIGINT (Ctrl-C)
void handle_sigint(int sig)
{
	(void)sig;
	rl_on_new_line();            // Tell Readline to start a new line
    rl_replace_line("", 0);      // Clear the current input
    rl_redisplay();       // Ensure prompt is displayed immediately
}

// Signal handler for SIGTERM (Ctrl-D)
void handle_sigterm(int sig)
{
	(void)sig;
    should_exit = 1;  // Set flag to exit the shell
}

// Signal handler for SIGQUIT (Ctrl-\) - No action needed
void handle_sigquit(int sig)
{
	(void)sig;
    // Do nothing
}

// Function to set up signal handling
void setup_signal_handling()
{
    struct sigaction sa_int, sa_term, sa_quit;

    // Set up SIGINT handler (Ctrl-C)
    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;  // No special flags
    sigaction(SIGINT, &sa_int, NULL);

    // Set up SIGTERM handler (Ctrl-D)
    sa_term.sa_handler = handle_sigterm;
    sigemptyset(&sa_term.sa_mask);
    sa_term.sa_flags = 0;  // No special flags
    sigaction(SIGTERM, &sa_term, NULL);

    // Set up SIGQUIT handler (Ctrl-\) - Do nothing
    sa_quit.sa_handler = handle_sigquit;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;  // No special flags
    sigaction(SIGQUIT, &sa_quit, NULL);
}

int	checker(t_token *token, char **envp)
{
	char	*path;
	char	**paths;
	char	*part_path;
	int		i;

	i = 0;
	path = ft_getenv("PATH", envp);
	paths = ft_split(path, ':');
	free(path);
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, token->token);
		free(part_path);
		if (access(path, X_OK) == 0)
		{
			free(token->token);
			token->args[0] = ft_strdup(path);
			ft_free_mtx(paths);
			return (0);
		}
		free(path);
		i++;
	}
	ft_free_mtx(paths);
	return (1);
}

int main(int ac, char **av, char **envp)
{
	t_token		*token;
	t_token		*tmp;

	token = init_token();
	// setup_signal_handling();
	(void)ac;
	(void)av;
	while (!should_exit)
	{
		read_line_from_user(token, envp);
		return_to_head(token);
		tmp = token;
		while (tmp)
		{
			if (tmp->token)
			{
				if (checker(tmp, envp) == 1)
					free_inside_token(token, "minishell: command not found: ", tmp->token);
			}
			tmp = tmp->next;
		}
		// print_tokens(token);
		execute(token);
	}
	return 0;
}
