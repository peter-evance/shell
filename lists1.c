#include "shell.h"

/**
 * list_length - determines the length of a linked list
 * @head: pointer to the first node
 *
 * Return: size of the list
 */
size_t list_length(const list_t *head)
{
    size_t i = 0;

    while (head)
    {
        head = head->next;
        i++;
    }
    return i;
}

/**
 * list_to_string_array - returns an array of strings from the list
 * @head: pointer to the first node
 *
 * Return: array of strings
 */
char **list_to_string_array(list_t *head)
{
    list_t *node = head;
    size_t i = list_length(head), j;
    char **strings;
    char *str;

    if (!head || !i)
        return NULL;
    strings = malloc(sizeof(char *) * (i + 1));
    if (!strings)
        return NULL;
    for (i = 0; node; node = node->next, i++)
    {
        str = malloc(_strlen(node->str) + 1);
        if (!str)
        {
            for (j = 0; j < i; j++)
                free(strings[j]);
            free(strings);
            return NULL;
        }

        str = custom_strcpy(str, node->str);
        strings[i] = str;
    }
    strings[i] = NULL;
    return strings;
}

/**
 * print_list - prints all elements of a list_t linked list
 * @head: pointer to the first node
 *
 * Return: size of the list
 */
size_t print_list(const list_t *head)
{
    size_t i = 0;

    while (head)
    {
        _puts(convert_number(head->num, 10, 0));
        _putchar(':');
        _putchar(' ');
        _puts(head->str ? head->str : "(nil)");
        _puts("\n");
        head = head->next;
        i++;
    }
    return i;
}

/**
 * find_node_starts_with - finds a node whose string starts with a prefix
 * @node: pointer to the list head
 * @prefix: prefix to match
 * @c: the next character after the prefix to match
 *
 * Return: matching node or NULL
 */
list_t *find_node_starts_with(list_t *node, char *prefix, char c)
{
    char *p = NULL;

    while (node)
    {
        p = starts_with(node->str, prefix);
        if (p && ((c == -1) || (*p == c)))
            return node;
        node = node->next;
    }
    return NULL;
}

/**
 * get_node_index - gets the index of a node in a linked list
 * @head: pointer to the list head
 * @node: pointer to the node
 *
 * Return: index of the node or -1 if not found
 */
ssize_t get_node_index(list_t *head, list_t *node)
{
    size_t i = 0;

    while (head)
    {
        if (head == node)
            return i;
        head = head->next;
        i++;
    }
    return -1;
}
