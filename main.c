#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "main.h"
#define TOKEN_LEN 16

/**
 * main - simple shell
 * Return: 0 on success
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nline;
	char **tokens = NULL;
	int status, i;

	while (1)
	{
		printf("($) ");
		nline = getline(&line, &len, stdin); /* get line*/
		if (nline == -1)
		{
			free(line);
			return (0);
		}

		line[nline - 1] = '\0';
		if (strcmp(line, "exit") == 0)
			break;
		/* tokenize*/
		tokens = tokenizer(line);
		/* execute*/
		status = execute(tokens);
		/* clean up */
		free_all(tokens);
	}
	if (line != NULL)
		free(line);
	return (status);
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
		tokens[position++] = strdup(token);

		/* If allocated memory isn't enough */
		if (position > tok_size)
		{
			tok_size += TOKEN_LEN;
			tokens = realloc(tokens, tok_size);
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
 * free_all - frees memory for tokens
 * @tokens: array of string
 */
void free_all(char **tokens)
{
	size_t i;

	if (tokens == NULL)
		return;
	for (i = 0; tokens[i] != NULL; i++)
		free(tokens[i]);

	free(tokens);
}

/**
 * execute - execute a command
 * @args: array of strings
 * Return: status
 */
int execute(char **args)
{
	pid_t child_pid;
	int status;

	child_pid = fork();
	if (child_pid == -1)
		return (1);

	if (child_pid == 0)
	{
		if (execve(args[0], args, NULL) == -1)
			perror("Error");
	}
	else
		wait(&status);

	return (status);
}
