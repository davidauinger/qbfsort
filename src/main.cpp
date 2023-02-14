#include <cstring>
#include <iostream>
#include <fstream>
#include <memory>
#include <map>
#include <vector>
#include <string>

#include "QbfFormula.hpp"
#include "LiteralSorter.hpp"
#include "ClauseSorter.hpp"
#include "QuantifierSorter.hpp"

std::map<std::string, std::string> parseArgs(int argc, char **argv);
void sortLiterals(QbfFormula &formula, const std::map<std::string, std::string> &args);
void sortClauses(QbfFormula &formula, const std::map<std::string, std::string> &args);
void sortQuantifiers(QbfFormula &formula, const std::map<std::string, std::string> &args);

int main(int argc, char **argv) {
  QbfFormula f{QbfFormula::fromStream(std::cin)};
  std::map<std::string, std::string> args{parseArgs(argc, argv)};
  sortLiterals(f, args);
  sortClauses(f, args);
  sortQuantifiers(f, args);
  QbfFormula::toStream(f, std::cout);
  return 0;
}

std::map<std::string, std::string> parseArgs(int argc, char **argv) {
  std::map<std::string, std::string> args;
  for (int i{1}; i < argc; ++i) {
    char *str{std::strpbrk(argv[i], "=")};
    const std::string &key{str ? std::string(argv[i], str - argv[i]) : argv[i]};
    const std::string &value{str ? str + 1 : ""};
    args.emplace(key , value);
  }
  return args;
}

void sortLiterals(QbfFormula &formula, const std::map<std::string, std::string> &args) {
  auto it{args.find("sortLiterals")};
  if (it == args.end()) {
    return;
  }
  std::shared_ptr<LiteralSorter> sorter{LiteralSorter::create(it->second, formula, args)};
  if (sorter) {
    formula.sortLiterals([sorter] (std::int32_t left, std::int32_t right) {
      return (*sorter)(left, right);
    });
  }
}

void sortClauses(QbfFormula &formula, const std::map<std::string, std::string> &args) {
  auto it{args.find("sortClauses")};
  if (it == args.end()) {
    return;
  }
  std::shared_ptr<ClauseSorter> sorter{ClauseSorter::create(it->second, formula, args)};
  if (sorter) {
    formula.sortClauses([sorter] (std::vector<std::int32_t> left, std::vector<std::int32_t> right) {
      return (*sorter)(left, right);
    });
  }
}

void sortQuantifiers(QbfFormula &formula, const std::map<std::string, std::string> &args) {
  auto it{args.find("sortQuantifiers")};
  if (it == args.end()) {
    return;
  }
  std::shared_ptr<QuantifierSorter> sorter{QuantifierSorter::create(it->second, formula, args)};
  if (sorter) {
    formula.sortQuantifiers([sorter] (std::int32_t left, std::int32_t right) {
      return (*sorter)(left, right);
    });
  }
}
