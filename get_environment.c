#include "shell.h"

/**
 * get_environment_strings - returns a copy of the environment strings
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * Return: String array containing environment variables and their values
 */
char **get_environment_strings(shell_info *info)
{
    if (!info->environ || info->env_changed)
    {
        info->environ = list_to_strings(info->env);
        info->env_changed = 0;
    }

    return info->environ;
}

/**
 * remove_environment_variable - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @variable: the environment variable to be removed
 * Return: 1 on successful removal, 0 otherwise
 */
int remove_environment_variable(shell_info *info, char *variable)
{
    list_t *node = info->env;
    size_t i = 0;
    char *p;

    if (!node || !variable)
        return 0;

    while (node)
    {
        p = starts_with(node->str, variable);
        if (p && *p == '=')
        {
            info->env_changed = delete_node_at_index(&(info->env), i);
            i = 0;
            node = info->env;
            continue;
        }
        node = node->next;
        i++;
    }
    return info->env_changed;
}

/**
 * set_environment_variable - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @variable: the environment variable to be set or modified
 * @value: the value to be associated with the environment variable
 * Return: Always 0
 */
int set_environment_variable(shell_info *info, char *variable, char *value)
{
    char *buf = NULL;
    list_t *node;
    char *p;

    if (!variable || !value)
        return 0;

    buf = malloc(_strlen(variable) + _strlen(value) + 2);
    if (!buf)
        return 1;
    custom_strcpy(buf, variable);
    _strcat(buf, "=");
    _strcat(buf, value);
    node = info->env;
    while (node)
    {
        p = starts_with(node->str, variable);
        if (p && *p == '=')
        {
            free(node->str);
            node->str = buf;
            info->env_changed = 1;
            return 0;
        }
        node = node->next;
    }
    add_node_end(&(info->env), buf, 0);
    free(buf);
    info->env_changed = 1;
    return 0;
}
