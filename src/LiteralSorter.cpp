#include "LiteralSorter.hpp"

LiteralSorter::LiteralSorter(const QbfFormula &formula) : formula{formula} {}

bool LiteralSorter::operator()(std::int32_t left, std::int32_t right) {
  return sort(left, right);
}

BasicLiteralSorter::BasicLiteralSorter(const QbfFormula &formula) : LiteralSorter(formula) {}

bool BasicLiteralSorter::sort(std::int32_t left, std::int32_t right) {
  return left < right;
}

CountedBinariesLiteralSorter::CountedBinariesLiteralSorter(const QbfFormula &formula) : LiteralSorter(formula) {}

bool CountedBinariesLiteralSorter::sort(std::int32_t left, std::int32_t right) {
  return getNewBinariesCount(left) < getNewBinariesCount(right);
}

std::size_t CountedBinariesLiteralSorter::getNewBinariesCount(std::int32_t literal) {
  return formula.getNewBinaryClausesByAssignment(literal).size();
}
