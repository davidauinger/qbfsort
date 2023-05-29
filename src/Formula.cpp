#include <qbfsort/Formula.hpp>

#include <algorithm>
#include <cstdlib>
#include <string>

#include <json/json.h>

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

void qbfsort::Formula::sortLiterals(
    std::function<bool(std::int32_t, std::int32_t)> sorter) {
  for (auto &v : matrix) {
    std::sort(v.begin(), v.end(), sorter);
  }
}

void qbfsort::Formula::stableSortLiterals(
    std::function<bool(std::int32_t, std::int32_t)> sorter) {
  for (auto &v : matrix) {
    std::stable_sort(v.begin(), v.end(), sorter);
  }
}

void qbfsort::Formula::sortClauses(
    std::function<bool(const std::vector<std::int32_t>&, const std::vector<std::int32_t>&)>
        sorter) {
  std::sort(matrix.begin(), matrix.end(), sorter);
}

void qbfsort::Formula::stableSortClauses(
    std::function<bool(const std::vector<std::int32_t>&, const std::vector<std::int32_t>&)>
        sorter) {
  std::stable_sort(matrix.begin(), matrix.end(), sorter);
}

void qbfsort::Formula::sortQuantifiers(
    std::function<bool(std::int32_t, std::int32_t)> sorter) {
  for (auto &p : prefix) {
    std::sort(p.second.begin(), p.second.end(), sorter);
  }
}

void qbfsort::Formula::stableSortQuantifiers(
    std::function<bool(std::int32_t, std::int32_t)> sorter) {
  for (auto &p : prefix) {
    std::stable_sort(p.second.begin(), p.second.end(), sorter);
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
  if (binaryCountsPositive.empty() || binaryCountsNegative.empty()) {
    precomputeBinaryCounts();
  }
  return literal > 0 ? binaryCountsPositive.at(literal)
                     : binaryCountsNegative.at(-literal);
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
  if (binaryWeights.empty()) {
    precomputeBinaryWeights();
  }
  return binaryWeights.at(variable);
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

void qbfsort::Formula::precomputeFrequencies() const {
  frequenciesPositive.clear();
  frequenciesPositive.push_back(0);
  frequenciesNegative.clear();
  frequenciesNegative.push_back(0);
  for (std::int32_t a{1}; a <= numberOfAtoms; ++a) {
    std::int32_t frequencyPositive{0};
    std::int32_t frequencyNegative{0};
    for (const std::vector<std::int32_t> &v : matrix) {
      if (std::count(v.begin(), v.end(), a)) {
        ++frequencyPositive;
      }
      if (std::count(v.begin(), v.end(), -a)) {
        ++frequencyNegative;
      }
    }
    frequenciesPositive.push_back(frequencyPositive);
    frequenciesNegative.push_back(frequencyNegative);
  }
}

void qbfsort::Formula::precomputeBinaryCounts() const {
  binaryCountsPositive.clear();
  binaryCountsPositive.push_back(0);
  binaryCountsNegative.clear();
  binaryCountsNegative.push_back(0);
  for (std::int32_t a{1}; a <= numberOfAtoms; ++a) {
    binaryCountsPositive.push_back(getNewBinaryClausesByAssignment(a).size());
    binaryCountsNegative.push_back(getNewBinaryClausesByAssignment(-a).size());
  }
}

void qbfsort::Formula::precomputeBinaryWeights() const {
  binaryWeights.clear();
  binaryWeights.push_back(0);
  for (std::int32_t a{1}; a <= numberOfAtoms; ++a) {
    binaryWeights.push_back(getWeightedBinariesHeuristic(a) *
                            getWeightedBinariesHeuristic(-a));
  }
}

std::set<std::pair<std::int32_t, std::int32_t>>
qbfsort::Formula::getNewBinaryClausesByAssignment(std::int32_t literal) const {
  std::set<std::pair<std::int32_t, std::int32_t>> newBinaryClauses;
  for (std::vector<std::int32_t> v : matrix) {
    if (std::find(v.begin(), v.end(), -literal) == v.end()) {
      continue;
    }
    v.erase(std::remove(v.begin(), v.end(), -literal), v.end());
    if (v.size() == 2) {
      if (std::find(std::cbegin(matrix), std::end(matrix), v) !=
          std::cend(matrix)) {
        continue;
      }
      std::reverse(v.begin(), v.end());
      if (std::find(std::cbegin(matrix), std::cend(matrix), v) !=
          std::cend(matrix)) {
        continue;
      }
      std::sort(v.begin(), v.end());
      newBinaryClauses.emplace(v[0], v[1]);
    }
  }
  return newBinaryClauses;
}

double
qbfsort::Formula::getWeightedBinariesHeuristic(std::int32_t literal) const {
  double weight{0.0};
  std::set<std::pair<std::int32_t, std::int32_t>> newBinaryClauses{
      getNewBinaryClausesByAssignment(literal)};
  for (const auto &p : newBinaryClauses) {
    weight += getWeightedBinariesLiteralWeight(-p.first);
    weight += getWeightedBinariesLiteralWeight(-p.second);
  }
  return weight;
}

double
qbfsort::Formula::getWeightedBinariesLiteralWeight(std::int32_t literal) const {
  double weight{0.0};
  for (const auto &c : matrix) {
    if (c.size() > 1 && std::find(c.cbegin(), c.cend(), literal) != c.cend()) {
      std::int32_t k{static_cast<std::int32_t>(c.size() - 2)};
      double partialWeight{5.0};
      while (k > 0) {
        partialWeight /= 5.0;
        --k;
      }
      weight += partialWeight;
    }
  }
  return weight;
}
