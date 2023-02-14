#include "ClauseSorter.hpp"

std::shared_ptr<ClauseSorter> ClauseSorter::create(const std::string &className, QbfFormula &formula, const std::map<std::string, std::string> &args) {
  auto it{factoryMap.find(className)};
  if (it == factoryMap.end()) {
    return nullptr;
  }
  return it->second(formula, args);
}

ClauseSorter::ClauseSorter(const QbfFormula &formula) : formula{formula} {}

bool ClauseSorter::operator()(const std::vector<std::int32_t> &left, const std::vector<std::int32_t> &right) const {
  return sort(left, right);
}

std::map<std::string, ClauseSorter::factoryMethod> ClauseSorter::factoryMap{
  {std::string(BasicClauseSorter::CLASS_NAME), [] (QbfFormula &formula, const std::map<std::string, std::string> &args) {
    return std::make_shared<BasicClauseSorter>(formula);
  }},
  {std::string(CountedBinariesClauseSorter::CLASS_NAME), [] (QbfFormula &formula, const std::map<std::string, std::string> &args) {
    return std::make_shared<CountedBinariesClauseSorter>(formula);
  }},
  {std::string(WeightedBinariesClauseSorter::CLASS_NAME), [] (QbfFormula &formula, const std::map<std::string, std::string> &args) {
    return std::make_shared<WeightedBinariesClauseSorter>(formula);
  }}
};

BasicClauseSorter::BasicClauseSorter(const QbfFormula &formula) : ClauseSorter(formula) {}

bool BasicClauseSorter::sort(const std::vector<std::int32_t> &left, const std::vector<std::int32_t> &right) const {
  return left < right;
}

CountedBinariesClauseSorter::CountedBinariesClauseSorter(const QbfFormula &formula) : ClauseSorter(formula) {}

bool CountedBinariesClauseSorter::sort(const std::vector<std::int32_t> &left, const std::vector<std::int32_t> &right) const {
  return getNewBinariesCount(left) < getNewBinariesCount(right);
}

std::size_t CountedBinariesClauseSorter::getNewBinariesCount(const std::vector<std::int32_t> &literals) const {
  std::set<std::pair<std::int32_t, std::int32_t>> newBinaryClauses;
  for (auto l : literals) {
    newBinaryClauses.merge(formula.getNewBinaryClausesByAssignment(l));
  }
  return newBinaryClauses.size();
}

WeightedBinariesClauseSorter::WeightedBinariesClauseSorter(const QbfFormula &formula) : ClauseSorter(formula) {}

bool WeightedBinariesClauseSorter::sort(const std::vector<std::int32_t> &left, const std::vector<std::int32_t> &right) const {
  return getWeights(left) < getWeights(right);
}

float WeightedBinariesClauseSorter::getWeights(const std::vector<std::int32_t> &literals) const {
  float weight{0.0f};
  for (auto l : literals) {
    weight += formula.getWeightedBinariesWeight(l > 0 ? l : -l);
  }
  return weight;
}
