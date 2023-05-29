#include <qbfsort/LiteralSorter.hpp>

qbfsort::LiteralSorter::LiteralSorter(const Formula &formula,
                                      const std::vector<std::string> &metrics)
    : formula{formula} {
  for (const auto &m : metrics) {
    compareMethods.push_back(getCompareMethod(m));
  }
}

bool qbfsort::LiteralSorter::operator()(std::int32_t left,
                                        std::int32_t right) const {
  for (const auto &m : compareMethods) {
    if (auto c{m(formula, left, right)}; c != 0) {
      return c < 0;
    }
  }
  return false;
}

qbfsort::LiteralSorter::compareMethod
qbfsort::LiteralSorter::getCompareMethod(std::string_view metric) {
  if (metric == metricNone) {
    return compareNone;
  }
  if (metric == std::string(metricBasic)) {
    return compareBasic;
  }
  if (metric == std::string(metricFrequencyLiteral)) {
    return compareFrequencyLiteral;
  }
  if (metric == std::string(metricFrequencyVariable)) {
    return compareFrequencyVariable;
  }
  if (metric == std::string(metricCountedBinariesLiteral)) {
    return compareCountedBinariesLiteral;
  }
  if (metric == std::string(metricCountedBinariesVariable)) {
    return compareCountedBinariesVariable;
  }
  if (metric == std::string(metricWeightedBinaries)) {
    return compareWeightedBinaries;
  }
  throw std::runtime_error("unknown metric " + std::string(metric));
}

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
  return formula.getWeightedBinariesWeight(std::abs(right)) -
         formula.getWeightedBinariesWeight(std::abs(left));
}
