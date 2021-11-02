#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "main.h"
#define TOKEN_LEN 16

/**
 * main - simple shell
 * @argc: number of arguments
 * @argv: array of arguments
 * Return: 0 on success
 */
int main(int argc __attribute__((unused)), char *argv[])
{
	char *line = NULL;
	char **tokens = NULL;
	int status, lineNumber = 1;

	while (1)
	{
		fflush(NULL);
		if (isatty(STDIN_FILENO))
			print("($) ");
		readline(&line);
		tokens = tokenizer(line); /* tokenize*/
		status = execute(tokens); /* execute*/
		if (status != 0)
		{
			print(argv[0]);
			print(": ");
			print_num(lineNumber);
			print(": ");
			fflush(stdout);
			if (status == 1)
			{
				print(tokens[0]);
				print(": not found\n");
			}
			else
				perror(tokens[0]);
		}
		lineNumber++;
		free_all(tokens); /* clean up */
	}
	if (line != NULL)
		free(line);
	return (status);
}

/**
 * readline - reads till new line
 * @line: string to be stored
 */
void readline(char **line)
{
	ssize_t nline;
	size_t len;

	nline = getline(line, &len, stdin); /* get line*/
	if (nline == -1)
	{
		free(*line);
		exit(0);
	}
	(*line)[nline - 1] = '\0';
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
 * execute - execute a command
 * @args: array of strings
 * Return: status
 */
int execute(char **args)
{
	pid_t child_pid;
	int status, i;
	builtin_t builtin[] = {
		{ "cd", "change current working directory: cd [<pathname>]", &cd },
		{ "exit", "exits out of shell: exit [<status>]", &cexit},
		{ "env", "prints enviromental variables: env", &env  },
		{NULL, NULL, NULL}
	};

	if (args == NULL || args[0] == NULL)
		return (0);

	/* search for builtins */
	if (_strcmp(args[0], "help") == 0)
		return (help(args, builtin));

	for (i = 0; builtin[i].name != NULL; i++)
	{
		if (_strcmp(args[0], builtin[i].name) == 0)
			return (builtin[i].func(args));
	}
	if (launch(&args[0]) != 0)
		return (1);

	child_pid = fork();
	if (child_pid == -1)
		return (1);

	if (child_pid == 0)
	{
		if (execve(args[0], args, __environ) == -1)
			return (1);
	}
	else
		wait(&status);

	return (status);
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
