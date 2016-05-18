CC	= g++
CXX	= -c
OP	= -Wall
NAME	= -o

All: greeting.cc myprint.o
	$(CC) *.o greeting.cc $(NAME) main

myprint.o: myprint.h myprint.cc
	$(CC) $(CXX) myprint.h myprint.cc

clean:
	rm -f *o *.h.gch main

#old vision
#All: greeting.cc myprint.o
#	g++ *.o greeting.cc -o main
#
#myprint.o: myprint.h myprint.cc
#	g++ -c myprint.h myprint.cc
#
#clean:
#	rm -f *.o *.h.gch main
