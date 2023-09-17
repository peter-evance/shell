#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;

/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 * struct passinfo - contains pseudo-arguements to pass into a function,
 * allowing uniform prototype for function pointer struct
 * @arg: a string generated from getline containing arguements
 * @argv:an array of strings generated from arg
 * @path: a string path for the current command
 * @argc: the argument count
 * @line_count: the error count
 * @err_num: the error code for exit()s
 * @linecount_flag: if on count this line of input
 * @fname: the program filename
 * @env: linked list local copy of environ
 * @environ: custom modified copy of environ from LL env
 * @history: the history node
 * @alias: the alias node
 * @env_changed: on if environ was changed
 * @status: the return status of the last exec'd command
 * @cmd_buf: address of pointer to cmd_buf, on if chaining
 * @cmd_buf_type: CMD_type ||, &&, ;
 * @readfd: the fd from which to read line input
 * @histcount: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} shell_info;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
		0, 0, 0}

/**
 * struct builtin - contains a builtin string and related function
 * @type: the builtin command flag
 * @func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(shell_info *);
} builtin_table;


/* shloop.c */
int hsh(shell_info *, char **);
int find_builtin(shell_info *);
void find_cmd(shell_info *);
void fork_cmd(shell_info *);

/* parser.c */
int is_cmd(shell_info *, char *);
char *dup_chars(char *, int, int);
char *find_path(shell_info *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* errors.c */
void print_to_stderr(char *);
int _eputchar(char);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);

/* string.c */
int _strlen(char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *_strcat(char *, char *);

/* string1.c */
char *custom_strcpy(char *, char *);
char *custom_strdup(const char *);
void _puts(char *);
int _putchar(char);

/* exits.c */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

/* tokenizer.c */
char **split_string(char *, char *);
char **split_string2(char *, char);

/* realloc.c */
char *_memset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);

/* memory.c */
int bfree(void **);

/* atoi.c */
int interactive(shell_info *);
int is_delim(char, char *);
int is_an_alphabet(int);
int shell_utilities(char *);

/* errors1.c */
int str_to_int(char *);
void print_shell_error(shell_info *, char *);
int print_integer(int, int);
char *int_to_str(long int, int, int);
void remove_comments(char *);

/* builtin.c */
int my_exit(shell_info *);
int _mycd(shell_info *);
int _myhelp(shell_info *);

/* builtin1.c */
int _myhistory(shell_info *);
int _myalias(shell_info *);

/*getline.c */
ssize_t get_input(shell_info *);
int _getline(shell_info *, char **, size_t *);
void sigintHandler(int);

/* getinfo.c */
void clear_info(shell_info *);
void set_info(shell_info *, char **);
void free_info(shell_info *, int);

/* environ.c */
char *_getenv(shell_info *, const char *);
int _myenv(shell_info *);
int _mysetenv(shell_info *);
int _myunsetenv(shell_info *);
int populate_env_list(shell_info *);

/* getenv.c */
char **get_environ(shell_info *);
int _unsetenv(shell_info *, char *);
int _setenv(shell_info *, char *, char *);

/* history.c */
char *get_history_file(shell_info *info);
int write_history(shell_info *info);
int read_history(shell_info *info);
int build_history_list(shell_info *info, char *buf, int linecount);
int renumber_history(shell_info *info);

/* lists.c */
list_t *add_node(list_t **, const char *, int);
list_t *add_node_end(list_t **, const char *, int);
size_t print_list_str(const list_t *);
int delete_node_at_index(list_t **, unsigned int);
void free_list(list_t **);

/* lists1.c */
size_t list_len(const list_t *);
char **list_to_strings(list_t *);
size_t print_list(const list_t *);
list_t *node_starts_with(list_t *, char *, char);
ssize_t get_node_index(list_t *, list_t *);

/* vars.c */
int is_chain_separator(shell_info *, char *, size_t *);
void check_chain_condition(shell_info *, char *, size_t *, size_t, size_t);
int replace_aliases(shell_info *);
int replace_variables(shell_info *);
int replace_string(char **, char *);

#endif
