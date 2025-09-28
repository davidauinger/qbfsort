#ifndef QBFSORT_CLAUSESORTER_HPP
#define QBFSORT_CLAUSESORTER_HPP

#include "Sorter.hpp"

namespace qbfsort {

class ClauseSorter : public Sorter<std::vector<std::int32_t>> {
public:
  ClauseSorter(const Formula &formula, const std::string &metric,
               bool isInverse = false);
  static const std::map<std::string_view, std::string_view> &getMetricsMap();

private:
  static const std::map<std::string_view, std::string_view> metricsMap;
  static const std::map<std::string_view, compareMethod> methodsMap;
};

} // namespace qbfsort

#endif
