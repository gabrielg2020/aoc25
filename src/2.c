#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "common/file_utils.h"

// Part 1: Check if a number is invalid (made of a pattern repeated exactly twice)
bool is_invalid_id_part1(long long num) {
    char str[32];
    sprintf(str, "%lld", num);

    int len = strlen(str);

    // Odd length can't be repeated twice
    if (len % 2 != 0) {
        return false;
    }

    int half = len / 2;

    // Check if first half equals second half
    for (int i = 0; i < half; i++) {
        if (str[i] != str[i + half]) {
            return false;
        }
    }

    return true;
}

// Part 2: Check if a number is invalid (made of a pattern repeated at least twice)
bool is_invalid_id_part2(long long num) {
    char str[32];
    sprintf(str, "%lld", num);

    int len = strlen(str);

    // Try all possible pattern lengths from 1 to len/2
    for (int pattern_len = 1; pattern_len <= len / 2; pattern_len++) {
        // Check if the length is divisible by pattern_len
        if (len % pattern_len != 0) {
            continue;
        }

        // Check if the entire string is made of this pattern repeated
        bool is_repeating = true;
        for (int i = pattern_len; i < len; i++) {
            if (str[i] != str[i % pattern_len]) {
                is_repeating = false;
                break;
            }
        }

        if (is_repeating) {
            return true;
        }
    }

    return false;
}

typedef struct {
    long long part1_sum;
    long long part2_sum;
} SumState;

void process_line(const char* line, int line_number, void* user_data) {
    SumState* state = (SumState*)user_data;

    if (strlen(line) == 0) {
        return;
    }

    // Make a copy of the line since strtok modifies it
    char* line_copy = strdup(line);
    char* token = strtok(line_copy, ",");

    while (token != NULL) {
        // Parse range: start-end
        long long start, end;
        if (sscanf(token, "%lld-%lld", &start, &end) == 2) {
            // Check each number in the range
            for (long long num = start; num <= end; num++) {
                if (is_invalid_id_part1(num)) {
                    state->part1_sum += num;
                }
                if (is_invalid_id_part2(num)) {
                    state->part2_sum += num;
                }
            }
        }

        token = strtok(NULL, ",");
    }

    free(line_copy);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    const char* filename = argv[1];

    SumState state = {0, 0};

    int num_lines = read_file_lines(filename, process_line, &state);

    if (num_lines < 0) {
        return 1;
    }

    printf("Part 1: %lld\n", state.part1_sum);
    printf("Part 2: %lld\n", state.part2_sum);

    return 0;
}
