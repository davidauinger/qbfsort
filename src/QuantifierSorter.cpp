#include "QuantifierSorter.hpp"

QuantifierSorter::QuantifierSorter(const QbfFormula &formula) : formula{formula} {}

bool QuantifierSorter::operator()(std::int32_t left, std::int32_t right) {
  return sort(left, right);
}

BasicQuantifierSorter::BasicQuantifierSorter(const QbfFormula &formula) : QuantifierSorter(formula) {}

bool BasicQuantifierSorter::sort(std::int32_t left, std::int32_t right) {
  return left < right;
}

CountedBinariesQuantifierSorter::CountedBinariesQuantifierSorter(const QbfFormula &formula) : QuantifierSorter(formula) {}

bool CountedBinariesQuantifierSorter::sort(std::int32_t left, std::int32_t right) {
  return getNewBinariesCount(left) < getNewBinariesCount(right);
}

std::size_t CountedBinariesQuantifierSorter::getNewBinariesCount(std::int32_t literal) {
  return formula.getNewBinaryClausesByAssignment(literal).size();
}
