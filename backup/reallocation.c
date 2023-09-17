#include "shell.h"

/**
 * custom_memset - fills memory with a constant byte
 * @ptr: the pointer to the memory area
 * @value: the byte to fill *ptr with
 * @num_bytes: the number of bytes to be filled
 * Return: a pointer to the memory area ptr
 */
char *custom_memset(char *ptr, char value, unsigned int num_bytes)
{
    unsigned int i;

    for (i = 0; i < num_bytes; i++)
        ptr[i] = value;
    return ptr;
}

/**
 * free_string_array - frees a string array and its elements
 * @strings: the string array to be freed
 */
void free_string_array(char **strings)
{
    char **temp = strings;

    if (!strings)
        return;

    while (*strings)
        free(*strings++);

    free(temp);
}

/**
 * custom_realloc - reallocates a block of memory
 * @ptr: pointer to previous malloc'ed block
 * @old_size: byte size of the previous block
 * @new_size: byte size of the new block
 * Return: pointer to the reallocated block
 */
void *custom_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
    char *p;

    if (!ptr)
        return malloc(new_size);
    if (!new_size)
        return free(ptr), NULL;
    if (new_size == old_size)
        return ptr;

    p = malloc(new_size);
    if (!p)
        return NULL;

    old_size = old_size < new_size ? old_size : new_size;
    while (old_size--)
        p[old_size] = ((char *)ptr)[old_size];
    free(ptr);
    return p;
}
