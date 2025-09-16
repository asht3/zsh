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

int compare_alphabet(const void* file_1, const void* file_2) {
    const char* f1 = (const char*) file_1;
    const char* f2 = (const char*) file_2;
    
    // Extract just the filename
    const char* name1 = my_strchr(f1, '/');
    name1 = name1 ? name1 + 1 : f1;
    
    const char* name2 = my_strchr(f2, '/');
    name2 = name2 ? name2 + 1 : f2;
    
    if (name1[0] == '.' && name2[0] == '.') {
        return my_strcmp(name1, name2);
    }
    else if (name1[0] == '.' && name2[0] != '.') {
        return -1;
    } else if (name2[0] == '.' && name1[0] != '.') {
        return 1;
    }
    
    return my_strcmp(name1, name2);
}

int compare_mod_times(const void* file_1, const void* file_2) {
    if (file_1 == NULL || file_2 == NULL) {
        return 0;
    }

    const char* f1 = (const char*) file_1;
    const char* f2 = (const char*) file_2;

    struct stat stat_f1, stat_f2;

    if (stat(f1, &stat_f1) != 0) return -1;

    if (stat(f2, &stat_f2) != 0) return -1;

    if (stat_f1.st_mtime < stat_f2.st_mtime) {
        return 1; // file 2 is more recent
    }
    else if (stat_f1.st_mtime > stat_f2.st_mtime) {
        return -1; // file 1 is more recent
    }
    else {
        return my_strcmp(f1, f2); // if times are equal, compare alphabetically
    }

    return 0;
}

void swap(void** arr, int i, int j) {
    char* temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

int partition(void** arr, int low, int high, int (*compare)(const void *, const void *)) {
    char* pivot = (char*) arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (compare(arr[j], pivot) < 0) {
            i++;
            swap(arr, i, j);
        }
    }

    swap(arr, i + 1, high);
    return i + 1;
}

void quicksort(void** arr, int low, int high, int (*compare)(const void *, const void *)) {
    if (low < high) {
        int pi = partition(arr, low, high, compare);
        quicksort(arr, low, pi - 1, compare);
        quicksort(arr, pi + 1, high, compare);
    }
}

int num_to_str(char *buffer, long num) {
    if (num == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return 1;
    }
    
    int digits = 0;
    long temp = num;
    
    if (temp < 0) {
        buffer[digits++] = '-';
        temp = -temp;
    }
    
    // Count digits
    long n = temp;
    int num_digits = 0;
    do {
        num_digits++;
        n /= 10;
    } while (n > 0);
    
    // Convert to string
    for (int i = num_digits - 1; i >= 0; i--) {
        buffer[digits + i] = '0' + (temp % 10);
        temp /= 10;
    }
    digits += num_digits;
    buffer[digits] = '\0';
    
    return digits;
}