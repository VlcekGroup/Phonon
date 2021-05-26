all: first second

first: readmatdyn.o
	g++ -o first readmatdyn.o

second: MC.o
	g++ -o second MC.o

readmatdyn.o: readmatdyn.cpp
	g++ -c readmatdyn.cpp

MC.o: MC.cpp
	g++ -c MC.cpp

clean:
	rm -f *.o first second