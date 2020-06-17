# Resources
Data Structures, Algorithms, Utility Classes

- C++ Resources compile with g++ in C++11, C++14, and C++17  
- Java Resources compile in Java 8, Java 11  
- Kotlin Resources compile in Kotlin 1.3  

### Currently being refactored
- 79 character line limit  
- 2 spaces for indentation  
- very compressed coding style to reduce wasted space in pdf, with multiple
statements on a single line  
- `if`, `else`, `for`, `while`, etc ... statements should either be immediately
followed by a single statement and a newline, a block wrapped in curly braces
followed by a newline, or only a newline
- otherwise, mostly adhering to
[Google's Style Guide](https://google.github.io/styleguide/cppguide.html)
- templates should either be a standalone function, or a struct/class
- functions and classes should have generic types if possible
- classes should have constant parameters passed as template parameters if
possible, variable paremeters should be passed in the constructor
- classes and functions should have a quick summary, descriptions of
member functions and fields if not obvious, specify conventions used,
mention the constant factor (roughly) relative to its advertised time
complexity,  time complexity and memory complexity,
and specify where it was tested
- if template parameters are non-trivial, those should be described as well,
possibly with an example provided
