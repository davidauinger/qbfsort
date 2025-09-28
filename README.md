This project provides algorithms to sort Quantified Boolean Formulas (QBF) with respect to certain metrics. 

# Compilation

A `CMakeLists.txt` file is provided with the project. To compile with cmake, run the following:
```
mkdir build
cd build
cmake ..
make
```
An executable file `qbfsort` should then be in the build directory.

# Usage

Both the input QBF and the output QBF are in the [QDIMACS standard](https://www.qbflib.org/qdimacs.html). By default, the program reads the input QBF from stdin and writes the output QBF to stdout. Of course, these can be redirected to files, so a typical call might look like:
```
./qbfsort < input > output
```

The program supports three kinds of sorting: literals, clauses and quantifiers. For each sorting the metric can be specified as key-value pair on the command line (`sortLiterals=<metric>`, `sortClauses=<metric>`, `sortQuantifiers=<metric>`). Currently, only two metrics are supported for each kind of sorting: `basic` (canonical order of the labels) and `countedBinaries` (by the number of newly created binary clauses). These metrics can be set for each kind of sorting separately. For example, to take the QBF from `example.cnf`, sort the literals by the number of newly created binary clauses, do not sort the clauses, sort the quantifiers by the canonical order and write the resulting QBF to `output.cnf`, the command may look like:
```
./qbfsort sortLiterals=countedBinaries  sortQuantifiers=basic < example.cnf > output.cnf
```
