#include <qbfsort/ClauseSorter.hpp>

qbfsort::ClauseSorter::ClauseSorter(const Formula &formula,
                                    const std::string &metric, bool isInverse)
    : Sorter(formula, methodsMap.at(metricsMap.at(metric)), isInverse) {}

const std::map<std::string_view, std::string_view> &
qbfsort::ClauseSorter::getMetricsMap() {
  return metricsMap;
}

const std::map<std::string_view, std::string_view>
    qbfsort::ClauseSorter::metricsMap{
        {qbfsort::Sorter<std::vector<std::int32_t>>::metricNone,
         qbfsort::Sorter<std::vector<std::int32_t>>::metricNone},
        {qbfsort::Sorter<std::vector<std::int32_t>>::metricBasic,
         qbfsort::Sorter<std::vector<std::int32_t>>::metricBasic},
        {qbfsort::Sorter<std::vector<std::int32_t>>::metricRandom,
         qbfsort::Sorter<std::vector<std::int32_t>>::metricRandom},
        {qbfsort::Sorter<std::vector<std::int32_t>>::metricFrequency,
         qbfsort::Sorter<
             std::vector<std::int32_t>>::metricFrequencyVariableMeans},
        {qbfsort::Sorter<std::vector<std::int32_t>>::metricFrequencyLiteral,
         qbfsort::Sorter<
             std::vector<std::int32_t>>::metricFrequencyLiteralMeans},
        {qbfsort::Sorter<std::vector<std::int32_t>>::metricFrequencyLiteralSums,
         qbfsort::Sorter<
             std::vector<std::int32_t>>::metricFrequencyLiteralSums},
        {qbfsort::Sorter<
             std::vector<std::int32_t>>::metricFrequencyLiteralMeans,
         qbfsort::Sorter<
             std::vector<std::int32_t>>::metricFrequencyLiteralMeans},
        {qbfsort::Sorter<std::vector<std::int32_t>>::metricFrequencyVariable,
         qbfsort::Sorter<
             std::vector<std::int32_t>>::metricFrequencyVariableMeans},
        {qbfsort::Sorter<
             std::vector<std::int32_t>>::metricFrequencyVariableSums,
         qbfsort::Sorter<
             std::vector<std::int32_t>>::metricFrequencyVariableSums},
        {qbfsort::Sorter<
             std::vector<std::int32_t>>::metricFrequencyVariableMeans,
         qbfsort::Sorter<
             std::vector<std::int32_t>>::metricFrequencyVariableMeans},
        {qbfsort::Sorter<std::vector<std::int32_t>>::metricCountedBinaries,
         qbfsort::Sorter<
             std::vector<std::int32_t>>::metricCountedBinariesVariableMeans},
        {qbfsort::Sorter<
             std::vector<std::int32_t>>::metricCountedBinariesLiteral,
         qbfsort::Sorter<
             std::vector<std::int32_t>>::metricCountedBinariesLiteralMeans},
        {qbfsort::Sorter<
             std::vector<std::int32_t>>::metricCountedBinariesLiteralSums,
         qbfsort::Sorter<
             std::vector<std::int32_t>>::metricCountedBinariesLiteralSums},
        {qbfsort::Sorter<
             std::vector<std::int32_t>>::metricCountedBinariesLiteralMeans,
         qbfsort::Sorter<
             std::vector<std::int32_t>>::metricCountedBinariesLiteralMeans},
        {qbfsort::Sorter<
             std::vector<std::int32_t>>::metricCountedBinariesVariable,
         qbfsort::Sorter<
             std::vector<std::int32_t>>::metricCountedBinariesVariableMeans},
        {qbfsort::Sorter<
             std::vector<std::int32_t>>::metricCountedBinariesVariableSums,
         qbfsort::Sorter<
             std::vector<std::int32_t>>::metricCountedBinariesVariableSums},
        {qbfsort::Sorter<
             std::vector<std::int32_t>>::metricCountedBinariesVariableMeans,
         qbfsort::Sorter<
             std::vector<std::int32_t>>::metricCountedBinariesVariableMeans},
        {qbfsort::Sorter<std::vector<std::int32_t>>::metricWeightedBinaries,
         qbfsort::Sorter<
             std::vector<std::int32_t>>::metricWeightedBinariesMeans},
        {qbfsort::Sorter<std::vector<std::int32_t>>::metricWeightedBinariesSums,
         qbfsort::Sorter<
             std::vector<std::int32_t>>::metricWeightedBinariesSums},
        {qbfsort::Sorter<
             std::vector<std::int32_t>>::metricWeightedBinariesMeans,
         qbfsort::Sorter<
             std::vector<std::int32_t>>::metricWeightedBinariesMeans}};

const std::map<std::string_view, qbfsort::ClauseSorter::compareMethod>
    qbfsort::ClauseSorter::methodsMap{
        {qbfsort::Sorter<std::vector<std::int32_t>>::metricNone,
         qbfsort::Sorter<std::vector<std::int32_t>>::compareNone},
        {qbfsort::Sorter<std::vector<std::int32_t>>::metricBasic,
         qbfsort::Sorter<std::vector<std::int32_t>>::compareBasic},
        {qbfsort::Sorter<std::vector<std::int32_t>>::metricRandom,
         qbfsort::Sorter<std::vector<std::int32_t>>::compareRandom},
        {qbfsort::Sorter<std::vector<std::int32_t>>::metricFrequencyLiteralSums,
         qbfsort::Sorter<
             std::vector<std::int32_t>>::compareFrequencyLiteralSums},
        {qbfsort::Sorter<
             std::vector<std::int32_t>>::metricFrequencyLiteralMeans,
         qbfsort::Sorter<
             std::vector<std::int32_t>>::compareFrequencyLiteralMeans},
        {qbfsort::Sorter<
             std::vector<std::int32_t>>::metricFrequencyVariableSums,
         qbfsort::Sorter<
             std::vector<std::int32_t>>::compareFrequencyVariableSums},
        {qbfsort::Sorter<
             std::vector<std::int32_t>>::metricFrequencyVariableMeans,
         qbfsort::Sorter<
             std::vector<std::int32_t>>::compareFrequencyVariableMeans},
        {qbfsort::Sorter<
             std::vector<std::int32_t>>::metricCountedBinariesLiteralSums,
         qbfsort::Sorter<
             std::vector<std::int32_t>>::compareCountedBinariesLiteralSums},
        {qbfsort::Sorter<
             std::vector<std::int32_t>>::metricCountedBinariesLiteralMeans,
         qbfsort::Sorter<
             std::vector<std::int32_t>>::compareCountedBinariesLiteralMeans},
        {qbfsort::Sorter<
             std::vector<std::int32_t>>::metricCountedBinariesVariableSums,
         qbfsort::Sorter<
             std::vector<std::int32_t>>::compareCountedBinariesVariableSums},
        {qbfsort::Sorter<
             std::vector<std::int32_t>>::metricCountedBinariesVariableMeans,
         qbfsort::Sorter<
             std::vector<std::int32_t>>::compareCountedBinariesVariableMeans},
        {qbfsort::Sorter<std::vector<std::int32_t>>::metricWeightedBinariesSums,
         qbfsort::Sorter<
             std::vector<std::int32_t>>::compareWeightedBinariesSums},
        {qbfsort::Sorter<
             std::vector<std::int32_t>>::metricWeightedBinariesMeans,
         qbfsort::Sorter<
             std::vector<std::int32_t>>::compareWeightedBinariesMeans}};
