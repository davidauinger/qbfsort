project(qbfsort)
cmake_minimum_required(VERSION 3.18)
find_package(jsoncpp REQUIRED)
find_package(Boost 1.74 COMPONENTS program_options REQUIRED)
option(WITH_TESTS "include tests in build directory")
set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES src/main.cpp src/Formula.cpp src/LiteralSorter.cpp src/ClauseSorter.cpp)
add_executable(qbfsort ${SOURCE_FILES})
target_include_directories(qbfsort PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include ${Boost_INCLUDE_DIR})
target_link_libraries(qbfsort jsoncpp_lib ${Boost_LIBRARIES})

if(${WITH_TESTS})
  enable_testing()
  file(COPY test DESTINATION ${CMAKE_BINARY_DIR})
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/test/output)

  add_test(NAME parse.empty
      COMMAND bash -c " \
      ./qbfsort < test/input/empty.qdimacs > test/output/empty.qdimacs; \
      diff test/output/empty.qdimacs test/expected/empty.qdimacs \
      "
  )
  add_test(NAME parse.comment
      COMMAND bash -c " \
      ./qbfsort < test/input/comment.qdimacs > test/output/comment.qdimacs; \
      diff test/output/comment.qdimacs test/expected/comment.qdimacs \
      "
  )
  add_test(NAME parse.short
      COMMAND bash -c " \
      ./qbfsort < test/input/short.qdimacs > test/output/short.qdimacs; \
      diff test/output/short.qdimacs test/expected/short.qdimacs \
      "
  )
  add_test(NAME parse.extended
      COMMAND bash -c " \
      ./qbfsort < test/input/extended.qdimacs > test/output/extended.qdimacs; \
      diff test/output/extended.qdimacs test/expected/extended.qdimacs \
      "
  )
  add_test(NAME parse.binaries
      COMMAND bash -c " \
      ./qbfsort < test/input/binaries.qdimacs > test/output/binaries.qdimacs; \
      diff test/output/binaries.qdimacs test/expected/binaries.qdimacs \
      "
  )

  add_test(NAME statistics.empty
      COMMAND bash -c " \
      ./qbfsort --input test/input/empty.qdimacs --output /dev/null --statistics test/output/empty.json; \
      diff <(jq -S . test/output/empty.json) <(jq -S . test/expected/empty.json) \
      "
  )
  add_test(NAME statistics.comment
      COMMAND bash -c " \
      ./qbfsort --input test/input/comment.qdimacs --output /dev/null --statistics test/output/comment.json; \
      diff <(jq -S . test/output/comment.json) <(jq -S . test/expected/comment.json) \
      "
  )
  add_test(NAME statistics.short
      COMMAND bash -c " \
      ./qbfsort --input test/input/short.qdimacs --output /dev/null --statistics test/output/short.json; \
      diff <(jq -S . test/output/short.json) <(jq -S . test/expected/short.json) \
      "
  )
  add_test(NAME statistics.extended
      COMMAND bash -c " \
      ./qbfsort --input test/input/extended.qdimacs --output /dev/null --statistics test/output/extended.json; \
      diff <(jq -S . test/output/extended.json) <(jq -S . test/expected/extended.json) \
      "
  )
  add_test(NAME statistics.binaries
      COMMAND bash -c " \
      ./qbfsort --input test/input/binaries.qdimacs --output /dev/null --statistics test/output/binaries.json; \
      diff <(jq -S . test/output/binaries.json) <(jq -S . test/expected/binaries.json) \
      "
  )

  add_test(NAME literals_none.empty
      COMMAND bash -c " \
      ./qbfsort --input test/input/empty.qdimacs --output test/output/literals_none_empty.qdimacs --stable --literals none; \
      diff test/output/literals_none_empty.qdimacs test/expected/literals_none_empty.qdimacs \
      "
  )
  add_test(NAME literals_none.comment
      COMMAND bash -c " \
      ./qbfsort --input test/input/comment.qdimacs --output test/output/literals_none_comment.qdimacs --stable --literals none; \
      diff test/output/literals_none_comment.qdimacs test/expected/literals_none_comment.qdimacs \
      "
  )
  add_test(NAME literals_none.short
      COMMAND bash -c " \
      ./qbfsort --input test/input/short.qdimacs --output test/output/literals_none_short.qdimacs --stable --literals none; \
      diff test/output/literals_none_short.qdimacs test/expected/literals_none_short.qdimacs \
      "
  )
  add_test(NAME literals_none.extended
      COMMAND bash -c " \
      ./qbfsort --input test/input/extended.qdimacs --output test/output/literals_none_extended.qdimacs --stable --literals none; \
      diff test/output/literals_none_extended.qdimacs test/expected/literals_none_extended.qdimacs \
      "
  )
  add_test(NAME literals_none.binaries
      COMMAND bash -c " \
      ./qbfsort --input test/input/binaries.qdimacs --output test/output/literals_none_binaries.qdimacs --stable --literals none; \
      diff test/output/literals_none_binaries.qdimacs test/expected/literals_none_binaries.qdimacs \
      "
  )

  add_test(NAME literals_basic.empty
      COMMAND bash -c " \
      ./qbfsort --input test/input/empty.qdimacs --output test/output/literals_basic_empty.qdimacs --stable --literals basic; \
      diff test/output/literals_basic_empty.qdimacs test/expected/literals_basic_empty.qdimacs \
      "
  )
  add_test(NAME literals_basic.comment
      COMMAND bash -c " \
      ./qbfsort --input test/input/comment.qdimacs --output test/output/literals_basic_comment.qdimacs --stable --literals basic; \
      diff test/output/literals_basic_comment.qdimacs test/expected/literals_basic_comment.qdimacs \
      "
  )
  add_test(NAME literals_basic.short
      COMMAND bash -c " \
      ./qbfsort --input test/input/short.qdimacs --output test/output/literals_basic_short.qdimacs --stable --literals basic; \
      diff test/output/literals_basic_short.qdimacs test/expected/literals_basic_short.qdimacs \
      "
  )
  add_test(NAME literals_basic.extended
      COMMAND bash -c " \
      ./qbfsort --input test/input/extended.qdimacs --output test/output/literals_basic_extended.qdimacs --stable --literals basic; \
      diff test/output/literals_basic_extended.qdimacs test/expected/literals_basic_extended.qdimacs \
      "
  )
  add_test(NAME literals_basic.binaries
      COMMAND bash -c " \
      ./qbfsort --input test/input/binaries.qdimacs --output test/output/literals_basic_binaries.qdimacs --stable --literals basic; \
      diff test/output/literals_basic_binaries.qdimacs test/expected/literals_basic_binaries.qdimacs \
      "
  )

  add_test(NAME literals_frequencyLiteral.empty
      COMMAND bash -c " \
      ./qbfsort --input test/input/empty.qdimacs --output test/output/literals_frequencyLiteral_empty.qdimacs --stable --literals frequencyLiteral; \
      diff test/output/literals_frequencyLiteral_empty.qdimacs test/expected/literals_frequencyLiteral_empty.qdimacs \
      "
  )
  add_test(NAME literals_frequencyLiteral.comment
      COMMAND bash -c " \
      ./qbfsort --input test/input/comment.qdimacs --output test/output/literals_frequencyLiteral_comment.qdimacs --stable --literals frequencyLiteral; \
      diff test/output/literals_frequencyLiteral_comment.qdimacs test/expected/literals_frequencyLiteral_comment.qdimacs \
      "
  )
  add_test(NAME literals_frequencyLiteral.short
      COMMAND bash -c " \
      ./qbfsort --input test/input/short.qdimacs --output test/output/literals_frequencyLiteral_short.qdimacs --stable --literals frequencyLiteral; \
      diff test/output/literals_frequencyLiteral_short.qdimacs test/expected/literals_frequencyLiteral_short.qdimacs \
      "
  )
  add_test(NAME literals_frequencyLiteral.extended
      COMMAND bash -c " \
      ./qbfsort --input test/input/extended.qdimacs --output test/output/literals_frequencyLiteral_extended.qdimacs --stable --literals frequencyLiteral; \
      diff test/output/literals_frequencyLiteral_extended.qdimacs test/expected/literals_frequencyLiteral_extended.qdimacs \
      "
  )
  add_test(NAME literals_frequencyLiteral.binaries
      COMMAND bash -c " \
      ./qbfsort --input test/input/binaries.qdimacs --output test/output/literals_frequencyLiteral_binaries.qdimacs --stable --literals frequencyLiteral; \
      diff test/output/literals_frequencyLiteral_binaries.qdimacs test/expected/literals_frequencyLiteral_binaries.qdimacs \
      "
  )

  add_test(NAME literals_frequencyVariable.empty
      COMMAND bash -c " \
      ./qbfsort --input test/input/empty.qdimacs --output test/output/literals_frequencyVariable_empty.qdimacs --stable --literals frequencyVariable; \
      diff test/output/literals_frequencyVariable_empty.qdimacs test/expected/literals_frequencyVariable_empty.qdimacs \
      "
  )
  add_test(NAME literals_frequencyVariable.comment
      COMMAND bash -c " \
      ./qbfsort --input test/input/comment.qdimacs --output test/output/literals_frequencyVariable_comment.qdimacs --stable --literals frequencyVariable; \
      diff test/output/literals_frequencyVariable_comment.qdimacs test/expected/literals_frequencyVariable_comment.qdimacs \
      "
  )
  add_test(NAME literals_frequencyVariable.short
      COMMAND bash -c " \
      ./qbfsort --input test/input/short.qdimacs --output test/output/literals_frequencyVariable_short.qdimacs --stable --literals frequencyVariable; \
      diff test/output/literals_frequencyVariable_short.qdimacs test/expected/literals_frequencyVariable_short.qdimacs \
      "
  )
  add_test(NAME literals_frequencyVariable.extended
      COMMAND bash -c " \
      ./qbfsort --input test/input/extended.qdimacs --output test/output/literals_frequencyVariable_extended.qdimacs --stable --literals frequencyVariable; \
      diff test/output/literals_frequencyVariable_extended.qdimacs test/expected/literals_frequencyVariable_extended.qdimacs \
      "
  )
  add_test(NAME literals_frequencyVariable.binaries
      COMMAND bash -c " \
      ./qbfsort --input test/input/binaries.qdimacs --output test/output/literals_frequencyVariable_binaries.qdimacs --stable --literals frequencyVariable; \
      diff test/output/literals_frequencyVariable_binaries.qdimacs test/expected/literals_frequencyVariable_binaries.qdimacs \
      "
  )

  add_test(NAME literals_countedBinariesLiteral.empty
      COMMAND bash -c " \
      ./qbfsort --input test/input/empty.qdimacs --output test/output/literals_countedBinariesLiteral_empty.qdimacs --stable --literals countedBinariesLiteral; \
      diff test/output/literals_countedBinariesLiteral_empty.qdimacs test/expected/literals_countedBinariesLiteral_empty.qdimacs \
      "
  )
  add_test(NAME literals_countedBinariesLiteral.comment
      COMMAND bash -c " \
      ./qbfsort --input test/input/comment.qdimacs --output test/output/literals_countedBinariesLiteral_comment.qdimacs --stable --literals countedBinariesLiteral; \
      diff test/output/literals_countedBinariesLiteral_comment.qdimacs test/expected/literals_countedBinariesLiteral_comment.qdimacs \
      "
  )
  add_test(NAME literals_countedBinariesLiteral.short
      COMMAND bash -c " \
      ./qbfsort --input test/input/short.qdimacs --output test/output/literals_countedBinariesLiteral_short.qdimacs --stable --literals countedBinariesLiteral; \
      diff test/output/literals_countedBinariesLiteral_short.qdimacs test/expected/literals_countedBinariesLiteral_short.qdimacs \
      "
  )
  add_test(NAME literals_countedBinariesLiteral.extended
      COMMAND bash -c " \
      ./qbfsort --input test/input/extended.qdimacs --output test/output/literals_countedBinariesLiteral_extended.qdimacs --stable --literals countedBinariesLiteral; \
      diff test/output/literals_countedBinariesLiteral_extended.qdimacs test/expected/literals_countedBinariesLiteral_extended.qdimacs \
      "
  )
  add_test(NAME literals_countedBinariesLiteral.binaries
      COMMAND bash -c " \
      ./qbfsort --input test/input/binaries.qdimacs --output test/output/literals_countedBinariesLiteral_binaries.qdimacs --stable --literals countedBinariesLiteral; \
      diff test/output/literals_countedBinariesLiteral_binaries.qdimacs test/expected/literals_countedBinariesLiteral_binaries.qdimacs \
      "
  )

  add_test(NAME literals_countedBinariesVariable.empty
      COMMAND bash -c " \
      ./qbfsort --input test/input/empty.qdimacs --output test/output/literals_countedBinariesVariable_empty.qdimacs --stable --literals countedBinariesVariable; \
      diff test/output/literals_countedBinariesVariable_empty.qdimacs test/expected/literals_countedBinariesVariable_empty.qdimacs \
      "
  )
  add_test(NAME literals_countedBinariesVariable.comment
      COMMAND bash -c " \
      ./qbfsort --input test/input/comment.qdimacs --output test/output/literals_countedBinariesVariable_comment.qdimacs --stable --literals countedBinariesVariable; \
      diff test/output/literals_countedBinariesVariable_comment.qdimacs test/expected/literals_countedBinariesVariable_comment.qdimacs \
      "
  )
  add_test(NAME literals_countedBinariesVariable.short
      COMMAND bash -c " \
      ./qbfsort --input test/input/short.qdimacs --output test/output/literals_countedBinariesVariable_short.qdimacs --stable --literals countedBinariesVariable; \
      diff test/output/literals_countedBinariesVariable_short.qdimacs test/expected/literals_countedBinariesVariable_short.qdimacs \
      "
  )
  add_test(NAME literals_countedBinariesVariable.extended
      COMMAND bash -c " \
      ./qbfsort --input test/input/extended.qdimacs --output test/output/literals_countedBinariesVariable_extended.qdimacs --stable --literals countedBinariesVariable; \
      diff test/output/literals_countedBinariesVariable_extended.qdimacs test/expected/literals_countedBinariesVariable_extended.qdimacs \
      "
  )
  add_test(NAME literals_countedBinariesVariable.binaries
      COMMAND bash -c " \
      ./qbfsort --input test/input/binaries.qdimacs --output test/output/literals_countedBinariesVariable_binaries.qdimacs --stable --literals countedBinariesVariable; \
      diff test/output/literals_countedBinariesVariable_binaries.qdimacs test/expected/literals_countedBinariesVariable_binaries.qdimacs \
      "
  )

  add_test(NAME literals_weightedBinaries.empty
      COMMAND bash -c " \
      ./qbfsort --input test/input/empty.qdimacs --output test/output/literals_weightedBinaries_empty.qdimacs --stable --literals weightedBinaries; \
      diff test/output/literals_weightedBinaries_empty.qdimacs test/expected/literals_weightedBinaries_empty.qdimacs \
      "
  )
  add_test(NAME literals_weightedBinaries.comment
      COMMAND bash -c " \
      ./qbfsort --input test/input/comment.qdimacs --output test/output/literals_weightedBinaries_comment.qdimacs --stable --literals weightedBinaries; \
      diff test/output/literals_weightedBinaries_comment.qdimacs test/expected/literals_weightedBinaries_comment.qdimacs \
      "
  )
  add_test(NAME literals_weightedBinaries.short
      COMMAND bash -c " \
      ./qbfsort --input test/input/short.qdimacs --output test/output/literals_weightedBinaries_short.qdimacs --stable --literals weightedBinaries; \
      diff test/output/literals_weightedBinaries_short.qdimacs test/expected/literals_weightedBinaries_short.qdimacs \
      "
  )
  add_test(NAME literals_weightedBinaries.extended
      COMMAND bash -c " \
      ./qbfsort --input test/input/extended.qdimacs --output test/output/literals_weightedBinaries_extended.qdimacs --stable --literals weightedBinaries; \
      diff test/output/literals_weightedBinaries_extended.qdimacs test/expected/literals_weightedBinaries_extended.qdimacs \
      "
  )
  add_test(NAME literals_weightedBinaries.binaries
      COMMAND bash -c " \
      ./qbfsort --input test/input/binaries.qdimacs --output test/output/literals_weightedBinaries_binaries.qdimacs --stable --literals weightedBinaries; \
      diff test/output/literals_weightedBinaries_binaries.qdimacs test/expected/literals_weightedBinaries_binaries.qdimacs \
      "
  )

  add_test(NAME quantifiers_none.empty
      COMMAND bash -c " \
      ./qbfsort --input test/input/empty.qdimacs --output test/output/quantifiers_none_empty.qdimacs --stable --quantifiers none; \
      diff test/output/quantifiers_none_empty.qdimacs test/expected/quantifiers_none_empty.qdimacs \
      "
  )
  add_test(NAME quantifiers_none.comment
      COMMAND bash -c " \
      ./qbfsort --input test/input/comment.qdimacs --output test/output/quantifiers_none_comment.qdimacs --stable --quantifiers none; \
      diff test/output/quantifiers_none_comment.qdimacs test/expected/quantifiers_none_comment.qdimacs \
      "
  )
  add_test(NAME quantifiers_none.short
      COMMAND bash -c " \
      ./qbfsort --input test/input/short.qdimacs --output test/output/quantifiers_none_short.qdimacs --stable --quantifiers none; \
      diff test/output/quantifiers_none_short.qdimacs test/expected/quantifiers_none_short.qdimacs \
      "
  )
  add_test(NAME quantifiers_none.extended
      COMMAND bash -c " \
      ./qbfsort --input test/input/extended.qdimacs --output test/output/quantifiers_none_extended.qdimacs --stable --quantifiers none; \
      diff test/output/quantifiers_none_extended.qdimacs test/expected/quantifiers_none_extended.qdimacs \
      "
  )
  add_test(NAME quantifiers_none.binaries
      COMMAND bash -c " \
      ./qbfsort --input test/input/binaries.qdimacs --output test/output/quantifiers_none_binaries.qdimacs --stable --quantifiers none; \
      diff test/output/quantifiers_none_binaries.qdimacs test/expected/quantifiers_none_binaries.qdimacs \
      "
  )

  add_test(NAME quantifiers_basic.empty
      COMMAND bash -c " \
      ./qbfsort --input test/input/empty.qdimacs --output test/output/quantifiers_basic_empty.qdimacs --stable --quantifiers basic; \
      diff test/output/quantifiers_basic_empty.qdimacs test/expected/quantifiers_basic_empty.qdimacs \
      "
  )
  add_test(NAME quantifiers_basic.comment
      COMMAND bash -c " \
      ./qbfsort --input test/input/comment.qdimacs --output test/output/quantifiers_basic_comment.qdimacs --stable --quantifiers basic; \
      diff test/output/quantifiers_basic_comment.qdimacs test/expected/quantifiers_basic_comment.qdimacs \
      "
  )
  add_test(NAME quantifiers_basic.short
      COMMAND bash -c " \
      ./qbfsort --input test/input/short.qdimacs --output test/output/quantifiers_basic_short.qdimacs --stable --quantifiers basic; \
      diff test/output/quantifiers_basic_short.qdimacs test/expected/quantifiers_basic_short.qdimacs \
      "
  )
  add_test(NAME quantifiers_basic.extended
      COMMAND bash -c " \
      ./qbfsort --input test/input/extended.qdimacs --output test/output/quantifiers_basic_extended.qdimacs --stable --quantifiers basic; \
      diff test/output/quantifiers_basic_extended.qdimacs test/expected/quantifiers_basic_extended.qdimacs \
      "
  )
  add_test(NAME quantifiers_basic.binaries
      COMMAND bash -c " \
      ./qbfsort --input test/input/binaries.qdimacs --output test/output/quantifiers_basic_binaries.qdimacs --stable --quantifiers basic; \
      diff test/output/quantifiers_basic_binaries.qdimacs test/expected/quantifiers_basic_binaries.qdimacs \
      "
  )

  add_test(NAME quantifiers_frequencyVariable.empty
      COMMAND bash -c " \
      ./qbfsort --input test/input/empty.qdimacs --output test/output/quantifiers_frequencyVariable_empty.qdimacs --stable --quantifiers frequencyVariable; \
      diff test/output/quantifiers_frequencyVariable_empty.qdimacs test/expected/quantifiers_frequencyVariable_empty.qdimacs \
      "
  )
  add_test(NAME quantifiers_frequencyVariable.comment
      COMMAND bash -c " \
      ./qbfsort --input test/input/comment.qdimacs --output test/output/quantifiers_frequencyVariable_comment.qdimacs --stable --quantifiers frequencyVariable; \
      diff test/output/quantifiers_frequencyVariable_comment.qdimacs test/expected/quantifiers_frequencyVariable_comment.qdimacs \
      "
  )
  add_test(NAME quantifiers_frequencyVariable.short
      COMMAND bash -c " \
      ./qbfsort --input test/input/short.qdimacs --output test/output/quantifiers_frequencyVariable_short.qdimacs --stable --quantifiers frequencyVariable; \
      diff test/output/quantifiers_frequencyVariable_short.qdimacs test/expected/quantifiers_frequencyVariable_short.qdimacs \
      "
  )
  add_test(NAME quantifiers_frequencyVariable.extended
      COMMAND bash -c " \
      ./qbfsort --input test/input/extended.qdimacs --output test/output/quantifiers_frequencyVariable_extended.qdimacs --stable --quantifiers frequencyVariable; \
      diff test/output/quantifiers_frequencyVariable_extended.qdimacs test/expected/quantifiers_frequencyVariable_extended.qdimacs \
      "
  )
  add_test(NAME quantifiers_frequencyVariable.binaries
      COMMAND bash -c " \
      ./qbfsort --input test/input/binaries.qdimacs --output test/output/quantifiers_frequencyVariable_binaries.qdimacs --stable --quantifiers frequencyVariable; \
      diff test/output/quantifiers_frequencyVariable_binaries.qdimacs test/expected/quantifiers_frequencyVariable_binaries.qdimacs \
      "
  )

  add_test(NAME quantifiers_countedBinariesVariable.empty
      COMMAND bash -c " \
      ./qbfsort --input test/input/empty.qdimacs --output test/output/quantifiers_countedBinariesVariable_empty.qdimacs --stable --quantifiers countedBinariesVariable; \
      diff test/output/quantifiers_countedBinariesVariable_empty.qdimacs test/expected/quantifiers_countedBinariesVariable_empty.qdimacs \
      "
  )
  add_test(NAME quantifiers_countedBinariesVariable.comment
      COMMAND bash -c " \
      ./qbfsort --input test/input/comment.qdimacs --output test/output/quantifiers_countedBinariesVariable_comment.qdimacs --stable --quantifiers countedBinariesVariable; \
      diff test/output/quantifiers_countedBinariesVariable_comment.qdimacs test/expected/quantifiers_countedBinariesVariable_comment.qdimacs \
      "
  )
  add_test(NAME quantifiers_countedBinariesVariable.short
      COMMAND bash -c " \
      ./qbfsort --input test/input/short.qdimacs --output test/output/quantifiers_countedBinariesVariable_short.qdimacs --stable --quantifiers countedBinariesVariable; \
      diff test/output/quantifiers_countedBinariesVariable_short.qdimacs test/expected/quantifiers_countedBinariesVariable_short.qdimacs \
      "
  )
  add_test(NAME quantifiers_countedBinariesVariable.extended
      COMMAND bash -c " \
      ./qbfsort --input test/input/extended.qdimacs --output test/output/quantifiers_countedBinariesVariable_extended.qdimacs --stable --quantifiers countedBinariesVariable; \
      diff test/output/quantifiers_countedBinariesVariable_extended.qdimacs test/expected/quantifiers_countedBinariesVariable_extended.qdimacs \
      "
  )
  add_test(NAME quantifiers_countedBinariesVariable.binaries
      COMMAND bash -c " \
      ./qbfsort --input test/input/binaries.qdimacs --output test/output/quantifiers_countedBinariesVariable_binaries.qdimacs --stable --quantifiers countedBinariesVariable; \
      diff test/output/quantifiers_countedBinariesVariable_binaries.qdimacs test/expected/quantifiers_countedBinariesVariable_binaries.qdimacs \
      "
  )

  add_test(NAME quantifiers_weightedBinaries.empty
      COMMAND bash -c " \
      ./qbfsort --input test/input/empty.qdimacs --output test/output/quantifiers_weightedBinaries_empty.qdimacs --stable --quantifiers weightedBinaries; \
      diff test/output/quantifiers_weightedBinaries_empty.qdimacs test/expected/quantifiers_weightedBinaries_empty.qdimacs \
      "
  )
  add_test(NAME quantifiers_weightedBinaries.comment
      COMMAND bash -c " \
      ./qbfsort --input test/input/comment.qdimacs --output test/output/quantifiers_weightedBinaries_comment.qdimacs --stable --quantifiers weightedBinaries; \
      diff test/output/quantifiers_weightedBinaries_comment.qdimacs test/expected/quantifiers_weightedBinaries_comment.qdimacs \
      "
  )
  add_test(NAME quantifiers_weightedBinaries.short
      COMMAND bash -c " \
      ./qbfsort --input test/input/short.qdimacs --output test/output/quantifiers_weightedBinaries_short.qdimacs --stable --quantifiers weightedBinaries; \
      diff test/output/quantifiers_weightedBinaries_short.qdimacs test/expected/quantifiers_weightedBinaries_short.qdimacs \
      "
  )
  add_test(NAME quantifiers_weightedBinaries.extended
      COMMAND bash -c " \
      ./qbfsort --input test/input/extended.qdimacs --output test/output/quantifiers_weightedBinaries_extended.qdimacs --stable --quantifiers weightedBinaries; \
      diff test/output/quantifiers_weightedBinaries_extended.qdimacs test/expected/quantifiers_weightedBinaries_extended.qdimacs \
      "
  )
  add_test(NAME quantifiers_weightedBinaries.binaries
      COMMAND bash -c " \
      ./qbfsort --input test/input/binaries.qdimacs --output test/output/quantifiers_weightedBinaries_binaries.qdimacs --stable --quantifiers weightedBinaries; \
      diff test/output/quantifiers_weightedBinaries_binaries.qdimacs test/expected/quantifiers_weightedBinaries_binaries.qdimacs \
      "
  )

  add_test(NAME clauses_none.empty
      COMMAND bash -c " \
      ./qbfsort --input test/input/empty.qdimacs --output test/output/clauses_none_empty.qdimacs --stable --clauses none; \
      diff test/output/clauses_none_empty.qdimacs test/expected/clauses_none_empty.qdimacs \
      "
  )
  add_test(NAME clauses_none.comment
      COMMAND bash -c " \
      ./qbfsort --input test/input/comment.qdimacs --output test/output/clauses_none_comment.qdimacs --stable --clauses none; \
      diff test/output/clauses_none_comment.qdimacs test/expected/clauses_none_comment.qdimacs \
      "
  )
  add_test(NAME clauses_none.short
      COMMAND bash -c " \
      ./qbfsort --input test/input/short.qdimacs --output test/output/clauses_none_short.qdimacs --stable --clauses none; \
      diff test/output/clauses_none_short.qdimacs test/expected/clauses_none_short.qdimacs \
      "
  )
  add_test(NAME clauses_none.extended
      COMMAND bash -c " \
      ./qbfsort --input test/input/extended.qdimacs --output test/output/clauses_none_extended.qdimacs --stable --clauses none; \
      diff test/output/clauses_none_extended.qdimacs test/expected/clauses_none_extended.qdimacs \
      "
  )
  add_test(NAME clauses_none.binaries
      COMMAND bash -c " \
      ./qbfsort --input test/input/binaries.qdimacs --output test/output/clauses_none_binaries.qdimacs --stable --clauses none; \
      diff test/output/clauses_none_binaries.qdimacs test/expected/clauses_none_binaries.qdimacs \
      "
  )

  add_test(NAME clauses_basic.empty
      COMMAND bash -c " \
      ./qbfsort --input test/input/empty.qdimacs --output test/output/clauses_basic_empty.qdimacs --stable --clauses basic; \
      diff test/output/clauses_basic_empty.qdimacs test/expected/clauses_basic_empty.qdimacs \
      "
  )
  add_test(NAME clauses_basic.comment
      COMMAND bash -c " \
      ./qbfsort --input test/input/comment.qdimacs --output test/output/clauses_basic_comment.qdimacs --stable --clauses basic; \
      diff test/output/clauses_basic_comment.qdimacs test/expected/clauses_basic_comment.qdimacs \
      "
  )
  add_test(NAME clauses_basic.short
      COMMAND bash -c " \
      ./qbfsort --input test/input/short.qdimacs --output test/output/clauses_basic_short.qdimacs --stable --clauses basic; \
      diff test/output/clauses_basic_short.qdimacs test/expected/clauses_basic_short.qdimacs \
      "
  )
  add_test(NAME clauses_basic.extended
      COMMAND bash -c " \
      ./qbfsort --input test/input/extended.qdimacs --output test/output/clauses_basic_extended.qdimacs --stable --clauses basic; \
      diff test/output/clauses_basic_extended.qdimacs test/expected/clauses_basic_extended.qdimacs \
      "
  )
  add_test(NAME clauses_basic.binaries
      COMMAND bash -c " \
      ./qbfsort --input test/input/binaries.qdimacs --output test/output/clauses_basic_binaries.qdimacs --stable --clauses basic; \
      diff test/output/clauses_basic_binaries.qdimacs test/expected/clauses_basic_binaries.qdimacs \
      "
  )

  add_test(NAME clauses_frequencyLiteralSums.empty
      COMMAND bash -c " \
      ./qbfsort --input test/input/empty.qdimacs --output test/output/clauses_frequencyLiteralSums_empty.qdimacs --stable --clauses frequencyLiteralSums; \
      diff test/output/clauses_frequencyLiteralSums_empty.qdimacs test/expected/clauses_frequencyLiteralSums_empty.qdimacs \
      "
  )
  add_test(NAME clauses_frequencyLiteralSums.comment
      COMMAND bash -c " \
      ./qbfsort --input test/input/comment.qdimacs --output test/output/clauses_frequencyLiteralSums_comment.qdimacs --stable --clauses frequencyLiteralSums; \
      diff test/output/clauses_frequencyLiteralSums_comment.qdimacs test/expected/clauses_frequencyLiteralSums_comment.qdimacs \
      "
  )
  add_test(NAME clauses_frequencyLiteralSums.short
      COMMAND bash -c " \
      ./qbfsort --input test/input/short.qdimacs --output test/output/clauses_frequencyLiteralSums_short.qdimacs --stable --clauses frequencyLiteralSums; \
      diff test/output/clauses_frequencyLiteralSums_short.qdimacs test/expected/clauses_frequencyLiteralSums_short.qdimacs \
      "
  )
  add_test(NAME clauses_frequencyLiteralSums.extended
      COMMAND bash -c " \
      ./qbfsort --input test/input/extended.qdimacs --output test/output/clauses_frequencyLiteralSums_extended.qdimacs --stable --clauses frequencyLiteralSums; \
      diff test/output/clauses_frequencyLiteralSums_extended.qdimacs test/expected/clauses_frequencyLiteralSums_extended.qdimacs \
      "
  )
  add_test(NAME clauses_frequencyLiteralSums.binaries
      COMMAND bash -c " \
      ./qbfsort --input test/input/binaries.qdimacs --output test/output/clauses_frequencyLiteralSums_binaries.qdimacs --stable --clauses frequencyLiteralSums; \
      diff test/output/clauses_frequencyLiteralSums_binaries.qdimacs test/expected/clauses_frequencyLiteralSums_binaries.qdimacs \
      "
  )

  add_test(NAME clauses_frequencyLiteralMeans.empty
      COMMAND bash -c " \
      ./qbfsort --input test/input/empty.qdimacs --output test/output/clauses_frequencyLiteralMeans_empty.qdimacs --stable --clauses frequencyLiteralMeans; \
      diff test/output/clauses_frequencyLiteralMeans_empty.qdimacs test/expected/clauses_frequencyLiteralMeans_empty.qdimacs \
      "
  )
  add_test(NAME clauses_frequencyLiteralMeans.comment
      COMMAND bash -c " \
      ./qbfsort --input test/input/comment.qdimacs --output test/output/clauses_frequencyLiteralMeans_comment.qdimacs --stable --clauses frequencyLiteralMeans; \
      diff test/output/clauses_frequencyLiteralMeans_comment.qdimacs test/expected/clauses_frequencyLiteralMeans_comment.qdimacs \
      "
  )
  add_test(NAME clauses_frequencyLiteralMeans.short
      COMMAND bash -c " \
      ./qbfsort --input test/input/short.qdimacs --output test/output/clauses_frequencyLiteralMeans_short.qdimacs --stable --clauses frequencyLiteralMeans; \
      diff test/output/clauses_frequencyLiteralMeans_short.qdimacs test/expected/clauses_frequencyLiteralMeans_short.qdimacs \
      "
  )
  add_test(NAME clauses_frequencyLiteralMeans.extended
      COMMAND bash -c " \
      ./qbfsort --input test/input/extended.qdimacs --output test/output/clauses_frequencyLiteralMeans_extended.qdimacs --stable --clauses frequencyLiteralMeans; \
      diff test/output/clauses_frequencyLiteralMeans_extended.qdimacs test/expected/clauses_frequencyLiteralMeans_extended.qdimacs \
      "
  )
  add_test(NAME clauses_frequencyLiteralMeans.binaries
      COMMAND bash -c " \
      ./qbfsort --input test/input/binaries.qdimacs --output test/output/clauses_frequencyLiteralMeans_binaries.qdimacs --stable --clauses frequencyLiteralMeans; \
      diff test/output/clauses_frequencyLiteralMeans_binaries.qdimacs test/expected/clauses_frequencyLiteralMeans_binaries.qdimacs \
      "
  )

  add_test(NAME clauses_frequencyVariableSums.empty
      COMMAND bash -c " \
      ./qbfsort --input test/input/empty.qdimacs --output test/output/clauses_frequencyVariableSums_empty.qdimacs --stable --clauses frequencyVariableSums; \
      diff test/output/clauses_frequencyVariableSums_empty.qdimacs test/expected/clauses_frequencyVariableSums_empty.qdimacs \
      "
  )
  add_test(NAME clauses_frequencyVariableSums.comment
      COMMAND bash -c " \
      ./qbfsort --input test/input/comment.qdimacs --output test/output/clauses_frequencyVariableSums_comment.qdimacs --stable --clauses frequencyVariableSums; \
      diff test/output/clauses_frequencyVariableSums_comment.qdimacs test/expected/clauses_frequencyVariableSums_comment.qdimacs \
      "
  )
  add_test(NAME clauses_frequencyVariableSums.short
      COMMAND bash -c " \
      ./qbfsort --input test/input/short.qdimacs --output test/output/clauses_frequencyVariableSums_short.qdimacs --stable --clauses frequencyVariableSums; \
      diff test/output/clauses_frequencyVariableSums_short.qdimacs test/expected/clauses_frequencyVariableSums_short.qdimacs \
      "
  )
  add_test(NAME clauses_frequencyVariableSums.extended
      COMMAND bash -c " \
      ./qbfsort --input test/input/extended.qdimacs --output test/output/clauses_frequencyVariableSums_extended.qdimacs --stable --clauses frequencyVariableSums; \
      diff test/output/clauses_frequencyVariableSums_extended.qdimacs test/expected/clauses_frequencyVariableSums_extended.qdimacs \
      "
  )
  add_test(NAME clauses_frequencyVariableSums.binaries
      COMMAND bash -c " \
      ./qbfsort --input test/input/binaries.qdimacs --output test/output/clauses_frequencyVariableSums_binaries.qdimacs --stable --clauses frequencyVariableSums; \
      diff test/output/clauses_frequencyVariableSums_binaries.qdimacs test/expected/clauses_frequencyVariableSums_binaries.qdimacs \
      "
  )

  add_test(NAME clauses_frequencyVariableMeans.empty
      COMMAND bash -c " \
      ./qbfsort --input test/input/empty.qdimacs --output test/output/clauses_frequencyVariableMeans_empty.qdimacs --stable --clauses frequencyVariableMeans; \
      diff test/output/clauses_frequencyVariableMeans_empty.qdimacs test/expected/clauses_frequencyVariableMeans_empty.qdimacs \
      "
  )
  add_test(NAME clauses_frequencyVariableMeans.comment
      COMMAND bash -c " \
      ./qbfsort --input test/input/comment.qdimacs --output test/output/clauses_frequencyVariableMeans_comment.qdimacs --stable --clauses frequencyVariableMeans; \
      diff test/output/clauses_frequencyVariableMeans_comment.qdimacs test/expected/clauses_frequencyVariableMeans_comment.qdimacs \
      "
  )
  add_test(NAME clauses_frequencyVariableMeans.short
      COMMAND bash -c " \
      ./qbfsort --input test/input/short.qdimacs --output test/output/clauses_frequencyVariableMeans_short.qdimacs --stable --clauses frequencyVariableMeans; \
      diff test/output/clauses_frequencyVariableMeans_short.qdimacs test/expected/clauses_frequencyVariableMeans_short.qdimacs \
      "
  )
  add_test(NAME clauses_frequencyVariableMeans.extended
      COMMAND bash -c " \
      ./qbfsort --input test/input/extended.qdimacs --output test/output/clauses_frequencyVariableMeans_extended.qdimacs --stable --clauses frequencyVariableMeans; \
      diff test/output/clauses_frequencyVariableMeans_extended.qdimacs test/expected/clauses_frequencyVariableMeans_extended.qdimacs \
      "
  )
  add_test(NAME clauses_frequencyVariableMeans.binaries
      COMMAND bash -c " \
      ./qbfsort --input test/input/binaries.qdimacs --output test/output/clauses_frequencyVariableMeans_binaries.qdimacs --stable --clauses frequencyVariableMeans; \
      diff test/output/clauses_frequencyVariableMeans_binaries.qdimacs test/expected/clauses_frequencyVariableMeans_binaries.qdimacs \
      "
  )

  add_test(NAME clauses_countedBinariesLiteralSums.empty
      COMMAND bash -c " \
      ./qbfsort --input test/input/empty.qdimacs --output test/output/clauses_countedBinariesLiteralSums_empty.qdimacs --stable --clauses countedBinariesLiteralSums; \
      diff test/output/clauses_countedBinariesLiteralSums_empty.qdimacs test/expected/clauses_countedBinariesLiteralSums_empty.qdimacs \
      "
  )
  add_test(NAME clauses_countedBinariesLiteralSums.comment
      COMMAND bash -c " \
      ./qbfsort --input test/input/comment.qdimacs --output test/output/clauses_countedBinariesLiteralSums_comment.qdimacs --stable --clauses countedBinariesLiteralSums; \
      diff test/output/clauses_countedBinariesLiteralSums_comment.qdimacs test/expected/clauses_countedBinariesLiteralSums_comment.qdimacs \
      "
  )
  add_test(NAME clauses_countedBinariesLiteralSums.short
      COMMAND bash -c " \
      ./qbfsort --input test/input/short.qdimacs --output test/output/clauses_countedBinariesLiteralSums_short.qdimacs --stable --clauses countedBinariesLiteralSums; \
      diff test/output/clauses_countedBinariesLiteralSums_short.qdimacs test/expected/clauses_countedBinariesLiteralSums_short.qdimacs \
      "
  )
  add_test(NAME clauses_countedBinariesLiteralSums.extended
      COMMAND bash -c " \
      ./qbfsort --input test/input/extended.qdimacs --output test/output/clauses_countedBinariesLiteralSums_extended.qdimacs --stable --clauses countedBinariesLiteralSums; \
      diff test/output/clauses_countedBinariesLiteralSums_extended.qdimacs test/expected/clauses_countedBinariesLiteralSums_extended.qdimacs \
      "
  )
  add_test(NAME clauses_countedBinariesLiteralSums.binaries
      COMMAND bash -c " \
      ./qbfsort --input test/input/binaries.qdimacs --output test/output/clauses_countedBinariesLiteralSums_binaries.qdimacs --stable --clauses countedBinariesLiteralSums; \
      diff test/output/clauses_countedBinariesLiteralSums_binaries.qdimacs test/expected/clauses_countedBinariesLiteralSums_binaries.qdimacs \
      "
  )

  add_test(NAME clauses_countedBinariesLiteralMeans.empty
      COMMAND bash -c " \
      ./qbfsort --input test/input/empty.qdimacs --output test/output/clauses_countedBinariesLiteralMeans_empty.qdimacs --stable --clauses countedBinariesLiteralMeans; \
      diff test/output/clauses_countedBinariesLiteralMeans_empty.qdimacs test/expected/clauses_countedBinariesLiteralMeans_empty.qdimacs \
      "
  )
  add_test(NAME clauses_countedBinariesLiteralMeans.comment
      COMMAND bash -c " \
      ./qbfsort --input test/input/comment.qdimacs --output test/output/clauses_countedBinariesLiteralMeans_comment.qdimacs --stable --clauses countedBinariesLiteralMeans; \
      diff test/output/clauses_countedBinariesLiteralMeans_comment.qdimacs test/expected/clauses_countedBinariesLiteralMeans_comment.qdimacs \
      "
  )
  add_test(NAME clauses_countedBinariesLiteralMeans.short
      COMMAND bash -c " \
      ./qbfsort --input test/input/short.qdimacs --output test/output/clauses_countedBinariesLiteralMeans_short.qdimacs --stable --clauses countedBinariesLiteralMeans; \
      diff test/output/clauses_countedBinariesLiteralMeans_short.qdimacs test/expected/clauses_countedBinariesLiteralMeans_short.qdimacs \
      "
  )
  add_test(NAME clauses_countedBinariesLiteralMeans.extended
      COMMAND bash -c " \
      ./qbfsort --input test/input/extended.qdimacs --output test/output/clauses_countedBinariesLiteralMeans_extended.qdimacs --stable --clauses countedBinariesLiteralMeans; \
      diff test/output/clauses_countedBinariesLiteralMeans_extended.qdimacs test/expected/clauses_countedBinariesLiteralMeans_extended.qdimacs \
      "
  )
  add_test(NAME clauses_countedBinariesLiteralMeans.binaries
      COMMAND bash -c " \
      ./qbfsort --input test/input/binaries.qdimacs --output test/output/clauses_countedBinariesLiteralMeans_binaries.qdimacs --stable --clauses countedBinariesLiteralMeans; \
      diff test/output/clauses_countedBinariesLiteralMeans_binaries.qdimacs test/expected/clauses_countedBinariesLiteralMeans_binaries.qdimacs \
      "
  )

  add_test(NAME clauses_countedBinariesVariableSums.empty
      COMMAND bash -c " \
      ./qbfsort --input test/input/empty.qdimacs --output test/output/clauses_countedBinariesVariableSums_empty.qdimacs --stable --clauses countedBinariesVariableSums; \
      diff test/output/clauses_countedBinariesVariableSums_empty.qdimacs test/expected/clauses_countedBinariesVariableSums_empty.qdimacs \
      "
  )
  add_test(NAME clauses_countedBinariesVariableSums.comment
      COMMAND bash -c " \
      ./qbfsort --input test/input/comment.qdimacs --output test/output/clauses_countedBinariesVariableSums_comment.qdimacs --stable --clauses countedBinariesVariableSums; \
      diff test/output/clauses_countedBinariesVariableSums_comment.qdimacs test/expected/clauses_countedBinariesVariableSums_comment.qdimacs \
      "
  )
  add_test(NAME clauses_countedBinariesVariableSums.short
      COMMAND bash -c " \
      ./qbfsort --input test/input/short.qdimacs --output test/output/clauses_countedBinariesVariableSums_short.qdimacs --stable --clauses countedBinariesVariableSums; \
      diff test/output/clauses_countedBinariesVariableSums_short.qdimacs test/expected/clauses_countedBinariesVariableSums_short.qdimacs \
      "
  )
  add_test(NAME clauses_countedBinariesVariableSums.extended
      COMMAND bash -c " \
      ./qbfsort --input test/input/extended.qdimacs --output test/output/clauses_countedBinariesVariableSums_extended.qdimacs --stable --clauses countedBinariesVariableSums; \
      diff test/output/clauses_countedBinariesVariableSums_extended.qdimacs test/expected/clauses_countedBinariesVariableSums_extended.qdimacs \
      "
  )
  add_test(NAME clauses_countedBinariesVariableSums.binaries
      COMMAND bash -c " \
      ./qbfsort --input test/input/binaries.qdimacs --output test/output/clauses_countedBinariesVariableSums_binaries.qdimacs --stable --clauses countedBinariesVariableSums; \
      diff test/output/clauses_countedBinariesVariableSums_binaries.qdimacs test/expected/clauses_countedBinariesVariableSums_binaries.qdimacs \
      "
  )

  add_test(NAME clauses_countedBinariesVariableMeans.empty
      COMMAND bash -c " \
      ./qbfsort --input test/input/empty.qdimacs --output test/output/clauses_countedBinariesVariableMeans_empty.qdimacs --stable --clauses countedBinariesVariableMeans; \
      diff test/output/clauses_countedBinariesVariableMeans_empty.qdimacs test/expected/clauses_countedBinariesVariableMeans_empty.qdimacs \
      "
  )
  add_test(NAME clauses_countedBinariesVariableMeans.comment
      COMMAND bash -c " \
      ./qbfsort --input test/input/comment.qdimacs --output test/output/clauses_countedBinariesVariableMeans_comment.qdimacs --stable --clauses countedBinariesVariableMeans; \
      diff test/output/clauses_countedBinariesVariableMeans_comment.qdimacs test/expected/clauses_countedBinariesVariableMeans_comment.qdimacs \
      "
  )
  add_test(NAME clauses_countedBinariesVariableMeans.short
      COMMAND bash -c " \
      ./qbfsort --input test/input/short.qdimacs --output test/output/clauses_countedBinariesVariableMeans_short.qdimacs --stable --clauses countedBinariesVariableMeans; \
      diff test/output/clauses_countedBinariesVariableMeans_short.qdimacs test/expected/clauses_countedBinariesVariableMeans_short.qdimacs \
      "
  )
  add_test(NAME clauses_countedBinariesVariableMeans.extended
      COMMAND bash -c " \
      ./qbfsort --input test/input/extended.qdimacs --output test/output/clauses_countedBinariesVariableMeans_extended.qdimacs --stable --clauses countedBinariesVariableMeans; \
      diff test/output/clauses_countedBinariesVariableMeans_extended.qdimacs test/expected/clauses_countedBinariesVariableMeans_extended.qdimacs \
      "
  )
  add_test(NAME clauses_countedBinariesVariableMeans.binaries
      COMMAND bash -c " \
      ./qbfsort --input test/input/binaries.qdimacs --output test/output/clauses_countedBinariesVariableMeans_binaries.qdimacs --stable --clauses countedBinariesVariableMeans; \
      diff test/output/clauses_countedBinariesVariableMeans_binaries.qdimacs test/expected/clauses_countedBinariesVariableMeans_binaries.qdimacs \
      "
  )

  add_test(NAME clauses_weightedBinariesSums.empty
      COMMAND bash -c " \
      ./qbfsort --input test/input/empty.qdimacs --output test/output/clauses_weightedBinariesSums_empty.qdimacs --stable --clauses weightedBinariesSums; \
      diff test/output/clauses_weightedBinariesSums_empty.qdimacs test/expected/clauses_weightedBinariesSums_empty.qdimacs \
      "
  )
  add_test(NAME clauses_weightedBinariesSums.comment
      COMMAND bash -c " \
      ./qbfsort --input test/input/comment.qdimacs --output test/output/clauses_weightedBinariesSums_comment.qdimacs --stable --clauses weightedBinariesSums; \
      diff test/output/clauses_weightedBinariesSums_comment.qdimacs test/expected/clauses_weightedBinariesSums_comment.qdimacs \
      "
  )
  add_test(NAME clauses_weightedBinariesSums.short
      COMMAND bash -c " \
      ./qbfsort --input test/input/short.qdimacs --output test/output/clauses_weightedBinariesSums_short.qdimacs --stable --clauses weightedBinariesSums; \
      diff test/output/clauses_weightedBinariesSums_short.qdimacs test/expected/clauses_weightedBinariesSums_short.qdimacs \
      "
  )
  add_test(NAME clauses_weightedBinariesSums.extended
      COMMAND bash -c " \
      ./qbfsort --input test/input/extended.qdimacs --output test/output/clauses_weightedBinariesSums_extended.qdimacs --stable --clauses weightedBinariesSums; \
      diff test/output/clauses_weightedBinariesSums_extended.qdimacs test/expected/clauses_weightedBinariesSums_extended.qdimacs \
      "
  )
  add_test(NAME clauses_weightedBinariesSums.binaries
      COMMAND bash -c " \
      ./qbfsort --input test/input/binaries.qdimacs --output test/output/clauses_weightedBinariesSums_binaries.qdimacs --stable --clauses weightedBinariesSums; \
      diff test/output/clauses_weightedBinariesSums_binaries.qdimacs test/expected/clauses_weightedBinariesSums_binaries.qdimacs \
      "
  )

  add_test(NAME clauses_weightedBinariesMeans.empty
      COMMAND bash -c " \
      ./qbfsort --input test/input/empty.qdimacs --output test/output/clauses_weightedBinariesMeans_empty.qdimacs --stable --clauses weightedBinariesMeans; \
      diff test/output/clauses_weightedBinariesMeans_empty.qdimacs test/expected/clauses_weightedBinariesMeans_empty.qdimacs \
      "
  )
  add_test(NAME clauses_weightedBinariesMeans.comment
      COMMAND bash -c " \
      ./qbfsort --input test/input/comment.qdimacs --output test/output/clauses_weightedBinariesMeans_comment.qdimacs --stable --clauses weightedBinariesMeans; \
      diff test/output/clauses_weightedBinariesMeans_comment.qdimacs test/expected/clauses_weightedBinariesMeans_comment.qdimacs \
      "
  )
  add_test(NAME clauses_weightedBinariesMeans.short
      COMMAND bash -c " \
      ./qbfsort --input test/input/short.qdimacs --output test/output/clauses_weightedBinariesMeans_short.qdimacs --stable --clauses weightedBinariesMeans; \
      diff test/output/clauses_weightedBinariesMeans_short.qdimacs test/expected/clauses_weightedBinariesMeans_short.qdimacs \
      "
  )
  add_test(NAME clauses_weightedBinariesMeans.extended
      COMMAND bash -c " \
      ./qbfsort --input test/input/extended.qdimacs --output test/output/clauses_weightedBinariesMeans_extended.qdimacs --stable --clauses weightedBinariesMeans; \
      diff test/output/clauses_weightedBinariesMeans_extended.qdimacs test/expected/clauses_weightedBinariesMeans_extended.qdimacs \
      "
  )
  add_test(NAME clauses_weightedBinariesMeans.binaries
      COMMAND bash -c " \
      ./qbfsort --input test/input/binaries.qdimacs --output test/output/clauses_weightedBinariesMeans_binaries.qdimacs --stable --clauses weightedBinariesMeans; \
      diff test/output/clauses_weightedBinariesMeans_binaries.qdimacs test/expected/clauses_weightedBinariesMeans_binaries.qdimacs \
      "
  )
endif()
