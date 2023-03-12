#include "QbfFormula.hpp"

#include <algorithm>
#include <cstdlib>
#include <string>

#include <json/json.h>

QbfFormula QbfFormula::fromStream(std::istream &from) {
  QbfFormula formula;
  std::int32_t numberOfClauses;
  std::string token;
  std::int32_t atom;
  from >> token;
  if (token != std::string(PROBLEM_LINE_P)) {
    throw std::runtime_error("invalid token in problem line: expected " + std::string(PROBLEM_LINE_P) + " but got " + token);
  }
  from >> token;
  if (token != std::string(PROBLEM_LINE_CNF)) {
    throw std::runtime_error("invalid token in problem line: expected " + std::string(PROBLEM_LINE_CNF) + " but got " + token);
  }
  from >> formula.numberOfAtoms;
  from >> numberOfClauses;
  from >> token;
  while (Quantifier::isQuantifier(token)) {
    formula.prefix.push_back(std::make_pair(Quantifier::from_string(token), std::vector<std::int32_t>()));
    from >> atom;
    while (atom != 0) {
      if (atom > formula.numberOfAtoms) {
        throw std::runtime_error("unexpected atom in prefix line: expected " + std::to_string(formula.numberOfAtoms) + " distinct atoms but got atom " + std::to_string(atom));
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
        throw std::runtime_error("unexpected literal in matrix line: expected " + std::to_string(formula.numberOfAtoms) + " distinct atoms but got literal " + std::to_string(atom));
      }
      formula.matrix.back().push_back(atom);
      from >> token;
    }
    if (from.eof()) {
      throw std::runtime_error("unexpected end of file during before end of clause");
    }
    from >> token;
  }
  formula.precomputeFrequencies();
  return formula;
}

void QbfFormula::toStream(const QbfFormula &formula, std::ostream &to) {
  to << std::string(PROBLEM_LINE_P) << " " << std::string(PROBLEM_LINE_CNF) << " " << formula.numberOfAtoms << " " << formula.matrix.size() << "\n";
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

void QbfFormula::printStatistics(std::ostream &to) const {
  Json::Value root;
  Json::StreamWriterBuilder builder;
  const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
  root["NumberOfAtoms"] = numberOfAtoms;
  root["NumberOfClauses"] = matrix.size();
  for (std::size_t a{1}; a <= numberOfAtoms; ++a) {
    root["Frequencies"]["PositiveLiterals"].append(getFrequencyLiteral(a));
    root["Frequencies"]["NegativeLiterals"].append(getFrequencyLiteral(-a));
    root["Frequencies"]["Variables"].append(getFrequencyVariable(a));
  }
  for (const std::vector<std::int32_t> &c : matrix) {
    root["Frequencies"]["ClauseLiteralSums"].append(getFrequencyClauseLiteralSum(c));
    root["Frequencies"]["ClauseVariableSums"].append(getFrequencyClauseVariableSum(c));
  }
  writer->write(root, &to);
}

void QbfFormula::sortLiterals(std::function<bool(std::int32_t, std::int32_t)> sorter) {
  for (auto &v : matrix) {
    std::sort(v.begin(), v.end(), sorter);
  }
}

void QbfFormula::sortClauses(std::function<bool(std::vector<std::int32_t>, std::vector<std::int32_t>)> sorter) {
  std::sort(matrix.begin(), matrix.end(), sorter);
}

void QbfFormula::sortQuantifiers(std::function<bool(std::int32_t, std::int32_t)> sorter) {
  for (auto &p : prefix) {
    std::sort(p.second.begin(), p.second.end(), sorter);
  }
}

std::int32_t QbfFormula::getFrequencyLiteral(std::int32_t literal) const {
  return literal > 0 ? frequenciesPositiveLiterals.at(literal) : frequenciesNegativeLiterals.at(-literal);
}

std::int32_t QbfFormula::getFrequencyVariable(std::int32_t variable) const {
  return frequenciesPositiveLiterals.at(variable) + frequenciesNegativeLiterals.at(variable);
}

std::int32_t QbfFormula::getFrequencyClauseLiteralSum(const std::vector<std::int32_t> &clause) const {
  std::int32_t literalSum{0};
  for (std::int32_t l : clause) {
    literalSum += getFrequencyLiteral(l);
  }
  return literalSum;
}

std::int32_t QbfFormula::getFrequencyClauseVariableSum(const std::vector<std::int32_t> &clause) const {
  std::int32_t variableSum{0};
  for (std::int32_t l : clause) {
    variableSum += getFrequencyVariable(std::abs(l));
  }
  return variableSum;
}

std::set<std::pair<std::int32_t, std::int32_t>> QbfFormula::getNewBinaryClausesByAssignment(std::int32_t literal) const {
  std::set<std::pair<std::int32_t, std::int32_t>> newBinaryClauses;
  for (const std::vector<std::int32_t> &v : matrix) {
    if (v.size() == 3 && std::find(v.begin(), v.end(), -literal) != v.end()) {
      newBinaryClauses.emplace(std::make_pair(v[0] != -literal ? v[0] : v[1], v[2] != -literal ? v[2] : v[1]));
    }
  }
  return newBinaryClauses;
}

void QbfFormula::precomputeFrequencies() {
  frequenciesPositiveLiterals.clear();
  frequenciesPositiveLiterals.push_back(0);
  frequenciesNegativeLiterals.clear();
  frequenciesNegativeLiterals.push_back(0);
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
    frequenciesPositiveLiterals.push_back(frequencyPositive);
    frequenciesNegativeLiterals.push_back(frequencyNegative);
  }
}

float QbfFormula::getWeightedBinariesWeight(std::int32_t variable) const {
  if (variable <= 0 || variable > numberOfAtoms) {
    return 0;
  }
  while (weightedBinariesWeights.size() <= numberOfAtoms) {
    /* initial setup */
    weightedBinariesWeights.push_back(-1);
  }
  if (weightedBinariesWeights[variable] < 0) {
    /* compute for the first time */
    weightedBinariesWeights[variable] = computeWeightedBinariesWeight(variable);
  }
  return weightedBinariesWeights[variable];
}

const QbfFormula::Quantifier QbfFormula::Quantifier::exists{QbfFormula::Quantifier("e")};

const QbfFormula::Quantifier QbfFormula::Quantifier::forall{QbfFormula::Quantifier("a")};

const QbfFormula::Quantifier& QbfFormula::Quantifier::from_string(const std::string &token) {
  if (token == exists.token) {
    return exists;
  }
  if (token == forall.token) {
    return forall;
  }
  throw std::runtime_error("invalid token is not a quantifier: " + token);
}

const std::string& QbfFormula::Quantifier::to_string(const Quantifier &quantifier) {
  return quantifier.token;
}

bool QbfFormula::Quantifier::isQuantifier(const std::string &token) {
  return token == exists.token || token == forall.token;
}

QbfFormula::Quantifier::Quantifier(const std::string &token) : token{token} {}

bool QbfFormula::Quantifier::operator==(const Quantifier &other) const {
  return this->token == other.token;
}

float QbfFormula::computeWeightedBinariesWeight(std::int32_t variable) const {
  return getWeightedBinariesHeuristic(variable) * getWeightedBinariesHeuristic(-variable);
}

float QbfFormula::getWeightedBinariesHeuristic(std::int32_t literal) const {
  float weight{0};
  std::set<std::pair<std::int32_t, std::int32_t>> newBinaryClauses{getNewBinaryClausesByAssignment(literal)};
  for (const auto &p : newBinaryClauses) {
    weight += getWeightedBinariesLiteralWeight(-p.first);
    weight += getWeightedBinariesLiteralWeight(-p.second);
  }
  return weight;
}

float QbfFormula::getWeightedBinariesLiteralWeight(std::int32_t literal) const {
  while (weightedBinariesLiteralWeights.size() <= 2 * numberOfAtoms) {
    /* initial setup */
    weightedBinariesLiteralWeights.push_back(-1);
  }
  std::size_t pos{static_cast<std::size_t>(literal > 0 ? literal : numberOfAtoms - literal)};
  if (weightedBinariesLiteralWeights[pos] < 0) {
    /* compute for the first time */
    weightedBinariesWeights[pos] = computeWeightedBinariesLiteralWeight(literal);
  }
  return weightedBinariesLiteralWeights[pos];
}

float QbfFormula::computeWeightedBinariesLiteralWeight(std::int32_t literal) const {
  float weight{0};
  for (const auto &v : matrix) {
    for (auto l : v) {
      if (l == literal) {
        std::int32_t k{static_cast<std::int32_t>(v.size() - 2)};
        float partialWeight{5.0f};
        while (k > 0) {
          partialWeight /= 5.0f;
          --k;
        }
        weight += partialWeight;
        continue;
      }
    }
  }
  return weight;
}
