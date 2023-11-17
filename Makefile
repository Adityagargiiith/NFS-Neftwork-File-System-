CC := gcc
CFLAGS := -g

# Path: command

all: client naming_server storage_server

client:
	$(CC) $(CFLAGS) -o client client.c command/*.c

naming_server:
	$(CC) $(CFLAGS) -o naming_server naming_server.c nmutils/*.c

storage_server:
	$(CC) $(CFLAGS) -o storage_server storage_server.c ssutils/*.c

clean:
	rm -f client naming_server storage_server
