#ifndef GUARD_SHELL
#define GUARD_SHELL

char **tokenizer(char *str);
size_t len_tokens(char *str);
void free_all(char **tokens, size_t size);
int execute(char **args);
#endif
