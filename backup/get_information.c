#include "shell.h"

/**
 * initialize_shell_info - initializes the shell_info struct
 * @info: struct address to be initialized
 */
void initialize_shell_info(shell_info *info)
{
    info->arg = NULL;
    info->argv = NULL;
    info->path = NULL;
    info->argc = 0;
}

/**
 * setup_shell_info - sets up the shell_info struct with argument vector
 * @info: struct address to be set up
 * @argument_vector: argument vector
 */
void setup_shell_info(shell_info *info, char **argument_vector)
{
    int i = 0;

    info->fname = argument_vector[0];
    if (info->arg)
    {
        info->argv = split_string(info->arg, " \t");
        if (!info->argv)
        {
            info->argv = malloc(sizeof(char *) * 2);
            if (info->argv)
            {
                info->argv[0] = custom_strdup(info->arg);
                info->argv[1] = NULL;
            }
        }
        for (i = 0; info->argv && info->argv[i]; i++)
            ;
        info->argc = i;

        replace_aliases(info);
        replace_variables(info);
    }
}

/**
 * free_shell_info - frees allocated memory in the shell_info struct
 * @info: struct address
 * @free_all: true if freeing all fields
 */
void free_shell_info(shell_info *info, int free_all)
{
    ffree(info->argv);
    info->argv = NULL;
    info->path = NULL;
    if (free_all)
    {
        if (!info->cmd_buf)
            free(info->arg);
        if (info->env)
            free_list(&(info->env));
        if (info->history)
            free_list(&(info->history));
        if (info->alias)
            free_list(&(info->alias));
        ffree(info->environ);
        info->environ = NULL;
        bfree((void **)info->cmd_buf);
        if (info->readfd > 2)
            close(info->readfd);
        _putchar(BUF_FLUSH);
    }
}
