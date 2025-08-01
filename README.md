# CppGP - A versatile Gaussian Process implementation in C++

CppGP is a Gaussian process library implemented in C++.

The framework is based on the Matlab toolbox [GPmat](https://github.com/SheffieldML/GPmat) by Neil Lawrence and others. However, it aims to be not a simple port to C++ but to implement new features like the ability to use derivatives as input data.

## Installation
```
mkdir build/
cd build/
cmake ..
make
```

## Notes:
- Data in matrices is given row-wise, i.e.,
    ```
    [ [x1(1), x1(2), x1(3), ...];
        [x2(1), x2(2), x2(3), ...];
        [x3(1), x3(2), x3(3), ...]
        ... ]
    ```
    Like that, it is possible to easily add new data points in text files by simpling adding a new line at the end of the file.
- By default, vectors are column vectors.
- Data in text files that contain a single variable is separated by spaces and newlines only, **not** by commas and semicolons.



## Disclaimer
This project has been created primarily for self-educational purposes and is not intended to be used in practice.
