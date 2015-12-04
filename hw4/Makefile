CC = gcc
CFLAGS = -Wall -fstack-protector -Wextra -Wunreachable-code -O -std=c99 -D_XOPEN_SOURCE=700 -D_XOPEN_SOURCE_EXTENDED -lpthread

all: twitterTrend_server twitterTrend_client
	@echo "build successfully"

twitterTrend_server: message.o queue.o twitterTrend_server.o twitter_db.o server_utility.o
	$(CC) -o $@ $^ $(CFLAGS)

twitterTrend_client: message.o twitterTrend_client.o client_db.o client_utility.o
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONEY: clean
clean:
	@rm -f ./*.o
	@rm -f twitterTrend_server twitterTrend_client
	@rm -f ./res/PA4_released/*.result
	@rm -f ./res/PA4_released/*.temp
