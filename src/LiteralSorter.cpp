#include <qbfsort/LiteralSorter.hpp>

qbfsort::LiteralSorter::LiteralSorter(const Formula &formula,
                                      const std::string &metric, bool isInverse)
    : Sorter<std::int32_t>(formula, methodsMap.at(metricsMap.at(metric)),
                           isInverse) {}

const std::map<std::string_view, std::string_view> &
qbfsort::LiteralSorter::getMetricsMap() {
  return metricsMap;
}

const std::map<std::string_view, std::string_view>
    qbfsort::LiteralSorter::metricsMap{
        {qbfsort::Sorter<std::int32_t>::metricNone,
         qbfsort::Sorter<std::int32_t>::metricNone},
        {qbfsort::Sorter<std::int32_t>::metricBasic,
         qbfsort::Sorter<std::int32_t>::metricBasic},
        {qbfsort::Sorter<std::int32_t>::metricRandom,
         qbfsort::Sorter<std::int32_t>::metricRandom},
        {qbfsort::Sorter<std::int32_t>::metricFrequency,
         qbfsort::Sorter<std::int32_t>::metricFrequencyVariable},
        {qbfsort::Sorter<std::int32_t>::metricFrequencyLiteral,
         qbfsort::Sorter<std::int32_t>::metricFrequencyLiteral},
        {qbfsort::Sorter<std::int32_t>::metricFrequencyVariable,
         qbfsort::Sorter<std::int32_t>::metricFrequencyVariable},
        {qbfsort::Sorter<std::int32_t>::metricCountedBinaries,
         qbfsort::Sorter<std::int32_t>::metricCountedBinariesVariable},
        {qbfsort::Sorter<std::int32_t>::metricCountedBinariesLiteral,
         qbfsort::Sorter<std::int32_t>::metricCountedBinariesLiteral},
        {qbfsort::Sorter<std::int32_t>::metricCountedBinariesVariable,
         qbfsort::Sorter<std::int32_t>::metricCountedBinariesVariable},
        {qbfsort::Sorter<std::int32_t>::metricWeightedBinaries,
         qbfsort::Sorter<std::int32_t>::metricWeightedBinaries}};

const std::map<std::string_view, qbfsort::LiteralSorter::compareMethod>
    qbfsort::LiteralSorter::methodsMap{
        {qbfsort::Sorter<std::int32_t>::metricNone,
         qbfsort::Sorter<std::int32_t>::compareNone},
        {qbfsort::Sorter<std::int32_t>::metricBasic,
         qbfsort::Sorter<std::int32_t>::compareBasic},
        {qbfsort::Sorter<std::int32_t>::metricRandom,
         qbfsort::Sorter<std::int32_t>::compareRandom},
        {qbfsort::Sorter<std::int32_t>::metricFrequencyLiteral,
         qbfsort::Sorter<std::int32_t>::compareFrequencyLiteral},
        {qbfsort::Sorter<std::int32_t>::metricFrequencyVariable,
         qbfsort::Sorter<std::int32_t>::compareFrequencyVariable},
        {qbfsort::Sorter<std::int32_t>::metricCountedBinariesLiteral,
         qbfsort::Sorter<std::int32_t>::compareCountedBinariesLiteral},
        {qbfsort::Sorter<std::int32_t>::metricCountedBinariesVariable,
         qbfsort::Sorter<std::int32_t>::compareCountedBinariesVariable},
        {qbfsort::Sorter<std::int32_t>::metricWeightedBinaries,
         qbfsort::Sorter<std::int32_t>::compareWeightedBinaries}};
