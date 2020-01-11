
CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Wextra
LDFLAGS = -lm -lws2_32

all: server.exe client.exe

%.o:%.c communication.h
	$(CC) $(CFLAGS) -c -o $@ $<

%.exe: %.o communication.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)