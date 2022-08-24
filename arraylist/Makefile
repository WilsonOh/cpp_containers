.SILENT:run clean

CC=clang++
CFLAGS=-std=c++20 -Wall -Wextra -Wpedantic -fsanitize=bounds -fsanitize=address

run: main
	./main

test: test_array_list.cpp ArrayList.hpp
	$(CC) $(CFLAGS) $< -o $@
	./test
	rm test

main: main.cpp ArrayList.hpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f main test
