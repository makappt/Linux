src = $(wildcard *.cpp)
obj = $(patsubst %.cpp, %.o, $(src))

all: epoll_ET

epoll_ET: epoll_ET.o wrap.o
	g++ epoll_ET.cpp wrap.cpp -o epoll_ET -Wall

# client: mult_thread_client.o wrap.o
#	g++ mult_thread_client.o wrap.o -o client -Wall

%.o: %.cpp
	g++ -c $< -Wall

.PHONY: clean all
clean:
	-rm -rf epoll_ET $(obj)
