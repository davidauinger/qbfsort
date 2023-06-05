#include <qbfsort/LiteralSorter.hpp>

qbfsort::LiteralSorter::LiteralSorter(const Formula &formula,
                                      const std::vector<std::string> &metrics,
                                      bool isInverse)
    : formula{formula}, isInverse{isInverse} {
  for (const auto &m : metrics) {
    compareMethods.push_back(methodsMap.at(m));
  }
}

bool qbfsort::LiteralSorter::operator()(std::int32_t left,
                                        std::int32_t right) const {
  for (const auto &m : compareMethods) {
    if (auto c{m(formula, left, right)}; c != 0) {
      return isInverse ? c > 0 : c < 0;
    }
  }
  return false;
}

const std::map<std::string_view, qbfsort::LiteralSorter::compareMethod>
    qbfsort::LiteralSorter::methodsMap{
        {metricNone, compareNone},
        {metricBasic, compareBasic},
        {metricFrequencyLiteral, compareFrequencyLiteral},
        {metricFrequencyVariable, compareFrequencyVariable},
        {metricCountedBinariesLiteral, compareCountedBinariesLiteral},
        {metricCountedBinariesVariable, compareCountedBinariesVariable},
        {metricWeightedBinaries, compareWeightedBinaries}};

std::int32_t qbfsort::LiteralSorter::compareNone(const Formula & /*formula*/,
                                                 std::int32_t /*left*/,
                                                 std::int32_t /*right*/) {
  return 0;
}

std::int32_t qbfsort::LiteralSorter::compareBasic(const Formula & /*formula*/,
                                                  std::int32_t left,
                                                  std::int32_t right) {
  return left - right;
}

std::int32_t qbfsort::LiteralSorter::compareFrequencyLiteral(
    const Formula &formula, std::int32_t left, std::int32_t right) {
  return formula.getFrequencyLiteral(right) - formula.getFrequencyLiteral(left);
}

std::int32_t qbfsort::LiteralSorter::compareFrequencyVariable(
    const Formula &formula, std::int32_t left, std::int32_t right) {
  return formula.getFrequencyVariable(right) -
         formula.getFrequencyVariable(left);
}

std::int32_t qbfsort::LiteralSorter::compareCountedBinariesLiteral(
    const Formula &formula, std::int32_t left, std::int32_t right) {
  return formula.getCountedBinariesLiteral(right) -
         formula.getCountedBinariesLiteral(left);
}

std::int32_t qbfsort::LiteralSorter::compareCountedBinariesVariable(
    const Formula &formula, std::int32_t left, std::int32_t right) {
  return formula.getCountedBinariesVariable(right) -
         formula.getCountedBinariesVariable(left);
}

std::int32_t qbfsort::LiteralSorter::compareWeightedBinaries(
    const Formula &formula, std::int32_t left, std::int32_t right) {
  const auto leftWeight{formula.getWeightedBinariesWeight(std::abs(left))};
  const auto rightWeight{formula.getWeightedBinariesWeight(std::abs(right))};
  if (leftWeight > rightWeight) {
    return -1;
  }
  if (leftWeight < rightWeight) {
    return 1;
  }
  return 0;
}
