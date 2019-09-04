# airport-manager

Simple graph implementation for an airport management system. 
Written in C for an Introduction to Algorithms and Data Structures class. Rewritten in C++ years later. 
Tested on Unix-like systems only.

See the [project description (in Portuguese)](https://github.com/HiveMindize/airport-graph/blob/master/P1-v1.3.pdf) for details and usage instructions.

The original implementation of this project received the maximum grade for its course.

Supports the following operations:

* Adding an airport (takes a 3-letter name and a maximum capacity)

* Changing a given airport's maximum capacity

* Adding/removing a single trip or a round trip between two airports

* Calculating the total number of flights between two airports

* Finding the airport with the most flights

* Finding the airport with the most connections to other airports

* Finding the most popular flight

* Closing/reopening an airport

* Listing all airports.

## Getting Started

The original project targets Unix-like operating systems only, and has been tested in such an environment.
It might work in other systems, but they are out of scope for the course this project was developed for.

### Prerequisites

To build the C version, you only need a compatible C compiler. This project was tested in GCC.

To build the C++ version, you need the Boost Graph Library installed in your system.

### Building

C version: `gcc proj1.c -o proj1`

C++ version: `make`

And run it with `./proj1`

## Built With

* [Boost Graph Library](https://www.boost.org/doc/libs/1_66_0/libs/graph/doc/index.html) - Used to implement an adjacency-list-based graph for the airport network (C++ version).

## Authors

### IAED 2015/2016 Group 25

* **David Coimbra** - *Original project group member and C++ rewrite* - [HiveMindize](https://github.com/HiveMindize)
* **Filipa Marques** - *Original project group member* - [filipaBit](https://github.com/filipaBit)
