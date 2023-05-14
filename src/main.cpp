#include <qbfsort/ClauseSorter.hpp>
#include <qbfsort/LiteralSorter.hpp>
#include <qbfsort/QbfFormula.hpp>
#include <qbfsort/QuantifierSorter.hpp>

#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

std::map<std::string, std::string> parseArgs(int argc, char **argv);
void printStatistics(qbfsort::QbfFormula &formula,
                     const std::map<std::string, std::string> &args);
void sortLiterals(qbfsort::QbfFormula &formula,
                  const std::map<std::string, std::string> &args);
void sortClauses(qbfsort::QbfFormula &formula,
                 const std::map<std::string, std::string> &args);
void sortQuantifiers(qbfsort::QbfFormula &formula,
                     const std::map<std::string, std::string> &args);

int main(int argc, char **argv) {
  qbfsort::QbfFormula f{qbfsort::QbfFormula::fromStream(std::cin)};
  std::map<std::string, std::string> args{parseArgs(argc, argv)};
  printStatistics(f, args);
  sortLiterals(f, args);
  sortClauses(f, args);
  sortQuantifiers(f, args);
  qbfsort::QbfFormula::toStream(f, std::cout);
  return 0;
}

std::map<std::string, std::string> parseArgs(int argc, char **argv) {
  std::map<std::string, std::string> args;
  for (int i{1}; i < argc; ++i) {
    char *str{std::strpbrk(argv[i], "=")};
    const std::string &key{str ? std::string(argv[i], str - argv[i]) : argv[i]};
    const std::string &value{str ? str + 1 : ""};
    args.emplace(key, value);
  }
  return args;
}

void printStatistics(qbfsort::QbfFormula &formula,
                     const std::map<std::string, std::string> &args) {
  auto it{args.find("printStatistics")};
  if (it == args.end()) {
    return;
  }
  std::ofstream to{it->second};
  formula.printStatistics(to);
}

void sortLiterals(qbfsort::QbfFormula &formula,
                  const std::map<std::string, std::string> &args) {
  auto it{args.find("sortLiterals")};
  if (it == args.end()) {
    return;
  }
  std::shared_ptr<qbfsort::LiteralSorter> sorter{
      qbfsort::LiteralSorter::create(it->second, formula, args)};
  if (sorter) {
    formula.sortLiterals([sorter](std::int32_t left, std::int32_t right) {
      return (*sorter)(left, right);
    });
  }
}

void sortClauses(qbfsort::QbfFormula &formula,
                 const std::map<std::string, std::string> &args) {
  auto it{args.find("sortClauses")};
  if (it == args.end()) {
    return;
  }
  std::shared_ptr<qbfsort::ClauseSorter> sorter{
      qbfsort::ClauseSorter::create(it->second, formula, args)};
  if (sorter) {
    formula.sortClauses([sorter](std::vector<std::int32_t> left,
                                 std::vector<std::int32_t> right) {
      return (*sorter)(left, right);
    });
  }
}

void sortQuantifiers(qbfsort::QbfFormula &formula,
                     const std::map<std::string, std::string> &args) {
  auto it{args.find("sortQuantifiers")};
  if (it == args.end()) {
    return;
  }
  std::shared_ptr<qbfsort::QuantifierSorter> sorter{
      qbfsort::QuantifierSorter::create(it->second, formula, args)};
  if (sorter) {
    formula.sortQuantifiers([sorter](std::int32_t left, std::int32_t right) {
      return (*sorter)(left, right);
    });
  }
}
