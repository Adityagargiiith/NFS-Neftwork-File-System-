CC := gcc
CFLAGS := -g

# Path: command

all: command client server

command: command/*.c
	$(CC) $(CFLAGS) -c command/*.c


client:
	$(CC) $(CFLAGS) -o client client.c


server:
	$(CC) $(CFLAGS) -o server server.c

clean:
	rm -rf *.o client server command/*.o

