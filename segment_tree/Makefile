.PHONY: all clean
.SILENT: clean

CC=g++
CFLAGS=@../compile_flags.txt

all:
	@echo Please enter a target name
	@exit 1

%: %.cpp
	$(CC) $(CFLAGS) $< -lfmt -o $@
clean:
	fd -tx -xrm
