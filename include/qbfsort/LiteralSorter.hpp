#ifndef QBFSORT_LITERALSORTER_HPP
#define QBFSORT_LITERALSORTER_HPP

#include "Formula.hpp"

#include <cstdint>
#include <functional>
#include <string>
#include <string_view>
#include <vector>

namespace qbfsort {

class LiteralSorter {
public:
  static constexpr std::string_view metricNone{"none"};
  static constexpr std::string_view metricBasic{"basic"};
  static constexpr std::string_view metricFrequencyLiteral{"frequencyLiteral"};
  static constexpr std::string_view metricFrequencyVariable{
      "frequencyVariable"};
  static constexpr std::string_view metricCountedBinariesLiteral{
      "countedBinariesLiteral"};
  static constexpr std::string_view metricCountedBinariesVariable{
      "countedBinariesVariable"};
  static constexpr std::string_view metricWeightedBinaries{"weightedBinaries"};
  LiteralSorter(const Formula &formula,
                const std::vector<std::string> &metrics);
  bool operator()(std::int32_t left, std::int32_t right) const;

private:
  using compareMethod =
      std::function<std::int32_t(const Formula &, std::int32_t, std::int32_t)>;
  std::vector<compareMethod> compareMethods{};
  static compareMethod getCompareMethod(std::string_view metric);
  static std::int32_t compareNone(const Formula &formula, std::int32_t left,
                                  std::int32_t right);
  static std::int32_t compareBasic(const Formula &formula, std::int32_t left,
                                   std::int32_t right);
  static std::int32_t compareFrequencyLiteral(const Formula &formula,
                                              std::int32_t left,
                                              std::int32_t right);
  static std::int32_t compareFrequencyVariable(const Formula &formula,
                                               std::int32_t left,
                                               std::int32_t right);
  static std::int32_t compareCountedBinariesLiteral(const Formula &formula,
                                                    std::int32_t left,
                                                    std::int32_t right);
  static std::int32_t compareCountedBinariesVariable(const Formula &formula,
                                                     std::int32_t left,
                                                     std::int32_t right);
  static std::int32_t compareWeightedBinaries(const Formula &formula,
                                              std::int32_t left,
                                              std::int32_t right);
  const Formula formula;
};

} // namespace qbfsort

#endif
