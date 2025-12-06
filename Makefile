# Advent of Code 2025 Makefile

CC = gcc
CFLAGS = -Wall -Wextra -I.
LDFLAGS =

# Find all .c files in src/ and create corresponding targets
SOURCES = $(wildcard src/[0-9]*.c)
TARGETS = $(patsubst src/%.c,%,$(SOURCES))

.PHONY: all clean run list

all: $(TARGETS)

# Pattern rule: compile any src/number.c to executable number
%: src/%.c common/file_utils.h
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Run a specific day (usage: make run DAY=1)
run:
ifndef DAY
	@echo "Usage: make run DAY=<day_number>"
	@echo "Example: make run DAY=1"
else
	@if [ -f $(DAY) ]; then \
		./$(DAY) assets/$(DAY).txt; \
	else \
		echo "Day $(DAY) not compiled. Run 'make $(DAY)' first."; \
	fi
endif

# Clean all compiled binaries
clean:
	rm -f $(TARGETS)

# Show available days
list:
	@echo "Available days:"
	@for day in $(TARGETS); do echo "  $$day"; done
