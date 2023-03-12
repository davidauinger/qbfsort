#ifndef CLAUSESORTER
#define CLAUSESORTER

#include "QbfFormula.hpp"

#include <functional>
#include <map>
#include <memory>
#include <string_view>

class ClauseSorter
{
public:
  static std::shared_ptr<ClauseSorter> create(const std::string &className, QbfFormula &formula, const std::map<std::string, std::string> &args);
  ClauseSorter(const QbfFormula &formula);
  virtual ~ClauseSorter() = default;
  bool operator()(const std::vector<std::int32_t> &left, const std::vector<std::int32_t> &right) const;
protected:
  const QbfFormula &formula;
private:
  using factoryMethod = std::function<std::shared_ptr<ClauseSorter>(QbfFormula&, const std::map<std::string, std::string>&)>;
  static std::map<std::string, factoryMethod> factoryMap;
  virtual bool sort(const std::vector<std::int32_t> &left, const std::vector<std::int32_t> &right) const = 0;
};

class BasicClauseSorter : public ClauseSorter
{
public:
  static constexpr std::string_view CLASS_NAME{"basic"};
  BasicClauseSorter(const QbfFormula &formula);
private:
  bool sort(const std::vector<std::int32_t> &left, const std::vector<std::int32_t> &right) const override;
};

class FrequencyClauseSorter : public ClauseSorter
{
public:
  static constexpr std::string_view CLASS_NAME{"frequency"};
  FrequencyClauseSorter(const QbfFormula &formula);
private:
  bool sort(const std::vector<std::int32_t> &left, const std::vector<std::int32_t> &right) const override;
};

class CountedBinariesClauseSorter : public ClauseSorter
{
public:
  static constexpr std::string_view CLASS_NAME{"countedBinaries"};
  CountedBinariesClauseSorter(const QbfFormula &formula);
private:
  bool sort(const std::vector<std::int32_t> &left, const std::vector<std::int32_t> &right) const override;
  std::size_t getNewBinariesCount(const std::vector<std::int32_t> &literals) const;
};

class WeightedBinariesClauseSorter : public ClauseSorter
{
public:
  static constexpr std::string_view CLASS_NAME{"weightedBinaries"};
  WeightedBinariesClauseSorter(const QbfFormula &formula);
private:
  bool sort(const std::vector<std::int32_t> &left, const std::vector<std::int32_t> &right) const override;
  float getWeights(const std::vector<std::int32_t> &literals) const;
};

#endif
