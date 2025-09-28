#ifndef QBFFORMULA
#define QBFFORMULA

#include <istream>
#include <functional>
#include <ostream>
#include <set>
#include <utility>
#include <vector>

class QbfFormula
{
public:
  static QbfFormula fromStream(std::istream &from);
  static void toStream(const QbfFormula &formula, std::ostream &to);
  void sortLiterals(std::function<bool(std::int32_t, std::int32_t)> sorter);
  void sortClauses(std::function<bool(std::vector<std::int32_t>, std::vector<std::int32_t>)> sorter);
  void sortQuantifiers(std::function<bool(std::int32_t, std::int32_t)> sorter);
  std::set<std::pair<std::int32_t, std::int32_t>> getNewBinaryClausesByAssignment(std::int32_t literal) const;
private:
  static constexpr std::string_view PROBLEM_LINE_P{"p"};
  static constexpr std::string_view PROBLEM_LINE_CNF{"cnf"};
  class Quantifier
  {
  public:
    static const Quantifier exists;
    static const Quantifier forall;
    static const Quantifier& from_string(const std::string &token);
    static const std::string& to_string(const Quantifier &quantifier);
  static bool isQuantifier(const std::string &token);
    Quantifier(const std::string &token);
    bool operator==(const Quantifier &other) const;
  private:
    const std::string token;
  };
  QbfFormula() = default;
  std::int32_t numberOfAtoms;
  std::vector<std::pair<Quantifier, std::vector<std::int32_t>>> prefix;
  std::vector<std::vector<std::int32_t>> matrix;
};

#endif
