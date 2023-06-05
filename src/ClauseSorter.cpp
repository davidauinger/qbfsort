#include <qbfsort/ClauseSorter.hpp>

#include <cstdlib>

qbfsort::ClauseSorter::ClauseSorter(const Formula &formula,
                                    const std::vector<std::string> &metrics,
                                    bool isInverse)
    : formula{formula}, isInverse{isInverse} {
  for (const auto &m : metrics) {
    compareMethods.push_back(getCompareMethod(m));
  }
}

bool qbfsort::ClauseSorter::operator()(
    const std::vector<std::int32_t> &left,
    const std::vector<std::int32_t> &right) const {
  for (const auto &m : compareMethods) {
    if (auto c{m(formula, left, right)}; c != 0) {
      return isInverse ? c > 0 : c < 0;
    }
  }
  return false;
}

qbfsort::ClauseSorter::compareMethod
qbfsort::ClauseSorter::getCompareMethod(std::string_view metric) {
  if (metric == metricNone) {
    return compareNone;
  }
  if (metric == std::string(metricBasic)) {
    return compareBasic;
  }
  if (metric == std::string(metricFrequencyLiteralSums)) {
    return compareFrequencyLiteralSums;
  }
  if (metric == std::string(metricFrequencyLiteralMeans)) {
    return compareFrequencyLiteralMeans;
  }
  if (metric == std::string(metricFrequencyVariableSums)) {
    return compareFrequencyVariableSums;
  }
  if (metric == std::string(metricFrequencyVariableMeans)) {
    return compareFrequencyVariableMeans;
  }
  if (metric == std::string(metricCountedBinariesLiteralSums)) {
    return compareCountedBinariesLiteralSums;
  }
  if (metric == std::string(metricCountedBinariesLiteralMeans)) {
    return compareCountedBinariesLiteralMeans;
  }
  if (metric == std::string(metricCountedBinariesVariableSums)) {
    return compareCountedBinariesVariableSums;
  }
  if (metric == std::string(metricCountedBinariesVariableMeans)) {
    return compareCountedBinariesVariableMeans;
  }
  if (metric == std::string(metricWeightedBinariesSums)) {
    return compareWeightedBinariesSums;
  }
  if (metric == std::string(metricWeightedBinariesMeans)) {
    return compareWeightedBinariesMeans;
  }
  throw std::runtime_error("unknown metric " + std::string(metric));
}

std::int32_t qbfsort::ClauseSorter::compareNone(
    const Formula & /*formula*/, const std::vector<std::int32_t> & /*left*/,
    const std::vector<std::int32_t> & /*right*/) {
  return 0;
}

std::int32_t
qbfsort::ClauseSorter::compareBasic(const Formula & /*formula*/,
                                    const std::vector<std::int32_t> &left,
                                    const std::vector<std::int32_t> &right) {
  if (left < right) {
    return -1;
  }
  if (left > right) {
    return 1;
  }
  return 0;
}

std::int32_t qbfsort::ClauseSorter::compareFrequencyLiteralSums(
    const Formula &formula, const std::vector<std::int32_t> &left,
    const std::vector<std::int32_t> &right) {
  return formula.getFrequencyClauseLiteralSum(right) -
         formula.getFrequencyClauseLiteralSum(left);
}

std::int32_t qbfsort::ClauseSorter::compareFrequencyLiteralMeans(
    const Formula &formula, const std::vector<std::int32_t> &left,
    const std::vector<std::int32_t> &right) {
  const auto leftMean{formula.getFrequencyClauseLiteralMean(left)};
  const auto rightMean{formula.getFrequencyClauseLiteralMean(right)};
  if (leftMean > rightMean) {
    return -1;
  }
  if (leftMean < rightMean) {
    return 1;
  }
  return 0;
}

std::int32_t qbfsort::ClauseSorter::compareFrequencyVariableSums(
    const Formula &formula, const std::vector<std::int32_t> &left,
    const std::vector<std::int32_t> &right) {
  return formula.getFrequencyClauseVariableSum(right) -
         formula.getFrequencyClauseVariableSum(left);
}

std::int32_t qbfsort::ClauseSorter::compareFrequencyVariableMeans(
    const Formula &formula, const std::vector<std::int32_t> &left,
    const std::vector<std::int32_t> &right) {
  const auto leftMean{formula.getFrequencyClauseVariableMean(left)};
  const auto rightMean{formula.getFrequencyClauseVariableMean(right)};
  if (leftMean > rightMean) {
    return -1;
  }
  if (leftMean < rightMean) {
    return 1;
  }
  return 0;
}

std::int32_t qbfsort::ClauseSorter::compareCountedBinariesLiteralSums(
    const Formula &formula, const std::vector<std::int32_t> &left,
    const std::vector<std::int32_t> &right) {
  return formula.getCountedBinariesClauseLiteralSum(right) -
         formula.getCountedBinariesClauseLiteralSum(left);
}

std::int32_t qbfsort::ClauseSorter::compareCountedBinariesLiteralMeans(
    const Formula &formula, const std::vector<std::int32_t> &left,
    const std::vector<std::int32_t> &right) {
  const auto leftMean{formula.getCountedBinariesClauseLiteralMean(left)};
  const auto rightMean{formula.getCountedBinariesClauseLiteralMean(right)};
  if (leftMean > rightMean) {
    return -1;
  }
  if (leftMean < rightMean) {
    return 1;
  }
  return 0;
}

std::int32_t qbfsort::ClauseSorter::compareCountedBinariesVariableSums(
    const Formula &formula, const std::vector<std::int32_t> &left,
    const std::vector<std::int32_t> &right) {
  return formula.getCountedBinariesClauseVariableSum(right) -
         formula.getCountedBinariesClauseVariableSum(left);
}

std::int32_t qbfsort::ClauseSorter::compareCountedBinariesVariableMeans(
    const Formula &formula, const std::vector<std::int32_t> &left,
    const std::vector<std::int32_t> &right) {
  const auto leftMean{formula.getCountedBinariesClauseVariableMean(left)};
  const auto rightMean{formula.getCountedBinariesClauseVariableMean(right)};
  if (leftMean > rightMean) {
    return -1;
  }
  if (leftMean < rightMean) {
    return 1;
  }
  return 0;
}

std::int32_t qbfsort::ClauseSorter::compareWeightedBinariesSums(
    const Formula &formula, const std::vector<std::int32_t> &left,
    const std::vector<std::int32_t> &right) {
  const auto leftSum{formula.getWeightedBinariesWeightClauseSum(left)};
  const auto rightSum{formula.getWeightedBinariesWeightClauseSum(right)};
  if (leftSum > rightSum) {
    return -1;
  }
  if (leftSum < rightSum) {
    return 1;
  }
  return 0;
}

std::int32_t qbfsort::ClauseSorter::compareWeightedBinariesMeans(
    const Formula &formula, const std::vector<std::int32_t> &left,
    const std::vector<std::int32_t> &right) {
  const auto leftMean{formula.getWeightedBinariesWeightClauseMean(left)};
  const auto rightMean{formula.getWeightedBinariesWeightClauseMean(right)};
  if (leftMean > rightMean) {
    return -1;
  }
  if (leftMean < rightMean) {
    return 1;
  }
  return 0;
}
