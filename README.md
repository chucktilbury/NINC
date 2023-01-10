# NINC
NINC Is Not C

This is the NINC programming language. It's a simple toy language that will help me learn how to the the packcc compiler generator. It consists of a compiler that compiles source code into assembly language, an assembler that creates a binary file from the assembly and a virtual machine that runs the binary. 

## Data types
- number - All numbers are double precision floats. There is no provision for bitwise operations.
- string - Strings are formatted using {} characters embedded in the string. Strings enclosed in double quotes are formartted and strings enclosed in single quotes are literal. There is no notion of a character. 
- boolean - Boolean values can have 2 states. True and False.
- nothing - This type is only used for functions that do not return an actual value. Variables cannot be of type nothing.

## Functions
Functions may return a value.
