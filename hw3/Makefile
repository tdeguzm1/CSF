
CC = g++
CONSERVATIVE_FLAGS = -std=c++11 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g -O0
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)


csim: main.o cache.o set.o slot.o
	$(CC) -o csim main.o cache.o slot.o set.o

cexper: experiment.o cache.o set.o slot.o
	$(CC) -o cexper experiment.o cache.o slot.o set.o

experiment.o: experiment.cpp slot.h slot.cpp set.h set.cpp cache.h cache.cpp
	$(CC) -c experiment.cpp $(CFLAGS)

main.o: main.cpp slot.h slot.cpp set.h set.cpp cache.h cache.cpp
	$(CC) -c main.cpp $(CFLAGS)

slot.o: slot.cpp slot.h cache_stats.h
	$(CC) -c slot.cpp $(CFLAGS)

set.o: set.cpp set.h cache_stats.h
	$(CC) -c set.cpp $(CFLAGS)

cache.o: cache.cpp cache.h cache_stats.h
	$(CC) -c cache.cpp $(CFLAGS)

.PHONY: solution.zip
solution.zip :
	rm -f solution.zip
	zip solution.zip Makefile README.txt *.h *.cpp

.PHONY: clean all
clean:
	rm -f *.o csim