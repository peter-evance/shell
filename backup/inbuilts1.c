#include "shell.h"

/**
 * display_history - displays the history list, one command by line, preceded
 *                   with line numbers, starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int display_history(shell_info *info)
{
    print_list(info->history);
    return (0);
}

/**
 * remove_alias - removes an alias by name
 * @info: parameter struct
 * @alias_name: the alias name
 *
 * Return: Always 0 on success, 1 on error
 */
int remove_alias(shell_info *info, char *alias_name)
{
    char *p, c;
    int ret;

    p = _strchr(alias_name, '=');
    if (!p)
        return (1);
    c = *p;
    *p = 0;
    ret = delete_node_at_index(&(info->alias),
                               get_node_index(info->alias, node_starts_with(info->alias, alias_name, -1)));
    *p = c;
    return (ret);
}

/**
 * add_alias - adds or updates an alias
 * @info: parameter struct
 * @alias_str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int add_alias(shell_info *info, char *alias_str)
{
    char *p;

    p = _strchr(alias_str, '=');
    if (!p)
        return (1);
    if (!*++p)
        return (remove_alias(info, alias_str));

    remove_alias(info, alias_str);
    return (add_node_end(&(info->alias), alias_str, 0) == NULL);
}

/**
 * print_single_alias - prints a single alias string
 * @alias_node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int print_single_alias(list_t *alias_node)
{
    char *p = NULL, *alias_str = NULL;

    if (alias_node)
    {
        p = _strchr(alias_node->str, '=');
        for (alias_str = alias_node->str; alias_str <= p; alias_str++)
            _putchar(*alias_str);
        _putchar('\'');
        _puts(p + 1);
        _puts("'\n");
        return (0);
    }
    return (1);
}

/**
 * manage_alias - mimics the alias builtin (man alias)
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int manage_alias(shell_info *info)
{
    int i = 0;
    char *p = NULL;
    list_t *alias_node = NULL;

    if (info->argc == 1)
    {
        alias_node = info->alias;
        while (alias_node)
        {
            print_single_alias(alias_node);
            alias_node = alias_node->next;
        }
        return (0);
    }
    for (i = 1; info->argv[i]; i++)
    {
        p = _strchr(info->argv[i], '=');
        if (p)
            add_alias(info, info->argv[i]);
        else
            print_single_alias(node_starts_with(info->alias, info->argv[i], '='));
    }

    return (0);
}
