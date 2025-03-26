CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: stream_encryption

programme: stream_encryption.c
	$(CC) $(CFLAGS) -o stream_encryption stream_encryption.c

clean:
	rm -f stream_encryption

.PHONY: all clean