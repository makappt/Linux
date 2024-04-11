src = $(wildcard *.cpp)
obj = $(patsubst %.cpp, %.o, $(src))

all: poll

server: poll.o wrap.o
	g++ select.o wrap.o -o server -Wall

# client: mult_thread_client.o wrap.o
#	g++ mult_thread_client.o wrap.o -o client -Wall

%.o: %.cpp
	g++ -c $< -Wall

.PHONY: clean all
clean:
	-rm -rf select $(obj)
