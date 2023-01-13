# NACC
NACC Is Not A C Compiler -- pronounced as knack.

This is the NACC programming language. It's a simple toy language that I am building for fun. It consists of a compiler that compiles source code into assembly language, an assembler that creates a binary file from the assembly and a virtual machine that runs the binary. 

NACC is a single pass compiler. Symbols must be defined before they are used. Comments start with a ";" and end at the end of the line. Keywords are not case sensitive.

## Not supported features
NACC is a tiny subset of a "real" programming language.
- Pointers
- Data strucutres

## Data types and variables 
There is no notion of a data structure. Global variables as well as function locals are supported. Variables may be initialized when they are defined or not. There is no notion of a pointer or memory allocation. Function pointers are not supported. Type casting is supported in situations where it's a good idea.
- number - All numbers are double precision floats. There is no provision for bitwise operations.
- string - Strings are formatted using {} characters embedded in the string. Strings enclosed in double quotes are formartted and strings enclosed in single quotes are literal. There is no notion of a character. 
- boolean - Boolean values can have 2 states. True and False.
- nothing - This type is only used for functions that do not return an actual value. Variables cannot be of type nothing.

## Functions
A function is a block of executable code that accepts zero or more inputs and has exactly one output. A function reference can be in an expression. The syntax for defining and calling a function is basically the same as the C syntax.

## Expressions 
Both boolean, numeric, and string expressions are supported and expressions can accept any data type by syntax, but semantacally, all of the types in an expression must be the same. A function reference can be a part of an expression as long as the return type matches. 

For constructs that require a boolean expression, an empty or blank expression is considered to be true. For example ```while {}``` and ```while(true) {}``` are symantically identical. 

## Examples

General structure of a program.
```C
; this is a comment

number var1 ; define a var uninitialized

; simple function definition
number func1(number v) {
  return(v + 10)
}

; This simply marks the beginning of the program. 
start {
  
  var1 = func1(20)
  print(func1(var1))
}

```

General syntax of statements.

```C

string s = "a string initializer"
number n = 10
Number x ; keywords are not case sensitive
boolean b = true

; assuming the variables n and b have not changed,
; this string prints out as "for bool ops, the number 10 is true"
string s1 = "for bool ops, the number {n} is {b}\n"

; nothing function { is semantically identical
; cannot return a value 
nothing function() {

  ; while(true) { is semantically identical
  while {
    nUmBeR p = x
    break ; keyword
  }
  
  ; comparison by value
  if ( x == 10 ) { ; curly braces are always required
    ; not, and, or are keywords
    if(x == n and "this is a string" == s) {
      do_some_stuff(s)
    }
    ; there is no "else if()" or "elif()" clause. 
    else(x <= 10) {
      do_something else()
    }
    ; always true
    else {
      ; if x is > 10
      or_do_this()
    }
  }
}

```
## Keywords
- string - names a variable of type string
- number - names a variable of type number
- boolean - names a variable of type boolean
- nothing - names a function with no return type
- if - introduces an if(){} clause
- else - introduces an else(){} cleause
- while - introdues a while(){} clause
- do - introduces a do{}while() clause
- and - logical and operation in boolean expression
- or - logical or operation in boolean expression
- not - logical not in boolean expression
- continue - cause the loop to jump to the top
- break - break out of a loop
- start - mark the entry point into a program
- return - mark an exit point in a function
- exit - mark the exit point of a program
- print - print the given expression to stdout
- trace - print trace information to stderr
