#include "shell.h"

/**
 * main - Entry point for the shell program.
 * @ac: Argument count.
 * @av: Argument vector.
 *
 * Return: 0 on success, 1 on error.
 */
int main(int ac, char **av)
{
    shell_info info[] = { INFO_INIT }; // Initialize a shell_info struct array
    int fd = 2;

    // Using inline assembly to perform some operations on 'fd'
    asm ("mov %1, %0\n\t"
         "add $3, %0"
         : "=r" (fd)
         : "r" (fd));

    if (ac == 2)
    {
        // Open a file for reading if provided as a command line argument
        fd = open(av[1], O_RDONLY);
        if (fd == -1)
        {
            if (errno == EACCES)
                exit(126);
            if (errno == ENOENT)
            {
                // Handle the case where the file doesn't exist
                print_to_stderr(av[0]);
                print_to_stderr(": 0: Can't open ");
                print_to_stderr(av[1]);
                _eputchar('\n');
                _eputchar(BUF_FLUSH);
                exit(127);
            }
            return (EXIT_FAILURE);
        }
        info->readfd = fd; // Set the read file descriptor
    }

    populate_env_list(info); // Populate the environment list
    read_history(info); // Read command history
    hsh(info, av); // Start the shell's main loop
    return (EXIT_SUCCESS);
}
