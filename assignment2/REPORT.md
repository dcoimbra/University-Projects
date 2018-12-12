# ES Report - Assignment 2

## Question 1

All of the functions need information from the instances of the classes they are defined in.
The function state() in FileSystemState has to be able to read
state in order to access its contents when making operations to the file system.
Functions in FileSystem need to be able to access 

## Question 2

Because all states must be derived from analyzing the file system. They can not be created by a constructor or it would be possible to define
states that do not correspond to reality.
The representation of a file system must also only be modified when applying operations on an already existing state; specifically, when
creating/writing to files.

## Question 3

Semantically, preconditions in main limit the set of entities that can be read and affected by the program, since they're necessarily applied to main's inputs, which will be the arguments passed in the command line.