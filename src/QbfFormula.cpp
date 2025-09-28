#include "QbfFormula.hpp"

#include <algorithm>
#include <cstdlib>
#include <string>

#include <iostream>

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
    from >> token;
  }
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

std::set<std::pair<std::int32_t, std::int32_t>> QbfFormula::getNewBinaryClausesByAssignment(std::int32_t literal) const {
  std::set<std::pair<std::int32_t, std::int32_t>> newBinaryClauses;
  for (const std::vector<std::int32_t> v : matrix) {
    auto it{std::find(v.begin(), v.end(), -literal)};
    if (v.size() == 3 && std::find(v.begin(), v.end(), -literal) != v.end()) {
      newBinaryClauses.emplace(std::make_pair(v[0] != -literal ? v[0] : v[1], v[2] != -literal ? v[2] : v[1]));
    }
  }
  return newBinaryClauses;
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

QbfFormula::Quantifier::Quantifier(const std::string &token) : token{token} {}

bool QbfFormula::Quantifier::operator==(const Quantifier &other) const {
  return this->token == other.token;
}

bool QbfFormula::Quantifier::isQuantifier(const std::string &token) {
  return token == exists.token || token == forall.token;
}
