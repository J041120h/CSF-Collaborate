CXX = g++
CXXFLAGS = -g -Wall -std=c++17 -Wextra -pedantic


csim: reader.o csim.o main.o
	$(CXX) $(CXXFLAGS) -o csim reader.o csim.o main.o

reader.o: reader.cpp reader.h
	$(CXX) $(CXXFLAGS) -c reader.cpp

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
