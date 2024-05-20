CC = g++
CFLAGS =

all: compile run

compile: build src/main.cpp
	$(CC) $(CFLAGS) src/main.cpp -o build/main.o

build:
	mkdir build

run: build/main.o
	build/main.o

clean:
	rmdir /s /q build

