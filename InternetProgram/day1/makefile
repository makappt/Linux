src = $(wildcard *.c)
obj = $(patsubst %.c, %.o, $(src))

all: server client

server: server.o wrap.o
	g++ server.o wrap.o -o server -Wall
client: client.o wrap.o
	g++ client.o wrap.o -o client -Wall

%.o:%.c
	g++ -c $< -Wall

.PHONY: clean all
clean: 
	-rm -rf server client $(obj)
