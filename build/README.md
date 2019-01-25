# Prerequisites

- CMake
- Git
- GCC
- Doxygen (to build documentation)

# Building

Change the working directory to the this directory (build), and run the
following commands:

```
cmake ..
make
```

# Running the program

After building the program, there should be a program file called `puristin`
in the build directory root.

# Running the tests

The tests can be run using either `ctest` or the test executable
`puristin_test`.
