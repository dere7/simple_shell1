#ifndef GUARD_SHELL
#define GUARD_SHELL


#define ARGUMENTS_MAX 63
#define SIZE 255
#define COMMAND_EXIT "exit"

#include <stddef.h>

char **tokenizer(char *str);
size_t len_tokens(char *str);
void free_all(char **tokens);
int execute(char **args);
int launch(char **str);
void readline(char **line);

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

int compare_strs(char *s1, char *s2);
/* utility functions */
char *_getenv(char *str);

/* builtins */

/**
 * struct builtins_s - to manage builtin functions
 * @name: name of a command
 * @desc: short description about the function
 * @func: the associated function
 *
 * Description: manages builtin functions
 */
typedef struct builtins_s
{
	char *name, *desc;
	int (*func)(char **args);
} builtin_t;
int cd(char **args);
int help(char **args, builtin_t *builtins);
int cexit(char **args);
int env(char **args);
void printBuiltins(builtin_t b);

/* string manipulation */
int _strcmp(char *s1, char *s2);
char *_strdup(char *str);
char *_strcat(char *dest, char *src);
char *_strstr(char *haystack, char *needle);
int _strlen(char *s);
int _atoi(char *s);

/* memory utility */
char *_memcpy(char *dest, char *src, unsigned int n);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);

/* misc */
void print(char *s);
int print_num(long n);
int _putchar(char c);
#endif
