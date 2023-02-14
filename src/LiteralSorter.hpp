#ifndef LITERALSORTER
#define LITERALSORTER

#include "QbfFormula.hpp"

#include <functional>
#include <map>
#include <memory>
#include <string_view>

class LiteralSorter
{
public:
  static std::shared_ptr<LiteralSorter> create(const std::string &className, QbfFormula &formula, const std::map<std::string, std::string> &args);
  LiteralSorter(const QbfFormula &formula);
  bool operator()(std::int32_t left, std::int32_t right) const;
protected:
  const QbfFormula &formula;
private:
  using factoryMethod = std::function<std::shared_ptr<LiteralSorter>(QbfFormula&, const std::map<std::string, std::string>&)>;
  static std::map<std::string, factoryMethod> factoryMap;
  virtual bool sort(std::int32_t left, std::int32_t right) const = 0;
};

class BasicLiteralSorter : public LiteralSorter
{
public:
  static constexpr std::string_view CLASS_NAME{"basic"};
  BasicLiteralSorter(const QbfFormula &formula);
private:
  bool sort(std::int32_t left, std::int32_t right) const override;
};

class CountedBinariesLiteralSorter : public LiteralSorter
{
public:
  static constexpr std::string_view CLASS_NAME{"countedBinaries"};
  CountedBinariesLiteralSorter(const QbfFormula &formula);
private:
  bool sort(std::int32_t left, std::int32_t right) const override;
  std::size_t getNewBinariesCount(std::int32_t literal) const;
};

class WeightedBinariesLiteralSorter : public LiteralSorter
{
public:
  static constexpr std::string_view CLASS_NAME{"weightedBinaries"};
  WeightedBinariesLiteralSorter(const QbfFormula &formula);
private:
  bool sort(std::int32_t left, std::int32_t right) const override;
};

#endif
