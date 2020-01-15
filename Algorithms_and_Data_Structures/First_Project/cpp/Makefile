CC=g++
BOOST=/usr/local/boost_1_68_0 # change this based on your Boost installation
CFLAGS= -g -I $(BOOST) -std=c++17 -o proj1 -pedantic -Wall -Wextra -Werror


proj1: proj1.cpp airport.cpp system.cpp
	$(CC) $(CFLAGS) proj1.cpp airport.cpp system.cpp 

clean: proj1
	rm proj1
