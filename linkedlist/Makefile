.SILENT:run clean

CC=clang++
CFLAGS=-std=c++20 -Wall -Wextra -Wpedantic -fsanitize=bounds -fsanitize=address

run: main
	./main

test: test_linked_list.cpp LinkedList.hpp
	$(CC) $(CFLAGS) $< -o $@
	./test

main: main.cpp LinkedList.hpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f main test
