This project provides algorithms to sort Quantified Boolean Formulas (QBF) with respect to certain metrics. 

# Compilation

A `CMakeLists.txt` file is provided with the project. To compile with cmake, run the following:
```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```
An executable file `qbfsort` should then be in the build directory.

# Tests

The `CMakeLists.txt` file contains optional tests. The tests need to be copied to the build directory during configuration. To include the tests, run cmake with the variable `WITH_TESTS=ON`:
```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DWITH_TESTS=ON ..
make
```
The tests can be executed from the build directory:
```
ctest --output-on-failure
```

# Usage

Both the input QBF and the output QBF are in the [QDIMACS standard](https://www.qbflib.org/qdimacs.html). By default, the program reads the input QBF from stdin and writes the output QBF to stdout. Of course, these can be redirected to files, so a typical call might look like:
```
./qbfsort < input > output
```

The program supports three kinds of sorting: literals, clauses and quantifiers. For each sorting the metric can be specified as key-value pair on the command line (`sortLiterals=<metric>`, `sortClauses=<metric>`, `sortQuantifiers=<metric>`). These metrics can be set for each kind of sorting separately. For example, to take the QBF from `example.cnf`, sort the literals by the number of newly created binary clauses, do not sort the clauses, sort the quantifiers by the canonical order and write the resulting QBF to `output.cnf`, the command may look like:
```
./qbfsort sortLiterals=countedBinaries  sortQuantifiers=basic < example.cnf > output.cnf
```

# Sorting metrics

## basic

This metric sorts the QBF in the canonical order of the literals. That means, negative literals come first, where -1 is the highest negative literal, then comes 1 and the other positive literals in canonical order. This metric does not take into consideration the occurrences of variables in the formula, but is mainly intended for testing purposes and to provide a defined (unique) order of the formula.

For example in the clause `1 5 -3 0` the literals will be sorted to `-3 1 5 0`. Clauses are sorted according to their first literal first, then their second literal (if the first literal is equal) and so on. The literals in the clauses are compared in the order they appear in the clause, such that the clause `6 1 3 0` is less than the clause `7 -1 8 0`, even though the second clause has a literal less than any literal in the first clause. Be aware that if both literals and clauses are sorted, the sorting of literals comes first, which might affect the order of the clauses. Quantifier blocks are sorted by sorting the variables in their canonical order. A quantifier block `e 3 7 1 4 0` is sorted to `e 1 3 4 7 0`.

## frequency

This metric counts the (positive or negative) occurrences of variables in the formula and sorts the formula in descending order according to that frequencies. In other words, literals or variables of the most frequent variables come first.

When sorting literals within a clause, the sign of the literal is ignored. Clauses are compared according to the sum of frequencies of all variables. Be aware that adding an additional literal to a clause makes the clause less because the additional literal puts another summand to the sum of frequencies, and clauses are sorted in descending order of (the sum of) frequencies (of their contained literals). Therefore, in general shorter clauses are less than longer clauses. The variables in quantifier blocks are (in descending order) sorted according to the (positive or negative) frequencies of the variables in the formula.

## countedBinaries

This metric takes into consideration the number of unique newly created binary clauses by assigning a variable to a literal. In other words, the number of ternary clauses containing the literal of the other sign is counted and taken as a measure. The formula is then sorted in descending order according to the number of newly created binary clauses (ternary clauses containing the literal of the opposite sign).

For sorting literals within a clause, the literals that create the most new binary clauses when being assigned to the formula come first. When sorting clauses, all new binary clauses created by assigning each literal in the clause are considered, possible duplicate binary clauses are eliminated, and the resulting number of (unique) newly created binary clauses is taken as a measure. For the quantifier blocks, the sum (of newly created binary clauses) for both the positive and the negative literal are used as a measure to sort the variables.

## weightedBinaries

This metric calculates a heuristic for each variable that is based on the unique newly created binary clauses by assigning the variable the positive or negative literal. The heuristic for the variable is the product of the heuristics for both corresponding (positive and negative) literals. The heuristic for the literal is based on the newly created binary clauses by assigning that literal. First the newly created binary clauses are calculated. Each newly created binary clause is weighted and the heuristic of the literal is the sum of the weights of the binary clauses. The weight of the binary clause is the sum of the weights of both opposite literals in the binary clause. The weight of the literal is 5 times the number of occurrences in binary clauses, plus 1 times the number of occurrences in ternary clauses, plus 1/5 times the number of occurrences in clauses of size 4 and so on (factor is divided by 5 for each further literal in the clause).
