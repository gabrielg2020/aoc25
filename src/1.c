#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/file_utils.h"

typedef struct {
    int position;
    int part1_count;  // Count when dial ends at 0
    int part2_count;  // Count every time dial passes through 0
} DialState;

// Count how many times we pass through 0 during a rotation
int count_zeros_in_rotation(int start_pos, char direction, int distance) {
    int count = 0;

    if (direction == 'R') {
        // Moving right (increasing numbers)
        // Count: how many times do we hit position 0?
        // We hit 0 when: (start_pos + k) % 100 == 0 for k in [1, distance]
        // This happens when: start_pos + k = 100, 200, 300, ...
        // So: k = 100 - start_pos, 200 - start_pos, 300 - start_pos, ...

        // First time we hit 0: when k = (100 - start_pos) % 100
        // But if start_pos == 0, first hit is at k = 100, not k = 0 (we start at 0, don't count it)

        int first_zero;
        if (start_pos == 0) {
            first_zero = 100;  // Next time we hit 0 is after full loop
        } else {
            first_zero = 100 - start_pos;  // Distance to reach 0
        }

        // Count how many times we hit 0
        if (distance >= first_zero) {
            count = 1 + (distance - first_zero) / 100;
        }

    } else if (direction == 'L') {
        // Moving left (decreasing numbers)
        // We hit 0 when: (start_pos - k) % 100 == 0 for k in [1, distance]
        // This happens when: start_pos - k = 0, -100, -200, ...
        // So: k = start_pos, start_pos + 100, start_pos + 200, ...

        int first_zero;
        if (start_pos == 0) {
            first_zero = 100;  // Next time we hit 0 is after full loop backwards
        } else {
            first_zero = start_pos;  // Distance to reach 0
        }

        // Count how many times we hit 0
        if (distance >= first_zero) {
            count = 1 + (distance - first_zero) / 100;
        }
    }

    return count;
}

void process_line(const char* line, int line_number, void* user_data) {
    DialState* state = (DialState*)user_data;

    if (strlen(line) == 0) {
        return;
    }

    char direction = line[0];
    int distance = atoi(line + 1);

    // Part 2: Count all times we pass through 0 during rotation
    int zeros_during = count_zeros_in_rotation(state->position, direction, distance);
    state->part2_count += zeros_during;

    // Update position
    if (direction == 'L') {
        state->position = (state->position - distance) % 100;
        if (state->position < 0) {
            state->position += 100;
        }
    } else if (direction == 'R') {
        state->position = (state->position + distance) % 100;
    }

    // Part 1: Count only if we end at 0
    if (state->position == 0) {
        state->part1_count++;
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    const char* filename = argv[1];

    DialState state = {50, 0, 0};  // Start at position 50, both counts at 0

    int num_lines = read_file_lines(filename, process_line, &state);

    if (num_lines < 0) {
        return 1;
    }

    printf("Part 1: %d\n", state.part1_count);
    printf("Part 2: %d\n", state.part2_count);

    return 0;
}
