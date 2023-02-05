project(qbfsort)
set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES main.cpp QbfFormula.cpp LiteralSorter.cpp ClauseSorter.cpp QuantifierSorter.cpp)
add_executable(qbfsort ${SOURCE_FILES})
