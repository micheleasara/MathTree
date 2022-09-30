## What is this?
A parser of arithmetic expressions which generates an expression tree and computes its result. The software implements [Pratt parsing](https://journal.stuffwithstuff.com/2011/03/19/pratt-parsers-expression-parsing-made-easy/) under the hood.

It is divided into a main library, _MathTree_, which contains most of the code in this repo, and a driver program that you can use through a command line interface. The driver program will first ask for a (real numbered) arithmetic expression as its input, which it will then validate by attempting to point at any syntax errors. If no syntax errors are detected, the driver will parse the input and display the result of its computation on the screen. A descriptive message may be shown instead in case of runtime error. 

Round brackets and the following operators are supported:

\+ (addition), - (subtraction), * (multiplication), / (division), ^ (exponentiation), sqrt (square root), log (logarithm base 10) and log_n (logarithm base n).

## How do I build it? What about testing?
Ensure you have CMake 3.22 or above installed.

To build the driver program:
1) navigate to the _driver_ subdirectory;
2) create a folder that will contain the build (e.g. name it _build_);
3) navigate to the newly created folder;
4) enter ```cmake ..``` in your terminal (use ```cmake -DTESTS=OFF ..``` instead to avoid building the tests);
5) type ```cmake --build .```;
6) the executable _driver_ will be available in the build folder, whereas the library will be in _driver/lib_ and the tests in _driver/lib/tests_.

To run all the tests, simply navigate to their folder after building and type ```ctest```. Note that there is a dependency on GoogleTest, which will be downloaded during the build process.

To only build the library, repeat the steps above but:
1) navigate to the _MathTree_ subfolder instead of _driver_;
2) tests will be disabled by default, and you need to set the flag to ```ON``` to enable them;
3) remember you can use CMake's ```--config``` parameter if you wish to change the build mode to Release or similar.