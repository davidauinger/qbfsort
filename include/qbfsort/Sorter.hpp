#ifndef QBFSORT_SORTER_HPP
#define QBFSORT_SORTER_HPP

#include "Formula.hpp"

#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace qbfsort {

template <typename T> class Sorter {
public:
  static constexpr std::string_view metricNone{"none"};
  static constexpr std::string_view metricBasic{"basic"};
  static constexpr std::string_view metricFrequency{"frequency"};
  static constexpr std::string_view metricFrequencyLiteral{"frequencyLiteral"};
  static constexpr std::string_view metricFrequencyVariable{
      "frequencyVariable"};
  static constexpr std::string_view metricFrequencyLiteralSums{
      "frequencyLiteralSums"};
  static constexpr std::string_view metricFrequencyLiteralMeans{
      "frequencyLiteralMeans"};
  static constexpr std::string_view metricFrequencyVariableSums{
      "frequencyVariableSums"};
  static constexpr std::string_view metricFrequencyVariableMeans{
      "frequencyVariableMeans"};
  static constexpr std::string_view metricCountedBinaries{"countedBinaries"};
  static constexpr std::string_view metricCountedBinariesLiteral{
      "countedBinariesLiteral"};
  static constexpr std::string_view metricCountedBinariesVariable{
      "countedBinariesVariable"};
  static constexpr std::string_view metricCountedBinariesLiteralSums{
      "countedBinariesLiteralSums"};
  static constexpr std::string_view metricCountedBinariesLiteralMeans{
      "countedBinariesLiteralMeans"};
  static constexpr std::string_view metricCountedBinariesVariableSums{
      "countedBinariesVariableSums"};
  static constexpr std::string_view metricCountedBinariesVariableMeans{
      "countedBinariesVariableMeans"};
  static constexpr std::string_view metricWeightedBinaries{"weightedBinaries"};
  static constexpr std::string_view metricWeightedBinariesSums{
      "weightedBinariesSums"};
  static constexpr std::string_view metricWeightedBinariesMeans{
      "weightedBinariesMeans"};
  bool operator()(const T &left, const T &right) const;

protected:
  using compareMethod =
      std::function<std::int32_t(const Formula &, const T &, const T &)>;
  Sorter(const Formula &formula, compareMethod compare, bool isInverse);
  static std::int32_t compareNone(const Formula &formula, const T &left,
                                  const T &right);
  static std::int32_t compareBasic(const Formula &formula, const T &left,
                                   const T &right);
  static std::int32_t compareFrequencyLiteral(const Formula &formula,
                                              std::int32_t left,
                                              std::int32_t right);
  static std::int32_t compareFrequencyVariable(const Formula &formula,
                                               std::int32_t left,
                                               std::int32_t right);
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
  static std::int32_t compareCountedBinariesLiteral(const Formula &formula,
                                                    std::int32_t left,
                                                    std::int32_t right);
  static std::int32_t compareCountedBinariesVariable(const Formula &formula,
                                                     std::int32_t left,
                                                     std::int32_t right);
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
  static std::int32_t compareWeightedBinaries(const Formula &formula,
                                              std::int32_t left,
                                              std::int32_t right);
  static std::int32_t
  compareWeightedBinariesSums(const Formula &formula,
                              const std::vector<std::int32_t> &left,
                              const std::vector<std::int32_t> &right);
  static std::int32_t
  compareWeightedBinariesMeans(const Formula &formula,
                               const std::vector<std::int32_t> &left,
                               const std::vector<std::int32_t> &right);

private:
  const Formula formula;
  const compareMethod compare;
  const bool isInverse;
};

} // namespace qbfsort

#endif
