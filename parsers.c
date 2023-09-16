#include "shell.h"

/**
 * is_executable_command - determines if a file is an executable command
 * @info: the shell_info struct
 * @path: path to the file
 * Return: 1 if true, 0 otherwise
 */
int is_executable_command(shell_info *info, char *path)
{
    struct stat st;

    (void)info;
    if (!path || stat(path, &st))
        return 0;

    if (st.st_mode & S_IFREG)
        return 1;

    return 0;
}

/**
 * duplicate_characters - duplicates characters from a string
 * @path_string: the string to duplicate characters from
 * @start: starting index
 * @stop: stopping index
 * Return: pointer to the new buffer
 */
char *duplicate_characters(char *path_string, int start, int stop)
{
    static char buffer[1024];
    int i = 0, k = 0;

    for (k = 0, i = start; i < stop; i++)
    {
        if (path_string[i] != ':')
            buffer[k++] = path_string[i];
    }

    buffer[k] = 0;
    return buffer;
}

/**
 * find_command_path - finds the command in the PATH environment variable
 * @info: the shell_info struct
 * @path_string: the PATH environment variable
 * @command: the command to find
 * Return: full path of command if found, or NULL if not found
 */
char *find_command_path(shell_info *info, char *path_string, char *command)
{
    int i = 0, curr_pos = 0;
    char *path;

    if (!path_string)
        return NULL;

    if ((_strlen(command) > 2) && starts_with(command, "./"))
    {
        if (is_executable_command(info, command))
            return command;
    }

    while (1)
    {
        if (!path_string[i] || path_string[i] == ':')
        {
            path = duplicate_characters(path_string, curr_pos, i);
            if (!*path)
                _strcat(path, command);
            else
            {
                _strcat(path, "/");
                _strcat(path, command);
            }

            if (is_executable_command(info, path))
                return path;

            if (!path_string[i])
                break;

            curr_pos = i;
        }
        i++;
    }

    return NULL;
}
