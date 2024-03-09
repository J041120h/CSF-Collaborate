CXX = g++
CXXFLAGS = -g -Wall -std=c++17 -Wextra -pedantic


csim: csim.o main.o
	$(CXX) $(CXXFLAGS) -o csim csim.o main.o

csim.o: csim.cpp csim.h
	$(CXX) $(CXXFLAGS) -c csim.cpp

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp


.PHONY: solution.zip
solution.zip :
	rm -f $@
	zip -9r $@ *.c *.cpp *.h README.txt Makefile

clean :
	rm -f csim *.o
