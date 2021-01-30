main: main.o dictionary.o
	g++ main.o dictionary.o -o main

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

dictionary.o: dictionary.cpp
	g++ -std=c++11 -c dictionary.cpp

clean:
	rm *.o
