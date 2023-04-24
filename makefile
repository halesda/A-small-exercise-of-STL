CC = g++
CFLAGS = -w -O2 -g -std=c++2a 


FILES = ./test


SOURCES = ./test.cpp


All : $(FILES)


test : test.cpp
	$(CC) $(CFLAGS) $^ -o $@

clean :
	rm -f $(FILES)
