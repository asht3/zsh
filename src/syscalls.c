#include "../include/syscalls.h"
#include "../include/utils.h"
#include "../include/builtins.h"

void list_directory(char* path) {
    DIR* dir;
    struct dirent* entry;

    if (path == NULL) path = ".";

    dir = opendir(path);
    if (dir == NULL) perror("ls: failed to open directory");

    while ((entry = readdir(dir)) != NULL) {
        write(STDOUT_FILENO, entry->d_name, my_strlen(entry->d_name));
        write(STDOUT_FILENO, "\n", 1);
    }

    closedir(dir);
}

void concat_files(char **args) {
    if (args[1] == NULL) {
        // perror("cat: expected argument");
        write(STDERR_FILENO, "cat: expected argument\n", 22);
        return;
    }

    for (int i = 1; args[i] != NULL; i++) {
        int file_fd = open(args[i], O_RDONLY);
        if (file_fd < 0) {
            perror("cat: failed to open file");
            continue;
        }

        char buffer[MAX_INPUT_SIZE];
        ssize_t bytes_read;
        while ((bytes_read = read(file_fd, buffer, sizeof(buffer))) > 0) {
            write(STDOUT_FILENO, buffer, bytes_read);
        }
        close(file_fd);
    }
}

void display_file_head(char **args) {
    if (args[1] == NULL) {
        write(STDERR_FILENO, "head: expected argument\n", 24);
        return;
    }

    int file_fd = open(args[1], O_RDONLY);
    if (file_fd < 0) {
        perror("head: failed to open file");
        return;
    }

    char char_buf;
    ssize_t bytes_read;
    int line_count = 0;
    while ((bytes_read = read(file_fd, &char_buf, 1)) > 0) {
        write(STDOUT_FILENO, &char_buf, bytes_read);
        if (char_buf == '\n') {
            line_count++;
        }
        if (line_count == 10) break;
    }

    close(file_fd);
}

void display_file_tail(char **args) {
    if (args[1] == NULL) {
        write(STDERR_FILENO, "tail: expected argument\n", 24);
        return;
    }

    int file_fd = open(args[1], O_RDONLY);
    if (file_fd < 0) {
        perror("tail: failed to open file");
        return;
    }

    char ch;
    int total_lines = 0;
    int last_char = 0;
    
    while (read(file_fd, &ch, 1) > 0) {
        last_char = ch;
        if (ch == '\n') total_lines++;
    }
    
    // File does not end with a newline, count the last line
    if (last_char != '\n' && last_char != 0) {
        total_lines++;
    }
    close(file_fd);

    file_fd = open(args[1], O_RDONLY);
    int lines_to_skip = (total_lines > 10) ? total_lines - 10 : 0;
    int current_line = 0;
    int output_started = (lines_to_skip == 0) ? 1 : 0;

    while (read(file_fd, &ch, 1) > 0) {
        if (output_started) {
            write(STDOUT_FILENO, &ch, 1);
        }
        if (ch == '\n') {
            current_line++;
        }
        if (current_line == lines_to_skip) {
            output_started = 1;
        }
    }
    close(file_fd);    
}