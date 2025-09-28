#ifndef QBFSORT_CLAUSESORTER_HPP
#define QBFSORT_CLAUSESORTER_HPP

#include "Formula.hpp"

#include <functional>
#include <string_view>
#include <vector>

namespace qbfsort {

class ClauseSorter {
public:
  static constexpr std::string_view metricNone{"none"};
  static constexpr std::string_view metricBasic{"basic"};
  static constexpr std::string_view metricFrequencyLiteralSums{"frequencyLiteralSums"};
  static constexpr std::string_view metricFrequencyLiteralMeans{"frequencyLiteralMeans"};
  static constexpr std::string_view metricFrequencyVariableSums{
      "frequencyVariableSums"};
  static constexpr std::string_view metricFrequencyVariableMeans{
      "frequencyVariableMeans"};
  static constexpr std::string_view metricCountedBinariesLiteralSums{
      "countedBinariesLiteralSums"};
  static constexpr std::string_view metricCountedBinariesLiteralMeans{
      "countedBinariesLiteralMeans"};
  static constexpr std::string_view metricCountedBinariesVariableSums{
      "countedBinariesVariableSums"};
  static constexpr std::string_view metricCountedBinariesVariableMeans{
      "countedBinariesVariableMeans"};
  static constexpr std::string_view metricWeightedBinariesSums{"weightedBinariesSums"};
  static constexpr std::string_view metricWeightedBinariesMeans{"weightedBinariesMeans"};
  ClauseSorter(const Formula &formula, const std::vector<std::string> &metrics,
               bool isInverse = false);
  bool operator()(const std::vector<std::int32_t> &left,
                  const std::vector<std::int32_t> &right) const;

private:
  using compareMethod = std::function<std::int32_t(
      const Formula &formula, const std::vector<std::int32_t> &,
      const std::vector<std::int32_t> &)>;
  std::vector<compareMethod> compareMethods{};
  static compareMethod getCompareMethod(std::string_view metric);
  static std::int32_t compareNone(const Formula &formula,
                                  const std::vector<std::int32_t> &left,
                                  const std::vector<std::int32_t> &right);
  static std::int32_t compareBasic(const Formula &formula,
                                   const std::vector<std::int32_t> &left,
                                   const std::vector<std::int32_t> &right);
  static std::int32_t
  compareFrequencyLiteralSums(const Formula &formula,
                          const std::vector<std::int32_t> &left,
                          const std::vector<std::int32_t> &right);
  static std::int32_t
  compareFrequencyLiteralMeans(const Formula &formula,
                          const std::vector<std::int32_t> &left,
                          const std::vector<std::int32_t> &right);
  static std::int32_t
  compareFrequencyVariableSums(const Formula &formula,
                           const std::vector<std::int32_t> &left,
                           const std::vector<std::int32_t> &right);
  static std::int32_t
  compareFrequencyVariableMeans(const Formula &formula,
                           const std::vector<std::int32_t> &left,
                           const std::vector<std::int32_t> &right);
  static std::int32_t
  compareCountedBinariesLiteralSums(const Formula &formula,
                                const std::vector<std::int32_t> &left,
                                const std::vector<std::int32_t> &right);
  static std::int32_t
  compareCountedBinariesLiteralMeans(const Formula &formula,
                                const std::vector<std::int32_t> &left,
                                const std::vector<std::int32_t> &right);
  static std::int32_t
  compareCountedBinariesVariableSums(const Formula &formula,
                                 const std::vector<std::int32_t> &left,
                                 const std::vector<std::int32_t> &right);
  static std::int32_t
  compareCountedBinariesVariableMeans(const Formula &formula,
                                 const std::vector<std::int32_t> &left,
                                 const std::vector<std::int32_t> &right);
  static std::int32_t
  compareWeightedBinariesSums(const Formula &formula,
                          const std::vector<std::int32_t> &left,
                          const std::vector<std::int32_t> &right);
  static std::int32_t
  compareWeightedBinariesMeans(const Formula &formula,
                          const std::vector<std::int32_t> &left,
                          const std::vector<std::int32_t> &right);
  const Formula formula;
  const bool isInverse;
};

} // namespace qbfsort

#endif
