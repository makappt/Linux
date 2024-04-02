src = $(wildcard *.cpp)
obj = $(patsubst %.cpp, %.o, $(src))

all: server client

server: mult_thread_server.o wrap.o
	g++ mult_thread_server.o wrap.o -o server -Wall

client: mult_thread_client.o wrap.o
	g++ mult_thread_client.o wrap.o -o client -Wall

%.o: %.cpp
	g++ -c $< -Wall

.PHONY: clean all
clean:
	-rm -rf server client $(obj)
