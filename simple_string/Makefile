CC=gcc
CXX=g++
CFLAGS=-Wall -Wpedantic -g -fsanitize=bounds -fsanitize=address -std=c++17
BINS=test

test: test.o Range.o simple_string.o
	$(CXX) $(CFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) -c $(CFLAGS) $^

clean:
	rm -f $(BINS) *.o
