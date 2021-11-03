#include "main.h"
#define TOKEN_LEN 16

/**
 * main - simple shell
 * @argc: number of arguments
 * @argv: array of arguments
 * Return: 0 on success
 */
int main(void)
{
	while (1)
	{
		char input[SIZE + 1] = {0x0};
		char *arguments[ARGUMENTS_MAX + 1] = {NULL};
		char tty_flag;
		int i;
		char *pointer = input;
		int current_status;

		tty_flag = (char) isatty(STDIN_FILENO);

		if (tty_flag == 1)
			printf("%s ", getuid() == 0 ? "#" : "$");
		fgets(input, SIZE, stdin);

		if (*pointer == '\n')
			continue;

		for (i = 0; *pointer && i < (int) sizeof(arguments) ; pointer++)
		{
			if (*pointer == '\n')
				break;
			if (*pointer == ' ')
				continue;
			for (arguments[i++] = pointer; *pointer != '\n' && *pointer &&
										   *pointer != ' '; pointer++)
				;
			*pointer = '\0';
		}

		if (compare_strs(COMMAND_EXIT, arguments[0]) == 0)
			return (0);
		signal(SIGINT, SIG_DFL);

		if (fork() == 0)
			exit(execvp(arguments[0], arguments));
		signal(SIGINT, SIG_IGN);

		wait(&current_status);
	}
}
/**
 * tokenizer - splits str into tokens
 * @str: string to be tokenzed
 * Return: array of token strings
 */
char **tokenizer(char *str)
{
	char **tokens = NULL, *token;
	const char *DELIM = " \t\a\r";
	size_t position = 0, tok_size = TOKEN_LEN, i;

	tokens = malloc(tok_size * sizeof(char *));
	if (tokens == NULL)
		return (NULL);

	token = strtok(str, DELIM);
	while (token != NULL)
	{
		tokens[position++] = _strdup(token);

		/* If allocated memory isn't enough */
		if (position > tok_size)
		{
			tok_size += TOKEN_LEN;
			tokens = _realloc(tokens, position + 1, tok_size);
			if (tokens == NULL)
			{
				for (i = 0; i < position; i++)
					free(tokens[i]);
				return (NULL);
			}
		}
		token = strtok(NULL, DELIM);
	}
	tokens[position] = NULL;

	return (tokens);
}

/**
 * launch - gets path for str
 * @str: path key
 * Return: status
 */
int launch(char **str)
{
	int status = 1;
	char *path, *tok, *cmd;
	struct stat statbuf;

	/* get path */
	path = _getenv("PATH");

	/* find given string */
	tok = strtok(path, ":");
	while (tok != NULL)
	{
		/* find if the commmand exists in a path */
		cmd = _strdup(tok);
		_strcat(_strcat(cmd, "/"), *str);
		if (stat(cmd, &statbuf) == 0)
		{
			free(*str);
			*str = _strdup(cmd);
			status = 0;
			break;
		}
		free(cmd);
		tok = strtok(NULL, ":");
	}
	free(path);

	return (status);
}
