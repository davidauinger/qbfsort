#include <qbfsort/ClauseSorter.hpp>
#include <qbfsort/Formula.hpp>
#include <qbfsort/LiteralSorter.hpp>
#include <qbfsort/QuantifierSorter.hpp>

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

void printMetricsMap(
    const std::map<std::string_view, std::string_view> &metricsMap);
std::string getLiteralMetric(const boost::program_options::variables_map &vm);
std::string getClauseMetric(const boost::program_options::variables_map &vm);
std::string
getQuantifierMetric(const boost::program_options::variables_map &vm);

int main(int argc, char **argv) {
  boost::program_options::options_description desc("Allowed options");
  desc.add_options()("help,h", "produce help message")(
      "input,i", boost::program_options::value<std::string>(),
      "file name of input formula")(
      "output,o", boost::program_options::value<std::string>(),
      "file name of output formula")
#ifdef WITH_JSONCPP
      ("statistics,j", boost::program_options::value<std::string>(),
       "print statistics to file")
#endif
          ("metric,m", boost::program_options::value<std::string>(),
           "sorting metric for literals, clauses and quantifiers")(
              "literals,l", boost::program_options::value<std::string>(),
              "sorting metric for literals")(
              "clauses,c", boost::program_options::value<std::string>(),
              "sorting metric for clauses")(
              "quantifiers,q", boost::program_options::value<std::string>(),
              "sorting metric for quantifiers")(
              "stable,s", "preserve order of equal elements during sorting")(
              "inverse,x", "sort in the inverse order")(
              "literalsinverse", "sort literals in the inverse order")(
              "clausesinverse", "sort clauses in the inverse order")(
              "quantifiersinverse", "sort quantifiers in the inverse order");
  boost::program_options::variables_map vm;
  boost::program_options::store(
      boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);
  if (vm.count("help")) {
    std::cout << desc << std::endl;
    std::cout << "\npossible literal metrics:\n" << std::endl;
    printMetricsMap(qbfsort::LiteralSorter::getMetricsMap());
    std::cout << "\npossible clause metrics:\n" << std::endl;
    printMetricsMap(qbfsort::ClauseSorter::getMetricsMap());
    std::cout << "\npossible quantifier metrics:\n" << std::endl;
    printMetricsMap(qbfsort::QuantifierSorter::getMetricsMap());
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
#ifdef WITH_JSONCPP
  if (vm.count("statistics")) {
    std::ofstream to{vm["statistics"].as<std::string>()};
    formula.printStatistics(to);
  }
#endif
  if (const auto metric{getLiteralMetric(vm)}; !metric.empty()) {
    qbfsort::LiteralSorter ls{formula, metric,
                              vm.count("inverse") != 0 ||
                                  vm.count("literalsinverse") != 0};
    if (vm.count("stable")) {
      formula.stableSortLiterals(ls);
    } else {
      formula.sortLiterals(ls);
    }
  }
  if (const auto metric{getClauseMetric(vm)}; !metric.empty()) {
    qbfsort::ClauseSorter cs{formula, metric,
                             vm.count("inverse") != 0 ||
                                 vm.count("clausesinverse") != 0};
    if (vm.count("stable")) {
      formula.stableSortClauses(cs);
    } else {
      formula.sortClauses(cs);
    }
  }
  if (const auto metric{getQuantifierMetric(vm)}; !metric.empty()) {
    qbfsort::QuantifierSorter qs{formula, metric,
                                 vm.count("inverse") != 0 ||
                                     vm.count("quantifiersinverse") != 0};
    if (vm.count("stable")) {
      formula.stableSortQuantifiers(qs);
    } else {
      formula.sortQuantifiers(qs);
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

void printMetricsMap(
    const std::map<std::string_view, std::string_view> &metricsMap) {
  for (const auto &[in, act] : metricsMap) {
    if (in == act) {
      std::cout << in << std::endl;
    } else {
      std::cout << in << " -> " << act << std::endl;
    }
  }
}

std::string getLiteralMetric(const boost::program_options::variables_map &vm) {
  if (vm.count("literals")) {
    return vm["literals"].as<std::string>();
  }
  if (vm.count("metric")) {
    return vm["metric"].as<std::string>();
  }
  return std::string();
}

std::string getClauseMetric(const boost::program_options::variables_map &vm) {
  if (vm.count("clauses")) {
    return vm["clauses"].as<std::string>();
  }
  if (vm.count("metric")) {
    return vm["metric"].as<std::string>();
  }
  return std::string();
}

std::string
getQuantifierMetric(const boost::program_options::variables_map &vm) {
  if (vm.count("quantifiers")) {
    return vm["quantifiers"].as<std::string>();
  }
  if (vm.count("metric")) {
    return vm["metric"].as<std::string>();
  }
  return std::string();
}
