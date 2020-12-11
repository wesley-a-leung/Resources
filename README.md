![build](https://github.com/wesley-a-leung/Resources/workflows/build/badge.svg)

# Resources
Data Structures, Algorithms, Utility Classes

- C++ Resources compile with g++ in C++11, C++14, and C++17  
- Java Resources compile in Java 8, Java 11  
- Kotlin Resources compile in Kotlin 1.3  

### Currently being refactored
- 79 character line limit  
- 2 spaces for indentation  
- very compressed coding style to reduce wasted space in PDF, with multiple
statements on a single line  
- `if`, `else`, `for`, `while`, etc ... statements should either be immediately
followed by a single statement and a newline, a block wrapped in curly braces
followed by a newline, a curly brace and a newline, or only a newline
- short functions and blocks can be squeezed onto a single line wrapped in
curly braces
- all lines should end in `\n`
- important variables and constants should be completely capitalized,
classes and filename should be upper camel case, other variables and functions
should be lower camel case, unless it is to match C++ STL conventions
- otherwise mostly adheres to
[Google's Style Guide](https://google.github.io/styleguide/cppguide.html)
- templates should either be a standalone function, or a struct/class
- functions and classes should have generic types if possible
- classes should have constant parameters passed as template parameters if
possible, variable parameters should be passed in the constructor
- classes and functions should have a quick summary, specify conventions used,
descriptions of template parameters, constructor arguments, member functions
and fields, mention the constant factor (roughly) relative to its advertised
time complexity, time complexity and memory complexity, and specify where it
was tested
- constant factors (very small, small, moderate, large, very large) roughly
differ by factors of 10
- if template parameters are non-trivial, those should be described as well,
possibly with an example provided
- `std::vector` is preferred to fixed-sized arrays, use `std::vector::reserve`
if possible
- the new operator should be avoided and memory should be allocated on the
stack, or `std::unique_ptr` should be used
- data structures should have at last one of the following constructors:
a constructor accepting a size variable as well as a function/lambda that
accepts no arguments and returns successive values for each index (see
`std::generate`), a constructor that accepts two iterators representing an 
half open range to be copied, and a constructor accepting a size variable and a
default value
