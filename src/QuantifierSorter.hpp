#ifndef QUANTIFIERSORTER
#define QUANTIFIERSORTER

#include "QbfFormula.hpp"

#include <functional>
#include <map>
#include <memory>
#include <string_view>

class QuantifierSorter
{
public:
  static std::shared_ptr<QuantifierSorter> create(const std::string &className, QbfFormula &formula, const std::map<std::string, std::string> &args);
  QuantifierSorter(const QbfFormula &formula);
  bool operator()(std::int32_t left, std::int32_t right) const;
protected:
  const QbfFormula &formula;
private:
  using factoryMethod = std::function<std::shared_ptr<QuantifierSorter>(QbfFormula&, const std::map<std::string, std::string>&)>;
  static std::map<std::string, factoryMethod> factoryMap;
  virtual bool sort(std::int32_t left, std::int32_t right) const = 0;
};

class BasicQuantifierSorter : public QuantifierSorter
{
public:
  static constexpr std::string_view CLASS_NAME{"basic"};
  BasicQuantifierSorter(const QbfFormula &formula);
private:
  bool sort(std::int32_t left, std::int32_t right) const override;
};

class CountedBinariesQuantifierSorter : public QuantifierSorter
{
public:
  static constexpr std::string_view CLASS_NAME{"countedBinaries"};
  CountedBinariesQuantifierSorter(const QbfFormula &formula);
private:
  bool sort(std::int32_t left, std::int32_t right) const override;
  std::size_t getNewBinariesCount(std::int32_t literal) const;
};

class WeightedBinariesQuantifierSorter : public QuantifierSorter
{
public:
  static constexpr std::string_view CLASS_NAME{"weightedBinaries"};
  WeightedBinariesQuantifierSorter(const QbfFormula &formula);
private:
  bool sort(std::int32_t left, std::int32_t right) const override;
};

#endif
