#include <qbfsort/ClauseSorter.hpp>

#include <cstdlib>

std::shared_ptr<qbfsort::ClauseSorter>
qbfsort::ClauseSorter::create(const std::string &className,
                              qbfsort::QbfFormula &formula,
                              const std::map<std::string, std::string> &args) {
  auto it{factoryMap.find(className)};
  if (it == factoryMap.end()) {
    return nullptr;
  }
  return it->second(formula, args);
}

qbfsort::ClauseSorter::ClauseSorter(const QbfFormula &formula)
    : formula{formula} {}

bool qbfsort::ClauseSorter::operator()(
    const std::vector<std::int32_t> &left,
    const std::vector<std::int32_t> &right) const {
  return sort(left, right);
}

std::map<std::string, qbfsort::ClauseSorter::factoryMethod>
    qbfsort::ClauseSorter::factoryMap{
        {std::string(BasicClauseSorter::CLASS_NAME),
         [](QbfFormula &formula,
            const std::map<std::string, std::string> &args) {
           return std::make_shared<BasicClauseSorter>(formula);
         }},
        {std::string(FrequencyClauseSorter::CLASS_NAME),
         [](QbfFormula &formula,
            const std::map<std::string, std::string> &args) {
           return std::make_shared<FrequencyClauseSorter>(formula);
         }},
        {std::string(CountedBinariesClauseSorter::CLASS_NAME),
         [](QbfFormula &formula,
            const std::map<std::string, std::string> &args) {
           return std::make_shared<CountedBinariesClauseSorter>(formula);
         }},
        {std::string(WeightedBinariesClauseSorter::CLASS_NAME),
         [](QbfFormula &formula,
            const std::map<std::string, std::string> &args) {
           return std::make_shared<WeightedBinariesClauseSorter>(formula);
         }}};

qbfsort::BasicClauseSorter::BasicClauseSorter(const QbfFormula &formula)
    : ClauseSorter(formula) {}

bool qbfsort::BasicClauseSorter::sort(
    const std::vector<std::int32_t> &left,
    const std::vector<std::int32_t> &right) const {
  return left < right;
}

qbfsort::FrequencyClauseSorter::FrequencyClauseSorter(const QbfFormula &formula)
    : ClauseSorter(formula) {}

bool qbfsort::FrequencyClauseSorter::sort(
    const std::vector<std::int32_t> &left,
    const std::vector<std::int32_t> &right) const {
  return formula.getFrequencyClauseVariableSum(left) >
         formula.getFrequencyClauseVariableSum(right);
}

qbfsort::CountedBinariesClauseSorter::CountedBinariesClauseSorter(
    const QbfFormula &formula)
    : ClauseSorter(formula) {}

bool qbfsort::CountedBinariesClauseSorter::sort(
    const std::vector<std::int32_t> &left,
    const std::vector<std::int32_t> &right) const {
  return getNewBinariesCount(left) > getNewBinariesCount(right);
}

std::size_t qbfsort::CountedBinariesClauseSorter::getNewBinariesCount(
    const std::vector<std::int32_t> &literals) const {
  std::set<std::pair<std::int32_t, std::int32_t>> newBinaryClauses;
  for (auto l : literals) {
    newBinaryClauses.merge(formula.getNewBinaryClausesByAssignment(l));
  }
  return newBinaryClauses.size();
}

qbfsort::WeightedBinariesClauseSorter::WeightedBinariesClauseSorter(
    const QbfFormula &formula)
    : ClauseSorter(formula) {}

bool qbfsort::WeightedBinariesClauseSorter::sort(
    const std::vector<std::int32_t> &left,
    const std::vector<std::int32_t> &right) const {
  return getWeights(left) > getWeights(right);
}

float qbfsort::WeightedBinariesClauseSorter::getWeights(
    const std::vector<std::int32_t> &literals) const {
  float weight{0.0f};
  for (auto l : literals) {
    weight += formula.getWeightedBinariesWeight(std::abs(l));
  }
  return weight;
}
