#include "../include/utils.h"

void write_error(const char* msg) {
    write(STDERR_FILENO, "Usage: ", 7);
    write(STDERR_FILENO, msg, my_strlen(msg));
    write(STDERR_FILENO, "\n", 1);
}

int my_strlen(const char* str_1) {
    int length = 0;

    while (*str_1 != '\0') {
        length++;
        str_1++;
    }

    return length;
}

char* my_strchr(const char* str, int search_char) {
    while (*str) { 
        if (*str == (char)search_char) {
            return (char*)str;
        }
        str++;
    }
    return NULL;
}

char* my_strtok(char* str, const char* delim) {
    static char* saved_ptr = NULL;
    if (str == NULL) {
        if (saved_ptr == NULL) {
            return NULL;
        }
        str = saved_ptr;
    }
    
    // Skip leading delimiters
    while (*str && my_strchr(delim, *str)) {
        str++;
    }
    
    if (*str == '\0') {
        saved_ptr = NULL;
        return NULL;
    }
    
    char* token_start = str;
    
    // Find the end of the token
    while (*str && !my_strchr(delim, *str)) {
        str++;
    }
    
    if (*str) {
        *str = '\0';
        saved_ptr = str + 1;
    } else {
        saved_ptr = NULL;
    }
    
    return token_start;
}

int my_strcmp(const char* str_1, const char* str_2) {
    while (*str_1 != '\0' || *str_2 != '\0') {
        if (*str_1 < *str_2) {
            return -1;
        }
        else if (*str_1 > *str_2) {
            return 1;
        }
        str_1++;
        str_2++;
    }
    return 0;
}

int my_strncmp(const char *str1, const char *str2, int n) {
    for (int i = 0; i < n; i++) {
        if (str1[i] == '\0' && str2[i] == '\0') {
            return 0;
        }
        if (str1[i] == '\0') {
            return -1;
        }
        if (str2[i] == '\0') {
            return 1;
        }
        if (str1[i] < str2[i]) {
            return -1;
        }
        if (str1[i] > str2[i]) {
            return 1;
        }
    }
    return 0;
}

char* my_strcpy(char* dest, const char* src) {
    char* original_dest = dest;
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return original_dest;
}
