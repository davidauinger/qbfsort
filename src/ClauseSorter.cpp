#include "ClauseSorter.hpp"

ClauseSorter::ClauseSorter(const QbfFormula &formula) : formula{formula} {}

bool ClauseSorter::operator()(std::vector<std::int32_t> left, std::vector<std::int32_t> right) {
  return sort(left, right);
}

BasicClauseSorter::BasicClauseSorter(const QbfFormula &formula) : ClauseSorter(formula) {}

bool BasicClauseSorter::sort(std::vector<std::int32_t> left, std::vector<std::int32_t> right) {
  return left < right;
}

CountedBinariesClauseSorter::CountedBinariesClauseSorter(const QbfFormula &formula) : ClauseSorter(formula) {}

bool CountedBinariesClauseSorter::sort(std::vector<std::int32_t> left, std::vector<std::int32_t> right) {
  return getNewBinariesCount(left) < getNewBinariesCount(right);
}

std::size_t CountedBinariesClauseSorter::getNewBinariesCount(std::vector<std::int32_t> literals) {
  std::set<std::pair<std::int32_t, std::int32_t>> newBinaryClauses;
  for (auto l : literals) {
    newBinaryClauses.merge(formula.getNewBinaryClausesByAssignment(l));
  }
  return newBinaryClauses.size();
}
