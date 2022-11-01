CC=g++
CFLAGS=-std=c++2a -Wall -Werror -O0

all: serial crcw crew

serial: main.o helpers.o serial.o
	$(CC) $(CFLAGS) $^ -o $@
	
crcw: main.o helpers.o crcw.o
	$(CC) $(CFLAGS) $^ -o $@ -lpthread
	
crew: main.o helpers.o crew.o
	$(CC) $(CFLAGS) $^ -o $@ -lpthread	

main.o: main.cpp
	$(CC) -c $(CFLAGS) $< -o $@
	
helpers.o: helpers.cpp
	$(CC) -c $(CFLAGS) $< -o $@
	
serial.o: serial.cpp
	$(CC) -c $(CFLAGS) $< -o $@
	
crew.o: crew.cpp
	$(CC) -c $(CFLAGS) $< -o $@ -lpthread
	
clean:
	rm -rf *.o serial crcw crew
