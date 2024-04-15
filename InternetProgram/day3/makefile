src = $(wildcard *.cpp)
obj = $(patsubst %.cpp, %.o, $(src))

all: poll

poll: poll.o wrap.o
	g++ poll.cpp wrap.cpp -o poll -Wall

%.o: %.cpp
	g++ -c $< -Wall

.PHONY: clean all
clean:
	-rm -rf poll $(obj)
