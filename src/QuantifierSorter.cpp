#include <qbfsort/QuantifierSorter.hpp>

#include <cstdlib>

std::shared_ptr<qbfsort::QuantifierSorter> qbfsort::QuantifierSorter::create(
    const std::string &className, QbfFormula &formula,
    const std::map<std::string, std::string> &args) {
  auto it{factoryMap.find(className)};
  if (it == factoryMap.end()) {
    return nullptr;
  }
  return it->second(formula, args);
}

qbfsort::QuantifierSorter::QuantifierSorter(const QbfFormula &formula)
    : formula{formula} {}

bool qbfsort::QuantifierSorter::operator()(std::int32_t left,
                                           std::int32_t right) const {
  return sort(left, right);
}

std::map<std::string, qbfsort::QuantifierSorter::factoryMethod>
    qbfsort::QuantifierSorter::factoryMap{
        {std::string(BasicQuantifierSorter::CLASS_NAME),
         [](QbfFormula &formula,
            const std::map<std::string, std::string> &args) {
           return std::make_shared<BasicQuantifierSorter>(formula);
         }},
        {std::string(FrequencyQuantifierSorter::CLASS_NAME),
         [](QbfFormula &formula,
            const std::map<std::string, std::string> &args) {
           return std::make_shared<FrequencyQuantifierSorter>(formula);
         }},
        {std::string(CountedBinariesQuantifierSorter::CLASS_NAME),
         [](QbfFormula &formula,
            const std::map<std::string, std::string> &args) {
           return std::make_shared<CountedBinariesQuantifierSorter>(formula);
         }},
        {std::string(WeightedBinariesQuantifierSorter::CLASS_NAME),
         [](QbfFormula &formula,
            const std::map<std::string, std::string> &args) {
           return std::make_shared<WeightedBinariesQuantifierSorter>(formula);
         }}};

qbfsort::BasicQuantifierSorter::BasicQuantifierSorter(const QbfFormula &formula)
    : QuantifierSorter(formula) {}

bool qbfsort::BasicQuantifierSorter::sort(std::int32_t left,
                                          std::int32_t right) const {
  return left < right;
}

qbfsort::FrequencyQuantifierSorter::FrequencyQuantifierSorter(
    const QbfFormula &formula)
    : QuantifierSorter(formula) {}

bool qbfsort::FrequencyQuantifierSorter::sort(std::int32_t left,
                                              std::int32_t right) const {
  return formula.getFrequencyVariable(std::abs(left)) >
         formula.getFrequencyVariable(std::abs(right));
}

qbfsort::CountedBinariesQuantifierSorter::CountedBinariesQuantifierSorter(
    const QbfFormula &formula)
    : QuantifierSorter(formula) {}

bool qbfsort::CountedBinariesQuantifierSorter::sort(std::int32_t left,
                                                    std::int32_t right) const {
  return getNewBinariesCount(left) > getNewBinariesCount(right);
}

std::size_t qbfsort::CountedBinariesQuantifierSorter::getNewBinariesCount(
    std::int32_t literal) const {
  return formula.getNewBinaryClausesByAssignment(literal).size() +
         formula.getNewBinaryClausesByAssignment(-literal).size();
}

qbfsort::WeightedBinariesQuantifierSorter::WeightedBinariesQuantifierSorter(
    const QbfFormula &formula)
    : QuantifierSorter(formula) {}

bool qbfsort::WeightedBinariesQuantifierSorter::sort(std::int32_t left,
                                                     std::int32_t right) const {
  return formula.getWeightedBinariesWeight(std::abs(left)) >
         formula.getWeightedBinariesWeight(std::abs(right));
}
