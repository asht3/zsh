#include "../include/utils.h"
#include "../include/process.h"
#include <stdlib.h>

int main (int argc, char** argv, char** envp) {
    if (argc > 1) {
        write_error(argv[0]);
        return EXIT_FAILURE;
    }

    char* input = NULL;
    size_t input_len = 0;
    ssize_t read_size;
    // int exit_code = 0; // debug
    while (1) {
        if (isatty(STDIN_FILENO)) {
            write(STDOUT_FILENO, "my_zsh $> ", 10);
            // if (exit_code) {
            //     printf("this should not be priting\n");
            // }
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
            // free(input);
            // exit(0);
            // exit_code = 1;
            break;
        }
        free(args);
    }
    free(input);

    return 0;
}

