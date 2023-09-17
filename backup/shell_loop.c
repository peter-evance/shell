#include "shell.h"

/**
 * shell_loop - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int shell_loop(shell_info *info, char **av)
{
    ssize_t read_result = 0;
    int builtin_ret = 0;

    while (read_result != -1 && builtin_ret != -2)
    {
        clear_info(info);
        if (interactive(info))
            _puts("$ ");
        _eputchar(BUF_FLUSH);
        read_result = get_input(info);
        if (read_result != -1)
        {
            set_info(info, av);
            builtin_ret = find_builtin(info);
            if (builtin_ret == -1)
                find_command(info);
        }
        else if (interactive(info))
            _putchar('\n');
        free_info(info, 0);
    }
    write_history(info);
    free_info(info, 1);
    if (!interactive(info) && info->status)
        exit(info->status);
    if (builtin_ret == -2)
    {
        if (info->err_num == -1)
            exit(info->status);
        exit(info->err_num);
    }
    return (builtin_ret);
}

/**
 * find_builtin - finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *         0 if builtin executed successfully,
 *         1 if builtin found but not successful,
 *         2 if builtin signals exit()
 */
int find_builtin(shell_info *info)
{
    int i, built_in_ret = -1;
    builtin_table builtintbl[] = {
        {"exit", my_exit},
        {"env", my_env},
        {"help", my_help},
        {"history", my_history},
        {"setenv", my_setenv},
        {"unsetenv", my_unsetenv},
        {"cd", my_cd},
        {"alias", my_alias},
        {NULL, NULL}
    };

    for (i = 0; builtintbl[i].type; i++)
    {
        if (_strcmp(info->argv[0], builtintbl[i].type) == 0)
        {
            info->line_count++;
            built_in_ret = builtintbl[i].func(info);
            break;
        }
    }
    return (built_in_ret);
}

/**
 * find_command - finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void find_command(shell_info *info)
{
    char *path = NULL;
    int i, count_non_delim = 0;

    info->path = info->argv[0];
    if (info->linecount_flag == 1)
    {
        info->line_count++;
        info->linecount_flag = 0;
    }
    for (i = 0; info->arg[i]; i++)
    {
        if (!is_delimiter(info->arg[i], " \t\n"))
        {
            count_non_delim++;
        }
    }
    if (!count_non_delim)
        return;

    path = find_path(info, _getenv(info, "PATH="), info->argv[0]);
    if (path)
    {
        info->path = path;
        fork_command(info);
    }
    else
    {
        if ((interactive(info) || _getenv(info, "PATH=") || info->argv[0][0] == '/') && is_command(info, info->argv[0]))
            fork_command(info);
        else if (*(info->arg) != '\n')
        {
            info->status = 127;
            print_shell_error(info, "not found\n");
        }
    }
}

/**
 * fork_command - forks an exec thread to run a command
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void fork_command(shell_info *info)
{
    pid_t child_pid;

    child_pid = fork();
    if (child_pid == -1)
    {
        /* TODO: PUT ERROR FUNCTION */
        perror("Error:");
        return;
    }
    if (child_pid == 0)
    {
        if (execve(info->path, info->argv, get_environ(info)) == -1)
        {
            free_info(info, 1);
            if (errno == EACCES)
                exit(126);
            exit(1);
        }
        /* TODO: PUT ERROR FUNCTION */
    }
    else
    {
        wait(&(info->status));
        if (WIFEXITED(info->status))
        {
            info->status = WEXITSTATUS(info->status);
            if (info->status == 126)
                print_shell_error(info, "Permission denied\n");
        }
    }
}
