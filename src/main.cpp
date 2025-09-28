#include <qbfsort/ClauseSorter.hpp>
#include <qbfsort/LiteralSorter.hpp>
#include <qbfsort/Formula.hpp>

#include <boost/program_options/option.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

int main(int argc, char **argv) {
  boost::program_options::options_description desc("Allowed options");
  desc.add_options()("help,h", "produce help message")(
      "input,i", boost::program_options::value<std::string>(),
      "file name of input formula")(
      "output,o", boost::program_options::value<std::string>(),
      "file name of output formula")(
      "stable", "preserve order of equal elements during sorting")(
      "literals,l", boost::program_options::value<std::vector<std::string>>(),
      "sorting metric for literals")(
      "clauses,c", boost::program_options::value<std::vector<std::string>>(),
      "sorting metric for clauses")(
      "quantifiers,q",
      boost::program_options::value<std::vector<std::string>>(),
      "sorting metric for quantifiers")(
      "statistics,s", boost::program_options::value<std::string>(),
      "print statistics to file");
  boost::program_options::variables_map vm;
  boost::program_options::store(
      boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);
  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return 0;
  }
  auto cinbuf{std::cin.rdbuf()};
  std::ifstream input;
  if (vm.count("input")) {
    input.open(vm["input"].as<std::string>());
    if (input.is_open()) {
      std::cin.rdbuf(input.rdbuf());
    }
  }
  qbfsort::Formula formula{qbfsort::Formula::fromStream(std::cin)};
  std::cin.rdbuf(cinbuf);
  if (vm.count("statistics")) {
    std::ofstream to{vm["statistics"].as<std::string>()};
    formula.printStatistics(to);
  }
  if (vm.count("literals")) {
    qbfsort::LiteralSorter ls{formula,
                              vm["literals"].as<std::vector<std::string>>()};
    if (vm.count("stable")) {
      formula.sortLiterals(ls);
    } else {
      formula.stableSortLiterals(ls);
    }
  }
  if (vm.count("clauses")) {
    qbfsort::ClauseSorter cs{formula,
                             vm["clauses"].as<std::vector<std::string>>()};
    if (vm.count("stable")) {
      formula.sortClauses(cs);
    } else {
      formula.stableSortClauses(cs);
    }
  }
  if (vm.count("quantifiers")) {
    qbfsort::LiteralSorter ls{
        formula, vm["quantifiers"].as<std::vector<std::string>>()};
    if (vm.count("stable")) {
      formula.sortQuantifiers(ls);
    } else {
      formula.stableSortQuantifiers(ls);
    }
  }
  auto coutbuf{std::cout.rdbuf()};
  std::ofstream output;
  if (vm.count("output")) {
    output.open(vm["output"].as<std::string>());
    if (output.is_open()) {
      std::cout.rdbuf(output.rdbuf());
    }
  }
  qbfsort::Formula::toStream(formula, std::cout);
  std::cout.rdbuf(coutbuf);
  return 0;
}
