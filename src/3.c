#include "../common/file_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Find maximum joltage for a bank using greedy approach
int find_max_joltage(const char *line) {
  int len = strlen(line);
  if (len < 2) {
    return 0;
  }

  // Find the maximum digit in the entire line
  char max_digit = '0';
  for (int i = 0; i < len; i++) {
    if (line[i] > max_digit) {
      max_digit = line[i];
    }
  }

  int max_joltage = 0;

  // Try each occurrence of the max digit as the tens place
  for (int i = 0; i < len - 1; i++) {
    if (line[i] == max_digit) {
      // Find the largest digit after position i
      char max_after = '0';
      for (int j = i + 1; j < len; j++) {
        if (line[j] > max_after) {
          max_after = line[j];
        }
      }

      int joltage = (line[i] - '0') * 10 + (max_after - '0');
      if (joltage > max_joltage) {
        max_joltage = joltage;
      }
    }
  }

  // Edge case: what if max_digit is only at the last position?
  // Then we need to consider second-max digit as tens place
  // Find second maximum digit
  char second_max = '0';
  for (int i = 0; i < len; i++) {
    if (line[i] != max_digit && line[i] > second_max) {
      second_max = line[i];
    }
  }

  // Try second_max as tens place with max_digit after it
  for (int i = 0; i < len - 1; i++) {
    if (line[i] == second_max) {
      // Find the largest digit after position i
      char max_after = '0';
      for (int j = i + 1; j < len; j++) {
        if (line[j] > max_after) {
          max_after = line[j];
        }
      }

      int joltage = (line[i] - '0') * 10 + (max_after - '0');
      if (joltage > max_joltage) {
        max_joltage = joltage;
      }
    }
  }

  return max_joltage;
}

// Part 2: Find maximum 12-digit joltage by picking 12 batteries
long long find_max_12_digit_joltage(const char *line) {
  int len = strlen(line);
  if (len < 12) {
    return 0;
  }

  char result[13]; // 12 digits + null terminator
  result[12] = '\0';

  int current_idx = 0;

  // For each of the 12 positions in our result
  for (int position = 0; position < 12; position++) {
    int remaining_needed = 12 - position - 1;
    int search_end = len - remaining_needed;

    // Find the largest digit we can pick for this position
    char max_digit = '0';
    int max_idx = current_idx;

    for (int i = current_idx; i < search_end; i++) {
      if (line[i] > max_digit) {
        max_digit = line[i];
        max_idx = i;
      }
    }

    result[position] = max_digit;
    current_idx = max_idx + 1;
  }

  return atoll(result);
}

typedef struct {
  long long part1_total;
  long long part2_total;
} JoltageState;

void process_line(const char *line, int line_number, void *user_data) {
  JoltageState *state = (JoltageState *)user_data;

  if (strlen(line) == 0) {
    return;
  }

  int max_joltage = find_max_joltage(line);
  state->part1_total += max_joltage;

  long long max_12_joltage = find_max_12_digit_joltage(line);
  state->part2_total += max_12_joltage;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
    return 1;
  }

  const char *filename = argv[1];

  JoltageState state = {0, 0};

  int num_lines = read_file_lines(filename, process_line, &state);

  if (num_lines < 0) {
    return 1;
  }

  printf("Part 1: %lld\n", state.part1_total);
  printf("Part 2: %lld\n", state.part2_total);

  return 0;
}
