#include <qbfsort/Formula.hpp>

#include <algorithm>
#include <cstdlib>
#include <string>

#ifdef WITH_JSONCPP
#include <json/json.h>
#endif

qbfsort::Formula qbfsort::Formula::fromStream(std::istream &from) {
  Formula formula;
  std::int32_t numberOfClauses;
  std::string token;
  std::int32_t atom;
  from >> token;
  while (token == std::string(COMMENT_LINE_C)) {
    std::string commentLine{};
    std::getline(from, commentLine);
    formula.comments.push_back(commentLine);
    from >> token;
  }
  if (token != std::string(PROBLEM_LINE_P)) {
    throw std::runtime_error("invalid token in problem line: expected " +
                             std::string(PROBLEM_LINE_P) + " but got " + token);
  }
  from >> token;
  if (token != std::string(PROBLEM_LINE_CNF)) {
    throw std::runtime_error("invalid token in problem line: expected " +
                             std::string(PROBLEM_LINE_CNF) + " but got " +
                             token);
  }
  from >> formula.numberOfAtoms;
  from >> numberOfClauses;
  from >> token;
  while (Quantifier::isQuantifier(token)) {
    formula.prefix.push_back(std::make_pair(Quantifier::from_string(token),
                                            std::vector<std::int32_t>()));
    from >> atom;
    while (atom != 0) {
      if (atom > formula.numberOfAtoms) {
        throw std::runtime_error("unexpected atom in prefix line: expected " +
                                 std::to_string(formula.numberOfAtoms) +
                                 " distinct atoms but got atom " +
                                 std::to_string(atom));
      }
      formula.prefix.back().second.push_back(atom);
      from >> atom;
    }
    from >> token;
  }
  while (!from.eof()) {
    formula.matrix.push_back(std::vector<std::int32_t>());
    while (token != "0") {
      atom = std::stoi(token);
      if (std::abs(atom) > formula.numberOfAtoms) {
        throw std::runtime_error(
            "unexpected literal in matrix line: expected " +
            std::to_string(formula.numberOfAtoms) +
            " distinct atoms but got literal " + std::to_string(atom));
      }
      formula.matrix.back().push_back(atom);
      from >> token;
    }
    if (from.eof()) {
      throw std::runtime_error(
          "unexpected end of file during before end of clause");
    }
    from >> token;
  }
  return formula;
}

void qbfsort::Formula::toStream(const Formula &formula, std::ostream &to) {
  for (const auto &commentLine : formula.comments) {
    to << std::string(COMMENT_LINE_C) << commentLine << "\n";
  }
  to << std::string(PROBLEM_LINE_P) << " " << std::string(PROBLEM_LINE_CNF)
     << " " << formula.numberOfAtoms << " " << formula.matrix.size() << "\n";
  for (const auto &p : formula.prefix) {
    to << Quantifier::to_string(p.first);
    for (auto a : p.second) {
      to << " " << std::to_string(a);
    }
    to << " 0\n";
  }
  for (const auto &v : formula.matrix) {
    for (auto l : v) {
      to << std::to_string(l) << " ";
    }
    to << "0\n";
  }
}

#ifdef WITH_JSONCPP
void qbfsort::Formula::printStatistics(std::ostream &to) const {
  Json::Value root;
  Json::StreamWriterBuilder builder;
  builder["precision"] = 15;
  const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
  root["NumberOfAtoms"] = numberOfAtoms;
  root["NumberOfClauses"] = matrix.size();
  root["Frequencies"]["PositiveLiterals"].resize(0);
  root["Frequencies"]["NegativeLiterals"].resize(0);
  root["Frequencies"]["Variables"].resize(0);
  for (std::size_t a{1}; a <= numberOfAtoms; ++a) {
    root["Frequencies"]["PositiveLiterals"].append(getFrequencyLiteral(a));
    root["Frequencies"]["NegativeLiterals"].append(getFrequencyLiteral(-a));
    root["Frequencies"]["Variables"].append(getFrequencyVariable(a));
  }
  root["Frequencies"]["ClauseLiteralSums"].resize(0);
  root["Frequencies"]["ClauseLiteralMeans"].resize(0);
  root["Frequencies"]["ClauseVariableSums"].resize(0);
  root["Frequencies"]["ClauseVariableMeans"].resize(0);
  for (const std::vector<std::int32_t> &c : matrix) {
    root["Frequencies"]["ClauseLiteralSums"].append(
        getFrequencyClauseLiteralSum(c));
    root["Frequencies"]["ClauseLiteralMeans"].append(
        getFrequencyClauseLiteralMean(c));
    root["Frequencies"]["ClauseVariableSums"].append(
        getFrequencyClauseVariableSum(c));
    root["Frequencies"]["ClauseVariableMeans"].append(
        getFrequencyClauseVariableMean(c));
  }
  root["CountedBinaries"]["PositiveLiterals"].resize(0);
  root["CountedBinaries"]["NegativeLiterals"].resize(0);
  root["CountedBinaries"]["Variables"].resize(0);
  for (std::size_t a{1}; a <= numberOfAtoms; ++a) {
    root["CountedBinaries"]["PositiveLiterals"].append(
        getCountedBinariesLiteral(a));
    root["CountedBinaries"]["NegativeLiterals"].append(
        getCountedBinariesLiteral(-a));
    root["CountedBinaries"]["Variables"].append(getCountedBinariesVariable(a));
  }
  root["CountedBinaries"]["ClauseLiteralSums"].resize(0);
  root["CountedBinaries"]["ClauseLiteralMeans"].resize(0);
  root["CountedBinaries"]["ClauseVariableSums"].resize(0);
  root["CountedBinaries"]["ClauseVariableMeans"].resize(0);
  for (const std::vector<std::int32_t> &c : matrix) {
    root["CountedBinaries"]["ClauseLiteralSums"].append(
        getCountedBinariesClauseLiteralSum(c));
    root["CountedBinaries"]["ClauseLiteralMeans"].append(
        getCountedBinariesClauseLiteralMean(c));
    root["CountedBinaries"]["ClauseVariableSums"].append(
        getCountedBinariesClauseVariableSum(c));
    root["CountedBinaries"]["ClauseVariableMeans"].append(
        getCountedBinariesClauseVariableMean(c));
  }
  root["WeightedBinaries"]["Variables"].resize(0);
  for (std::size_t a{1}; a <= numberOfAtoms; ++a) {
    root["WeightedBinaries"]["Variables"].append(getWeightedBinariesWeight(a));
  }
  root["WeightedBinaries"]["ClauseVariableSums"].resize(0);
  root["WeightedBinaries"]["ClauseVariableMeans"].resize(0);
  for (const std::vector<std::int32_t> &c : matrix) {
    root["WeightedBinaries"]["ClauseVariableSums"].append(
        getWeightedBinariesWeightClauseSum(c));
    root["WeightedBinaries"]["ClauseVariableMeans"].append(
        getWeightedBinariesWeightClauseMean(c));
  }
  writer->write(root, &to);
}
#endif

void qbfsort::Formula::sortLiterals(
    const std::function<bool(std::int32_t, std::int32_t)> &sorter) {
  for (auto &v : matrix) {
    std::sort(v.begin(), v.end(), std::cref(sorter));
  }
}

void qbfsort::Formula::stableSortLiterals(
    const std::function<bool(std::int32_t, std::int32_t)> &sorter) {
  for (auto &v : matrix) {
    std::stable_sort(v.begin(), v.end(), std::cref(sorter));
  }
}

void qbfsort::Formula::sortClauses(
    const std::function<bool(const std::vector<std::int32_t> &,
                             const std::vector<std::int32_t> &)> &sorter) {
  std::sort(matrix.begin(), matrix.end(), std::cref(sorter));
}

void qbfsort::Formula::stableSortClauses(
    const std::function<bool(const std::vector<std::int32_t> &,
                             const std::vector<std::int32_t> &)> &sorter) {
  std::stable_sort(matrix.begin(), matrix.end(), std::cref(sorter));
}

void qbfsort::Formula::sortQuantifiers(
    const std::function<bool(std::int32_t, std::int32_t)> &sorter) {
  for (auto &p : prefix) {
    std::sort(p.second.begin(), p.second.end(), std::cref(sorter));
  }
}

void qbfsort::Formula::stableSortQuantifiers(
    const std::function<bool(std::int32_t, std::int32_t)> &sorter) {
  for (auto &p : prefix) {
    std::stable_sort(p.second.begin(), p.second.end(), std::cref(sorter));
  }
}

std::int32_t qbfsort::Formula::getFrequencyLiteral(std::int32_t literal) const {
  if (frequenciesPositive.empty() || frequenciesNegative.empty()) {
    precomputeFrequencies();
  }
  return literal > 0 ? frequenciesPositive.at(literal)
                     : frequenciesNegative.at(-literal);
}

std::int32_t
qbfsort::Formula::getFrequencyVariable(std::int32_t variable) const {
  return getFrequencyLiteral(variable) + getFrequencyLiteral(-variable);
}

std::int32_t qbfsort::Formula::getFrequencyClauseLiteralSum(
    const std::vector<std::int32_t> &clause) const {
  std::int32_t literalSum{0};
  for (std::int32_t l : clause) {
    literalSum += getFrequencyLiteral(l);
  }
  return literalSum;
}

double qbfsort::Formula::getFrequencyClauseLiteralMean(
    const std::vector<std::int32_t> &clause) const {
  return static_cast<double>(getFrequencyClauseLiteralSum(clause)) /
         static_cast<double>(clause.size());
}

std::int32_t qbfsort::Formula::getFrequencyClauseVariableSum(
    const std::vector<std::int32_t> &clause) const {
  std::int32_t variableSum{0};
  for (std::int32_t l : clause) {
    variableSum += getFrequencyVariable(std::abs(l));
  }
  return variableSum;
}

double qbfsort::Formula::getFrequencyClauseVariableMean(
    const std::vector<std::int32_t> &clause) const {
  return static_cast<double>(getFrequencyClauseVariableSum(clause)) /
         static_cast<double>(clause.size());
}

std::int32_t
qbfsort::Formula::getCountedBinariesLiteral(std::int32_t literal) const {
  return getNewBinaryClauses(literal).size();
}

std::int32_t
qbfsort::Formula::getCountedBinariesVariable(std::int32_t variable) const {
  return getCountedBinariesLiteral(variable) +
         getCountedBinariesLiteral(-variable);
}

std::int32_t qbfsort::Formula::getCountedBinariesClauseLiteralSum(
    const std::vector<std::int32_t> &clause) const {
  std::int32_t literalSum{0};
  for (std::int32_t l : clause) {
    literalSum += getCountedBinariesLiteral(l);
  }
  return literalSum;
}

double qbfsort::Formula::getCountedBinariesClauseLiteralMean(
    const std::vector<std::int32_t> &clause) const {
  return static_cast<double>(getCountedBinariesClauseLiteralSum(clause)) /
         static_cast<double>(clause.size());
}

std::int32_t qbfsort::Formula::getCountedBinariesClauseVariableSum(
    const std::vector<std::int32_t> &clause) const {
  std::int32_t variableSum{0};
  for (std::int32_t l : clause) {
    variableSum += getCountedBinariesVariable(std::abs(l));
  }
  return variableSum;
}

double qbfsort::Formula::getCountedBinariesClauseVariableMean(
    const std::vector<std::int32_t> &clause) const {
  return static_cast<double>(getCountedBinariesClauseVariableSum(clause)) /
         static_cast<double>(clause.size());
}

double
qbfsort::Formula::getWeightedBinariesWeight(std::int32_t variable) const {
  if (weightedBinariesWeights.empty()) {
    precomputeWeightedBinariesWeights();
  }
  return weightedBinariesWeights.at(variable);
}

double qbfsort::Formula::getWeightedBinariesWeightClauseSum(
    const std::vector<std::int32_t> &clause) const {
  double weight{0.0};
  for (auto l : clause) {
    weight += getWeightedBinariesWeight(std::abs(l));
  }
  return weight;
}

double qbfsort::Formula::getWeightedBinariesWeightClauseMean(
    const std::vector<std::int32_t> &clause) const {
  return getWeightedBinariesWeightClauseSum(clause) /
         static_cast<double>(clause.size());
}

const qbfsort::Formula::Quantifier qbfsort::Formula::Quantifier::exists{
    Formula::Quantifier("e")};

const qbfsort::Formula::Quantifier qbfsort::Formula::Quantifier::forall{
    Formula::Quantifier("a")};

const qbfsort::Formula::Quantifier &
qbfsort::Formula::Quantifier::from_string(const std::string &token) {
  if (token == exists.token) {
    return exists;
  }
  if (token == forall.token) {
    return forall;
  }
  throw std::runtime_error("invalid token is not a quantifier: " + token);
}

const std::string &
qbfsort::Formula::Quantifier::to_string(const Quantifier &quantifier) {
  return quantifier.token;
}

bool qbfsort::Formula::Quantifier::isQuantifier(const std::string &token) {
  return token == exists.token || token == forall.token;
}

qbfsort::Formula::Quantifier::Quantifier(const std::string &token)
    : token{token} {}

bool qbfsort::Formula::Quantifier::operator==(const Quantifier &other) const {
  return this->token == other.token;
}

std::vector<std::pair<std::int32_t, std::pair<std::int32_t, std::int32_t>>>
qbfsort::Formula::getAllBinaryClausesFromAssignment(
    const std::set<std::int32_t> &clause) {
  std::vector<std::pair<std::int32_t, std::pair<std::int32_t, std::int32_t>>>
      binaryClauses;
  if (clause.size() != 3) {
    return binaryClauses;
  }
  for (auto literal : clause) {
    auto newBinaryClauseSet{clause};
    newBinaryClauseSet.erase(literal);
    binaryClauses.push_back({literal,
                             {*std::min_element(newBinaryClauseSet.cbegin(),
                                                newBinaryClauseSet.cend()),
                              *std::max_element(newBinaryClauseSet.cbegin(),
                                                newBinaryClauseSet.cend())}});
  }
  return binaryClauses;
}

double qbfsort::Formula::getPartialWeight(std::int32_t size) {
  if (size < 2) {
    return 0.0;
  }
  double partialWeight{5.0};
  while (size > 2) {
    partialWeight /= 5.0;
    --size;
  }
  return partialWeight;
}

const std::set<std::pair<std::int32_t, std::int32_t>> &
qbfsort::Formula::getBinaryClauses() const {
  if (binaryClauses.empty()) {
    precomputeBinaryClauses();
  }
  return binaryClauses;
}

const std::set<std::pair<std::int32_t, std::int32_t>> &
qbfsort::Formula::getNewBinaryClauses(std::int32_t literal) const {
  if (newBinaryClausesPositive.empty() || newBinaryClausesNegative.empty()) {
    precomputeNewBinaryClauses();
  }
  return literal > 0 ? newBinaryClausesPositive.at(literal)
                     : newBinaryClausesNegative.at(-literal);
}

double qbfsort::Formula::getBinaryWeight(std::int32_t literal) const {
  double weight{0.0};
  for (auto [literal1, literal2] : getNewBinaryClauses(literal)) {
    weight += getLiteralWeight(-literal1);
    weight += getLiteralWeight(-literal2);
  }
  return weight;
}

double qbfsort::Formula::getLiteralWeight(std::int32_t literal) const {
  if (literalWeightsPositive.empty() || literalWeightsNegative.empty()) {
    precomputeLiteralWeights();
  }
  return literal > 0 ? literalWeightsPositive[literal]
                     : literalWeightsNegative[-literal];
}

void qbfsort::Formula::precomputeFrequencies() const {
  frequenciesPositive = std::vector<std::int32_t>(numberOfAtoms + 1, 0);
  frequenciesNegative = std::vector<std::int32_t>(numberOfAtoms + 1, 0);
  for (const auto &clause : matrix) {
    for (auto literal : clause) {
      if (literal > 0) {
        ++frequenciesPositive[literal];
      } else {
        ++frequenciesNegative[-literal];
      }
    }
  }
}

void qbfsort::Formula::precomputeBinaryClauses() const {
  binaryClauses.clear();
  for (const std::vector<std::int32_t> &clause : matrix) {
    std::set<std::int32_t> clauseSet;
    for (std::size_t i{0}; i < clause.size() && clauseSet.size() <= 2; ++i) {
      clauseSet.insert(clause[i]);
    }
    if (clauseSet.size() == 2) {
      binaryClauses.insert(
          {std::min(clause[0], clause[1]), std::max(clause[0], clause[1])});
    }
  }
}

void qbfsort::Formula::precomputeNewBinaryClauses() const {
  newBinaryClausesPositive =
      std::vector<std::set<std::pair<std::int32_t, std::int32_t>>>(
          numberOfAtoms + 1);
  newBinaryClausesNegative =
      std::vector<std::set<std::pair<std::int32_t, std::int32_t>>>(
          numberOfAtoms + 1);
  for (const std::vector<std::int32_t> &clause : matrix) {
    std::set<std::int32_t> clauseSet;
    for (std::size_t i{0}; i < clause.size() && clauseSet.size() <= 3; ++i) {
      clauseSet.insert(clause[i]);
    }
    for (const auto &[literal, newBinaryClause] :
         getAllBinaryClausesFromAssignment(clauseSet)) {
      if (getBinaryClauses().find(newBinaryClause) !=
          getBinaryClauses().cend()) {
        continue;
      }
      if (literal > 0) {
        newBinaryClausesNegative[literal].insert(newBinaryClause);
      } else {
        newBinaryClausesPositive[-literal].insert(newBinaryClause);
      }
    }
  }
}

void qbfsort::Formula::precomputeWeightedBinariesWeights() const {
  weightedBinariesWeights = std::vector<double>(numberOfAtoms + 1, 0.0);
  for (std::int32_t atom{1}; atom <= numberOfAtoms; ++atom) {
    weightedBinariesWeights[atom] =
        getBinaryWeight(atom) * getBinaryWeight(-atom);
  }
}

void qbfsort::Formula::precomputeLiteralWeights() const {
  literalWeightsPositive = std::vector<double>(numberOfAtoms + 1, 0.0);
  literalWeightsNegative = std::vector<double>(numberOfAtoms + 1, 0.0);
  for (const auto &clause : matrix) {
    std::set<std::int32_t> clauseSet;
    for (auto literal : clause) {
      clauseSet.insert(literal);
    }
    for (auto literal : clauseSet) {
      if (literal > 0) {
        literalWeightsPositive[literal] += getPartialWeight(clauseSet.size());
      } else {
        literalWeightsNegative[-literal] += getPartialWeight(clauseSet.size());
      }
    }
  }
}
