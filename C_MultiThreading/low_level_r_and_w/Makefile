CC=clang
CFLAGS=-Wall -Wextra -Werror -g -std=c11 -pedantic

all: bobcat

bobcat: bobcat.c

debug: bobcat.c
	$(CC) $(CFLAGS) $< -o bobcat

.PHONY: format
format:
	clang-format -i bobcat.c

.PHONY: check-format
check-format:
	clang-format --dry-run --Werror bobcat.c

.PHONY: clean
clean:
	rm bobcat
