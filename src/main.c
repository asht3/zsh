#include "../include/utils.h"
#include "../include/process.h"
#include <stdlib.h>

int main (int argc, char** argv, char** envp) {
    if (argc > 1) {
        write_error(argv[0]);
        return EXIT_FAILURE;
    }

    print_title();

    char* input = NULL;
    size_t input_len = 0;
    ssize_t read_size;
    while (1) {
        if (isatty(STDIN_FILENO)) {
            // write(STDOUT_FILENO, "my_zsh $> ", 10);
            printf("\033[1;36mmy_zsh $> \033[0m");
        }
        read_size = getline(&input, &input_len, stdin);

        if (read_size == -1) {
            break;
        }

        // Remove newline character
        if (input[read_size - 1] == '\n') {
            input[read_size - 1] = '\0';
        }

        // Parse command
        char** args = parse_input(input);

        // Execute command
        if (execute_command(args, envp) == 1) {
            free(args);
            break;
        }
        free(args);
    }
    free(input);

    return 0;
}

