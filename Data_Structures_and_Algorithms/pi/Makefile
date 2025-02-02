CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic
LDFLAGS = -lm
EXEC = mathlib-test
OBJS = bbp.o mathlib-test.o e.o newton.o viete.o euler.o madhava.o

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $(EXEC) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(EXEC) $(OBJS)
format:
	clang-format -i -style=file *.[ch]
