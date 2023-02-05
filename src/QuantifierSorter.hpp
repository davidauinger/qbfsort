#ifndef QUANTIFIERSORTER
#define QUANTIFIERSORTER

#include "QbfFormula.hpp"

class QuantifierSorter
{
public:
  QuantifierSorter(const QbfFormula &formula);
  bool operator()(std::int32_t left, std::int32_t right);
protected:
  const QbfFormula &formula;
private:
  virtual bool sort(std::int32_t left, std::int32_t right) = 0;
};

class BasicQuantifierSorter : public QuantifierSorter
{
public:
  BasicQuantifierSorter(const QbfFormula &formula);
private:
  bool sort(std::int32_t left, std::int32_t right) override;
};

class CountedBinariesQuantifierSorter : public QuantifierSorter
{
public:
  CountedBinariesQuantifierSorter(const QbfFormula &formula);
private:
  bool sort(std::int32_t left, std::int32_t right) override;
  std::size_t getNewBinariesCount(std::int32_t literal);
};

#endif
