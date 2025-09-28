#include <qbfsort/Sorter.hpp>

template <typename T>
bool qbfsort::Sorter<T>::operator()(const T &left, const T &right) const {
  if (auto c{compare(formula, left, right)}; c != 0) {
    return isInverse ? c > 0 : c < 0;
  }
  return false;
}

template <typename T>
qbfsort::Sorter<T>::Sorter(const Formula &formula, compareMethod compare,
                           bool isInverse)
    : formula{formula}, compare{compare}, isInverse{isInverse} {}

template <typename T>
std::int32_t qbfsort::Sorter<T>::compareNone(const Formula & /*formula*/,
                                             const T & /*left*/,
                                             const T & /*right*/) {
  return 0;
}

template <>
std::int32_t qbfsort::Sorter<std::int32_t>::compareBasic(
    const qbfsort::Formula & /*formula*/, const std::int32_t &left,
    const std::int32_t &right) {
  return left - right;
}

template <>
std::int32_t qbfsort::Sorter<std::vector<std::int32_t>>::compareBasic(
    const qbfsort::Formula & /*formula*/, const std::vector<std::int32_t> &left,
    const std::vector<std::int32_t> &right) {
  if (left < right) {
    return -1;
  }
  if (left > right) {
    return 1;
  }
  return 0;
}

template <>
std::int32_t
qbfsort::Sorter<std::int32_t>::compareRandom(const qbfsort::Formula &formula,
                                             const std::int32_t &left,
                                             const std::int32_t &right) {
  generator.seed(formula.getHashcode() + left);
  auto leftValue{generator()};
  generator.seed(formula.getHashcode() + right);
  auto rightValue{generator()};
  return leftValue - rightValue;
}

template <>
std::int32_t qbfsort::Sorter<std::vector<std::int32_t>>::compareRandom(
    const qbfsort::Formula &formula, const std::vector<std::int32_t> &left,
    const std::vector<std::int32_t> &right) {
  std::vector<std::int32_t> leftValue(left), rightValue(right);
  for (std::size_t i{0}; i < left.size(); ++i) {
    generator.seed(formula.getHashcode() + left[i]);
    generator();
    leftValue[i] = generator();
  }
  for (std::size_t i{0}; i < right.size(); ++i) {
    generator.seed(formula.getHashcode() + right[i]);
    rightValue[i] = generator();
  }
  if (leftValue < rightValue) {
    return -1;
  }
  if (leftValue > rightValue) {
    return 1;
  }
  return 0;
}

template <>
std::int32_t qbfsort::Sorter<std::int32_t>::compareFrequencyLiteral(
    const Formula &formula, std::int32_t left, std::int32_t right) {
  return formula.getFrequencyLiteral(right) - formula.getFrequencyLiteral(left);
}

template <>
std::int32_t qbfsort::Sorter<std::int32_t>::compareFrequencyVariable(
    const Formula &formula, std::int32_t left, std::int32_t right) {
  return formula.getFrequencyVariable(right) -
         formula.getFrequencyVariable(left);
}

template <>
std::int32_t
qbfsort::Sorter<std::vector<std::int32_t>>::compareFrequencyLiteralSums(
    const Formula &formula, const std::vector<std::int32_t> &left,
    const std::vector<std::int32_t> &right) {
  return formula.getFrequencyClauseLiteralSum(right) -
         formula.getFrequencyClauseLiteralSum(left);
}

template <>
std::int32_t
qbfsort::Sorter<std::vector<std::int32_t>>::compareFrequencyLiteralMeans(
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

template <>
std::int32_t
qbfsort::Sorter<std::vector<std::int32_t>>::compareFrequencyVariableSums(
    const Formula &formula, const std::vector<std::int32_t> &left,
    const std::vector<std::int32_t> &right) {
  return formula.getFrequencyClauseVariableSum(right) -
         formula.getFrequencyClauseVariableSum(left);
}

template <>
std::int32_t
qbfsort::Sorter<std::vector<std::int32_t>>::compareFrequencyVariableMeans(
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

template <>
std::int32_t qbfsort::Sorter<std::int32_t>::compareCountedBinariesLiteral(
    const Formula &formula, std::int32_t left, std::int32_t right) {
  return formula.getCountedBinariesLiteral(right) -
         formula.getCountedBinariesLiteral(left);
}

template <>
std::int32_t qbfsort::Sorter<std::int32_t>::compareCountedBinariesVariable(
    const Formula &formula, std::int32_t left, std::int32_t right) {
  return formula.getCountedBinariesVariable(right) -
         formula.getCountedBinariesVariable(left);
}

template <>
std::int32_t
qbfsort::Sorter<std::vector<std::int32_t>>::compareCountedBinariesLiteralSums(
    const Formula &formula, const std::vector<std::int32_t> &left,
    const std::vector<std::int32_t> &right) {
  return formula.getCountedBinariesClauseLiteralSum(right) -
         formula.getCountedBinariesClauseLiteralSum(left);
}

template <>
std::int32_t
qbfsort::Sorter<std::vector<std::int32_t>>::compareCountedBinariesLiteralMeans(
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

template <>
std::int32_t
qbfsort::Sorter<std::vector<std::int32_t>>::compareCountedBinariesVariableSums(
    const Formula &formula, const std::vector<std::int32_t> &left,
    const std::vector<std::int32_t> &right) {
  return formula.getCountedBinariesClauseVariableSum(right) -
         formula.getCountedBinariesClauseVariableSum(left);
}

template <>
std::int32_t
qbfsort::Sorter<std::vector<std::int32_t>>::compareCountedBinariesVariableMeans(
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

template <>
std::int32_t qbfsort::Sorter<std::int32_t>::compareWeightedBinaries(
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

template <>
std::int32_t
qbfsort::Sorter<std::vector<std::int32_t>>::compareWeightedBinariesSums(
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

template <>
std::int32_t
qbfsort::Sorter<std::vector<std::int32_t>>::compareWeightedBinariesMeans(
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

template <typename T> std::mt19937 qbfsort::Sorter<T>::generator;

template class qbfsort::Sorter<std::int32_t>;

template class qbfsort::Sorter<std::vector<std::int32_t>>;
