SOURCES = clique.c cmdLineParameters.c fileIO.c hashTable.c main.c murmur3.c
OBJECTS = clique.o cmdLineParameters.o fileIO.o hashTable.o main.o murmur3.o
EXECUTABLE = clique
CC = gcc
FLAGS = -Wall -g -c -O2

all: $(OBJECTS)
	$(CC) -o $(EXECUTABLE) clique.o cmdLineParameters.o fileIO.o hashTable.o main.o murmur3.o

clique.o: clique.c
	$(CC) $(FLAGS) clique.c

cmdLineParameters.o: cmdLineParameters.c
	$(CC) $(FLAGS) cmdLineParameters.c

fileIO.o: fileIO.c
	$(CC) $(FLAGS) fileIO.c

hashTable.o: hashTable.c
	$(CC) $(FLAGS) hashTable.c

main.o: main.c
	$(CC) $(FLAGS) main.c

murmur3.o: murmur3.c
	$(CC) $(FLAGS) murmur3.c

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)