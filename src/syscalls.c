#include "../include/syscalls.h"
#include "../include/utils.h"
#include "../include/builtins.h"

void list_directory(char** args) {
    DIR* dir;
    struct dirent* entry;

    int a_flag = 0;
    int l_flag = 0;
    int t_flag = 0;
    char* target_path = ".";

    for (int i = 1; args[i] != NULL; i++) {
        if (args[i][0] == '-') {
            if (my_strchr(args[i], 'a') != NULL) a_flag = 1;
            if (my_strchr(args[i], 'l') != NULL) l_flag = 1;
            if (my_strchr(args[i], 't') != NULL) t_flag = 1;
        } else target_path = args[i];
    }


    dir = opendir(target_path);
    if (dir == NULL) perror("ls: failed to open directory");

    char* entries[MAX_INPUT_SIZE];
    int count = 0;
    char full_path[MAX_INPUT_SIZE];

    while ((entry = readdir(dir)) != NULL) {
        if (!a_flag && entry->d_name[0] == '.') {
            continue;
        }

        // Store full path for stat operations
        my_strcpy(full_path, target_path);
        if (target_path[my_strlen(target_path) - 1] != '/') {
            my_strcpy(full_path + my_strlen(full_path), "/");
        }
        my_strcpy(full_path + my_strlen(full_path), entry->d_name);
        
        entries[count] = malloc(my_strlen(full_path) + 1);
        my_strcpy(entries[count], full_path);
        count++;
    }
    entries[count] = NULL;
    closedir(dir);

    // Sort entries
    if (t_flag) {
        quicksort((void**)entries, 0, count - 1, compare_mod_times);
    } else {
        quicksort((void**)entries, 0, count - 1, compare_alphabet);
    }

    display_entries(entries, count, l_flag);
    if (!l_flag) write(STDOUT_FILENO, "\n", 1);
}

void display_entries(char** entries, int count, int l_flag) {
    for (int i = 0; i < count; i++) {
        char *filename = entries[i];
        // Extract just the filename from the full path
        char *last_slash = my_strchr(filename, '/');
        char *display_name = last_slash ? last_slash + 1 : filename;
        
        if (l_flag) {
            struct stat st;
            if (stat(filename, &st) == 0) {
                print_long_format(display_name, &st);
            }
        } else {
            write(STDOUT_FILENO, display_name, my_strlen(display_name));
            write(STDOUT_FILENO, "  ", 2);
        }
        
        free(entries[i]);
    }
}

void print_long_format(const char *filename, struct stat *st) {
    char buffer[512];
    int pos = 0;
    
    // File type and permissions
    buffer[pos++] = S_ISDIR(st->st_mode) ? 'd' : '-';
    buffer[pos++] = (st->st_mode & S_IRUSR) ? 'r' : '-';
    buffer[pos++] = (st->st_mode & S_IWUSR) ? 'w' : '-';
    buffer[pos++] = (st->st_mode & S_IXUSR) ? 'x' : '-';
    buffer[pos++] = (st->st_mode & S_IRGRP) ? 'r' : '-';
    buffer[pos++] = (st->st_mode & S_IWGRP) ? 'w' : '-';
    buffer[pos++] = (st->st_mode & S_IXGRP) ? 'x' : '-';
    buffer[pos++] = (st->st_mode & S_IROTH) ? 'r' : '-';
    buffer[pos++] = (st->st_mode & S_IWOTH) ? 'w' : '-';
    buffer[pos++] = (st->st_mode & S_IXOTH) ? 'x' : '-';
    buffer[pos++] = ' ';
    
    // Number of links
    pos += num_to_str(&buffer[pos], st->st_nlink);
    buffer[pos++] = ' ';
    
    // User ID (numeric)
    pos += num_to_str(&buffer[pos], st->st_uid);
    buffer[pos++] = ' ';
    
    // Group ID (numeric)  
    pos += num_to_str(&buffer[pos], st->st_gid);
    buffer[pos++] = ' ';
    
    // File size
    pos += num_to_str(&buffer[pos], st->st_size);
    buffer[pos++] = ' ';
    
    // Filename
    my_strcpy(&buffer[pos], filename);
    pos += my_strlen(filename);
    buffer[pos++] = '\n';
    
    write(STDOUT_FILENO, buffer, pos);
}

void concat_files(char **args) {
    if (args[1] == NULL) {
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