#include <qbfsort/LiteralSorter.hpp>

#include <cstdlib>

std::shared_ptr<qbfsort::LiteralSorter>
qbfsort::LiteralSorter::create(const std::string &className,
                               QbfFormula &formula,
                               const std::map<std::string, std::string> &args) {
  auto it{factoryMap.find(className)};
  if (it == factoryMap.end()) {
    return nullptr;
  }
  return it->second(formula, args);
}

qbfsort::LiteralSorter::LiteralSorter(const QbfFormula &formula)
    : formula{formula} {}

bool qbfsort::LiteralSorter::operator()(std::int32_t left,
                                        std::int32_t right) const {
  return sort(left, right);
}

std::map<std::string, qbfsort::LiteralSorter::factoryMethod>
    qbfsort::LiteralSorter::factoryMap{
        {std::string(BasicLiteralSorter::CLASS_NAME),
         [](QbfFormula &formula,
            const std::map<std::string, std::string> &args) {
           return std::make_shared<BasicLiteralSorter>(formula);
         }},
        {std::string(FrequencyLiteralSorter::CLASS_NAME),
         [](QbfFormula &formula,
            const std::map<std::string, std::string> &args) {
           return std::make_shared<FrequencyLiteralSorter>(formula);
         }},
        {std::string(CountedBinariesLiteralSorter::CLASS_NAME),
         [](QbfFormula &formula,
            const std::map<std::string, std::string> &args) {
           return std::make_shared<CountedBinariesLiteralSorter>(formula);
         }},
        {std::string(WeightedBinariesLiteralSorter::CLASS_NAME),
         [](QbfFormula &formula,
            const std::map<std::string, std::string> &args) {
           return std::make_shared<WeightedBinariesLiteralSorter>(formula);
         }}};

qbfsort::BasicLiteralSorter::BasicLiteralSorter(const QbfFormula &formula)
    : LiteralSorter(formula) {}

bool qbfsort::BasicLiteralSorter::sort(std::int32_t left,
                                       std::int32_t right) const {
  return left < right;
}

qbfsort::FrequencyLiteralSorter::FrequencyLiteralSorter(
    const QbfFormula &formula)
    : LiteralSorter(formula) {}

bool qbfsort::FrequencyLiteralSorter::sort(std::int32_t left,
                                           std::int32_t right) const {
  return formula.getFrequencyVariable(std::abs(left)) >
         formula.getFrequencyVariable(std::abs(right));
}

qbfsort::CountedBinariesLiteralSorter::CountedBinariesLiteralSorter(
    const QbfFormula &formula)
    : LiteralSorter(formula) {}

bool qbfsort::CountedBinariesLiteralSorter::sort(std::int32_t left,
                                                 std::int32_t right) const {
  return getNewBinariesCount(left) > getNewBinariesCount(right);
}

std::size_t qbfsort::CountedBinariesLiteralSorter::getNewBinariesCount(
    std::int32_t literal) const {
  return formula.getNewBinaryClausesByAssignment(literal).size();
}

qbfsort::WeightedBinariesLiteralSorter::WeightedBinariesLiteralSorter(
    const QbfFormula &formula)
    : LiteralSorter(formula) {}

bool qbfsort::WeightedBinariesLiteralSorter::sort(std::int32_t left,
                                                  std::int32_t right) const {
  return formula.getWeightedBinariesWeight(std::abs(left)) >
         formula.getWeightedBinariesWeight(std::abs(right));
}
