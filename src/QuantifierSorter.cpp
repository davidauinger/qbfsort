#include <qbfsort/QuantifierSorter.hpp>

qbfsort::QuantifierSorter::QuantifierSorter(const Formula &formula,
                                            const std::string &metric,
                                            bool isInverse)
    : Sorter(formula, methodsMap.at(metricsMap.at(metric)), isInverse) {}

const std::map<std::string_view, std::string_view> &
qbfsort::QuantifierSorter::getMetricsMap() {
  return metricsMap;
}
const std::map<std::string_view, std::string_view>
    qbfsort::QuantifierSorter::metricsMap{
        {qbfsort::Sorter<std::int32_t>::metricNone,
         qbfsort::Sorter<std::int32_t>::metricNone},
        {qbfsort::Sorter<std::int32_t>::metricBasic,
         qbfsort::Sorter<std::int32_t>::metricBasic},
        {qbfsort::Sorter<std::int32_t>::metricFrequency,
         qbfsort::Sorter<std::int32_t>::metricFrequencyVariable},
        {qbfsort::Sorter<std::int32_t>::metricFrequencyVariable,
         qbfsort::Sorter<std::int32_t>::metricFrequencyVariable},
        {qbfsort::Sorter<std::int32_t>::metricCountedBinaries,
         qbfsort::Sorter<std::int32_t>::metricCountedBinariesVariable},
        {qbfsort::Sorter<std::int32_t>::metricCountedBinariesVariable,
         qbfsort::Sorter<std::int32_t>::metricCountedBinariesVariable},
        {qbfsort::Sorter<std::int32_t>::metricWeightedBinaries,
         qbfsort::Sorter<std::int32_t>::metricWeightedBinaries}};

const std::map<std::string_view, qbfsort::QuantifierSorter::compareMethod>
    qbfsort::QuantifierSorter::methodsMap{
        {qbfsort::Sorter<std::int32_t>::metricNone,
         qbfsort::Sorter<std::int32_t>::compareNone},
        {qbfsort::Sorter<std::int32_t>::metricBasic,
         qbfsort::Sorter<std::int32_t>::compareBasic},
        {qbfsort::Sorter<std::int32_t>::metricFrequencyVariable,
         qbfsort::Sorter<std::int32_t>::compareFrequencyVariable},
        {qbfsort::Sorter<std::int32_t>::metricCountedBinariesVariable,
         qbfsort::Sorter<std::int32_t>::compareCountedBinariesVariable},
        {qbfsort::Sorter<std::int32_t>::metricWeightedBinaries,
         qbfsort::Sorter<std::int32_t>::compareWeightedBinaries}};
