#ifndef QBFSORT_FORMULA_HPP
#define QBFSORT_FORMULA_HPP

#include <functional>
#include <istream>
#include <ostream>
#include <set>
#include <utility>
#include <vector>

namespace qbfsort {

class Formula {
public:
  static Formula fromStream(std::istream &from);
  static void toStream(const Formula &formula, std::ostream &to);
  void printStatistics(std::ostream &to) const;
  void sortLiterals(std::function<bool(std::int32_t, std::int32_t)> sorter);
  void
  stableSortLiterals(std::function<bool(std::int32_t, std::int32_t)> sorter);
  void sortClauses(
      std::function<bool(const std::vector<std::int32_t>&, const std::vector<std::int32_t>&)>
          sorter);
  void stableSortClauses(
      std::function<bool(const std::vector<std::int32_t>&, const std::vector<std::int32_t>&)>
          sorter);
  void sortQuantifiers(std::function<bool(std::int32_t, std::int32_t)> sorter);
  void
  stableSortQuantifiers(std::function<bool(std::int32_t, std::int32_t)> sorter);
  std::int32_t getFrequencyLiteral(std::int32_t literal) const;
  std::int32_t getFrequencyVariable(std::int32_t variable) const;
  std::int32_t
  getFrequencyClauseLiteralSum(const std::vector<std::int32_t> &clause) const;
  double
  getFrequencyClauseLiteralMean(const std::vector<std::int32_t> &clause) const;
  std::int32_t
  getFrequencyClauseVariableSum(const std::vector<std::int32_t> &clause) const;
  double
  getFrequencyClauseVariableMean(const std::vector<std::int32_t> &clause) const;
  std::int32_t getCountedBinariesLiteral(std::int32_t literal) const;
  std::int32_t getCountedBinariesVariable(std::int32_t variable) const;
  std::int32_t getCountedBinariesClauseLiteralSum(
      const std::vector<std::int32_t> &clause) const;
  double getCountedBinariesClauseLiteralMean(
      const std::vector<std::int32_t> &clause) const;
  std::int32_t getCountedBinariesClauseVariableSum(
      const std::vector<std::int32_t> &clause) const;
  double getCountedBinariesClauseVariableMean(
      const std::vector<std::int32_t> &clause) const;
  double getWeightedBinariesWeight(std::int32_t variable) const;
  double getWeightedBinariesWeightClauseSum(
      const std::vector<std::int32_t> &clause) const;
  double getWeightedBinariesWeightClauseMean(
      const std::vector<std::int32_t> &clause) const;

private:
  static constexpr std::string_view COMMENT_LINE_C{"c"};
  static constexpr std::string_view PROBLEM_LINE_P{"p"};
  static constexpr std::string_view PROBLEM_LINE_CNF{"cnf"};
  class Quantifier {
  public:
    static const Quantifier exists;
    static const Quantifier forall;
    static const Quantifier &from_string(const std::string &token);
    static const std::string &to_string(const Quantifier &quantifier);
    static bool isQuantifier(const std::string &token);
    Quantifier(const std::string &token);
    bool operator==(const Quantifier &other) const;

  private:
    const std::string token;
  };
  std::vector<std::string> comments;
  std::int32_t numberOfAtoms;
  std::vector<std::pair<Quantifier, std::vector<std::int32_t>>> prefix;
  std::vector<std::vector<std::int32_t>> matrix;
  mutable std::vector<std::int32_t> frequenciesPositive;
  mutable std::vector<std::int32_t> frequenciesNegative;
  mutable std::vector<std::int32_t> binaryCountsPositive;
  mutable std::vector<std::int32_t> binaryCountsNegative;
  mutable std::vector<double> binaryWeights;
  Formula() = default;
  void precomputeFrequencies() const;
  void precomputeBinaryCounts() const;
  void precomputeBinaryWeights() const;
  std::set<std::pair<std::int32_t, std::int32_t>>
  getNewBinaryClausesByAssignment(std::int32_t literal) const;
  double getWeightedBinariesHeuristic(std::int32_t literal) const;
  double getWeightedBinariesLiteralWeight(std::int32_t literal) const;
};

} // namespace qbfsort

#endif
