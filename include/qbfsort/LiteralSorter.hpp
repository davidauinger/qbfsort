#ifndef QBFSORT_LITERALSORTER_HPP
#define QBFSORT_LITERALSORTER_HPP

#include "Sorter.hpp"

namespace qbfsort {

class LiteralSorter : public Sorter<std::int32_t> {
public:
  LiteralSorter(const Formula &formula, const std::string &metric,
                bool isInverse = false);
  static const std::map<std::string_view, std::string_view> &getMetricsMap();

private:
  static const std::map<std::string_view, std::string_view> metricsMap;
  static const std::map<std::string_view, compareMethod> methodsMap;
};

} // namespace qbfsort

#endif
