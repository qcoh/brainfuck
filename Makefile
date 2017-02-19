CC = clang-5.0
WARNINGS = -Weverything -Wno-padded

SOURCE = brainfuck.c
EXECUTABLE = brainfuck

all:
	$(CC) $(WARNINGS) $(SOURCE) -o $(EXECUTABLE)

.PHONY: clean

clean:
	rm -f $(EXECUTABLE)
