#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdio.h>
#include <stdlib.h>

// Read entire file into a dynamically allocated buffer
// Returns NULL on error
// Caller is responsible for freeing the returned buffer
char* read_file(const char* filename, size_t* out_size) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size < 0) {
        perror("Failed to get file size");
        fclose(file);
        return NULL;
    }

    // Allocate buffer (with extra byte for null terminator)
    char* buffer = (char*)malloc(file_size + 1);
    if (!buffer) {
        perror("Failed to allocate memory");
        fclose(file);
        return NULL;
    }

    // Read file
    size_t bytes_read = fread(buffer, 1, file_size, file);
    buffer[bytes_read] = '\0';

    fclose(file);

    if (out_size) {
        *out_size = bytes_read;
    }

    return buffer;
}

// Read file line by line and call callback for each line
// Returns number of lines processed, -1 on error
typedef void (*line_callback)(const char* line, int line_number, void* user_data);

int read_file_lines(const char* filename, line_callback callback, void* user_data) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return -1;
    }

    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    int line_number = 0;

    while ((read = getline(&line, &len, file)) != -1) {
        // Remove trailing newline if present
        if (read > 0 && line[read - 1] == '\n') {
            line[read - 1] = '\0';
            if (read > 1 && line[read - 2] == '\r') {
                line[read - 2] = '\0';
            }
        }

        callback(line, line_number++, user_data);
    }

    free(line);
    fclose(file);

    return line_number;
}

#endif // FILE_UTILS_H
