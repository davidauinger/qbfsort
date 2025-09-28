#include <qbfsort/ClauseSorter.hpp>
#include <qbfsort/Formula.hpp>
#include <qbfsort/LiteralSorter.hpp>

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

std::vector<std::string>
getLiteralMetrics(const boost::program_options::variables_map &vm);
std::vector<std::string>
getClauseMetrics(const boost::program_options::variables_map &vm);
std::vector<std::string>
getQuantifierMetrics(const boost::program_options::variables_map &vm);

int main(int argc, char **argv) {
  boost::program_options::options_description desc("Allowed options");
  desc.add_options()("help,h", "produce help message")(
      "input,i", boost::program_options::value<std::string>(),
      "file name of input formula")(
      "output,o", boost::program_options::value<std::string>(),
      "file name of output formula")(
      "stable,s", "preserve order of equal elements during sorting")(
      "inverse,x", "sort in the inverse order")(
      "literals,l", boost::program_options::value<std::vector<std::string>>(),
      "sorting metric for literals")(
      "clauses,c", boost::program_options::value<std::vector<std::string>>(),
      "sorting metric for clauses")(
      "quantifiers,q",
      boost::program_options::value<std::vector<std::string>>(),
      "sorting metric for quantifiers")("basic,b",
                                        "same as -l basic -c basic -q basic")(
      "frequency,f", "same as -l frequencyVariable -c frequencyVariableMeans "
                     "-q frequencyVariable")(
      "countedBinaries,n",
      "same as -l countedBinariesVariable -c countedBinariesVariableMeans -q "
      "countedBinariesVariable")("weightedBinaries,w",
                                 "same as -l weightedBinaries -c "
                                 "weightedBinariesMeans -q weightedBinaries")(
      "statistics,j", boost::program_options::value<std::string>(),
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
  if (const auto metrics{getLiteralMetrics(vm)}; !metrics.empty()) {
    qbfsort::LiteralSorter ls{formula, metrics, vm.count("inverse") != 0};
    if (vm.count("stable")) {
      formula.stableSortLiterals(ls);
    } else {
      formula.sortLiterals(ls);
    }
  }
  if (const auto metrics{getClauseMetrics(vm)}; !metrics.empty()) {
    qbfsort::ClauseSorter cs{formula, metrics, vm.count("inverse") != 0};
    if (vm.count("stable")) {
      formula.stableSortClauses(cs);
    } else {
      formula.sortClauses(cs);
    }
  }
  if (const auto metrics{getQuantifierMetrics(vm)}; !metrics.empty()) {
    qbfsort::LiteralSorter ls{formula, metrics, vm.count("inverse") != 0};
    if (vm.count("stable")) {
      formula.stableSortQuantifiers(ls);
    } else {
      formula.sortQuantifiers(ls);
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

std::vector<std::string>
getLiteralMetrics(const boost::program_options::variables_map &vm) {
  if (vm.count("literals")) {
    return vm["literals"].as<std::vector<std::string>>();
  }
  if (vm.count("basic")) {
    return std::vector<std::string>({"basic"});
  }
  if (vm.count("frequency")) {
    return std::vector<std::string>({"frequencyVariable"});
  }
  if (vm.count("countedBinaries")) {
    return std::vector<std::string>({"countedBinariesVariable"});
  }
  if (vm.count("weightedBinaries")) {
    return std::vector<std::string>({"weightedBinaries"});
  }
  return std::vector<std::string>();
}

std::vector<std::string>
getClauseMetrics(const boost::program_options::variables_map &vm) {
  if (vm.count("clauses")) {
    return vm["clauses"].as<std::vector<std::string>>();
  }
  if (vm.count("basic")) {
    return std::vector<std::string>({"basic"});
  }
  if (vm.count("frequency")) {
    return std::vector<std::string>({"frequencyVariableMeans"});
  }
  if (vm.count("countedBinaries")) {
    return std::vector<std::string>({"countedBinariesVariableMeans"});
  }
  if (vm.count("weightedBinaries")) {
    return std::vector<std::string>({"weightedBinariesMeans"});
  }
  return std::vector<std::string>();
}

std::vector<std::string>
getQuantifierMetrics(const boost::program_options::variables_map &vm) {
  if (vm.count("quantifiers")) {
    return vm["quantifiers"].as<std::vector<std::string>>();
  }
  if (vm.count("basic")) {
    return std::vector<std::string>({"basic"});
  }
  if (vm.count("frequency")) {
    return std::vector<std::string>({"frequencyVariable"});
  }
  if (vm.count("countedBinaries")) {
    return std::vector<std::string>({"countedBinariesVariable"});
  }
  if (vm.count("weightedBinaries")) {
    return std::vector<std::string>({"weightedBinaries"});
  }
  return std::vector<std::string>();
}
