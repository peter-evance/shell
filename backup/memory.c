#include "shell.h"

/**
 * free_pointer_and_nullify - frees a pointer and sets it to NULL
 * @ptr: Address of the pointer to free
 *
 * Return: 1 if the pointer was freed, otherwise 0.
 */
int free_pointer_and_nullify(void **ptr)
{
    if (ptr && *ptr)
    {
        free(*ptr);
        *ptr = NULL;
        return 1;
    }
    return 0;
}
