project(qbfsort)
cmake_minimum_required(VERSION 3.18)
find_package(jsoncpp REQUIRED)
set(WITH_TESTS)
set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES main.cpp QbfFormula.cpp LiteralSorter.cpp ClauseSorter.cpp QuantifierSorter.cpp)
add_executable(qbfsort ${SOURCE_FILES})
target_link_libraries(qbfsort jsoncpp_lib)

if(${WITH_TESTS})
  enable_testing()

  add_test(NAME basic
      COMMAND bash -c " \
      ./qbfsort printStatistics=test/basic.json < test/input/basic.cnf > test/basic.cnf; \
      diff test/basic.cnf test/expected/basic.cnf && \
      diff <(jq -S . test/basic.json) <(jq -S . test/expected/basic.json) \
      "
  )
  add_test(NAME literal_basic
      COMMAND bash -c " \
      ./qbfsort sortLiterals=basic < test/input/basic.cnf > test/literal_basic.cnf; \
      diff test/literal_basic.cnf test/expected/literal_basic.cnf \
      "
  )
  add_test(NAME clause_basic
      COMMAND bash -c " \
      ./qbfsort sortClauses=basic < test/input/basic.cnf > test/clause_basic.cnf; \
      diff test/clause_basic.cnf test/expected/clause_basic.cnf \
      "
  )
  add_test(NAME quantifier_basic 
      COMMAND bash -c " \
      ./qbfsort sortQuantifiers=basic < test/input/basic.cnf > test/quantifier_basic.cnf; \
      diff test/quantifier_basic.cnf test/expected/quantifier_basic.cnf \
      "
  )

  add_test(NAME frequency
      COMMAND bash -c " \
      ./qbfsort printStatistics=test/frequency.json < test/input/frequency.cnf > test/frequency.cnf; \
      diff test/frequency.cnf test/expected/frequency.cnf &&  \
      diff <(jq -S . test/frequency.json) <(jq -S . test/expected/frequency.json) \
      "
  )
  add_test(NAME literal_frequency
      COMMAND bash -c " \
      ./qbfsort sortLiterals=frequency < test/input/frequency.cnf > test/literal_frequency.cnf; \
      diff test/literal_frequency.cnf test/expected/literal_frequency.cnf \
      "
  )
  add_test(NAME clause_frequency
      COMMAND bash -c " \
      ./qbfsort sortClauses=frequency < test/input/frequency.cnf > test/clause_frequency.cnf; \
      diff test/clause_frequency.cnf test/expected/clause_frequency.cnf \
      "
  )
  add_test(NAME quantifier_frequency
      COMMAND bash -c " \
      ./qbfsort sortQuantifiers=frequency < test/input/frequency.cnf > test/quantifier_frequency.cnf; \
      diff test/quantifier_frequency.cnf test/expected/quantifier_frequency.cnf \
      "
  )

  add_test(NAME countedBinaries
      COMMAND bash -c " \
      ./qbfsort printStatistics=test/countedBinaries < test/input/countedBinaries.cnf > test/countedBinaries.cnf; \
      diff test/countedBinaries.cnf test/expected/countedBinaries.cnf &&  \
      diff <(jq -S . test/countedBinaries.json) <(jq -S . test/expected/countedBinaries.json) \
      "
  )
  add_test(NAME literal_countedBinaries
      COMMAND bash -c " \
      ./qbfsort sortLiterals=countedBinaries < test/input/countedBinaries.cnf > test/literal_countedBinaries.cnf; \
      diff test/literal_countedBinaries.cnf test/expected/literal_countedBinaries.cnf \
      "
  )
  add_test(NAME clause_countedBinaries
      COMMAND bash -c " \
      ./qbfsort sortClauses=countedBinaries < test/input/countedBinaries.cnf > test/clause_countedBinaries.cnf; \
      diff test/clause_countedBinaries.cnf test/expected/clause_countedBinaries.cnf \
      "
  )
  add_test(NAME quantifier_countedBinaries
      COMMAND bash -c " \
      ./qbfsort sortQuantifiers=countedBinaries < test/input/countedBinaries.cnf > test/quantifier_countedBinaries.cnf; \
      diff test/quantifier_countedBinaries.cnf test/expected/quantifier_countedBinaries.cnf \
      "
  )

  add_test(NAME weightedBinaries
      COMMAND bash -c " \
      ./qbfsort printStatistics=test/weightedBinaries.json < test/input/weightedBinaries.cnf > test/weightedBinaries.cnf; \
      diff test/weightedBinaries.cnf test/expected/weightedBinaries.cnf && \
      diff <(jq -S . test/weightedBinaries.json) <(jq -S . test/expected/weightedBinaries.json) \
      "
  )
  add_test(NAME literal_weightedBinaries
      COMMAND bash -c " \
      ./qbfsort sortLiterals=weightedBinaries < test/input/weightedBinaries.cnf > test/literal_weightedBinaries.cnf; \
      diff test/literal_weightedBinaries.cnf test/expected/literal_weightedBinaries.cnf \
      "
  )
  add_test(NAME clause_weightedBinaries
      COMMAND bash -c " \
      ./qbfsort sortClauses=weightedBinaries < test/input/weightedBinaries.cnf > test/clause_weightedBinaries.cnf; \
      diff test/clause_weightedBinaries.cnf test/expected/clause_weightedBinaries.cnf \
      "
  )
  add_test(NAME quantifier_weightedBinaries 
      COMMAND bash -c " \
      ./qbfsort sortQuantifiers=weightedBinaries < test/input/weightedBinaries.cnf > test/quantifier_weightedBinaries.cnf; \
      diff test/quantifier_weightedBinaries.cnf test/expected/quantifier_weightedBinaries.cnf \
      "
  )

  file(COPY ../test DESTINATION ${CMAKE_BINARY_DIR})
endif()
