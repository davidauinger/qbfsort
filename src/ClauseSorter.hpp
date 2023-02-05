#ifndef CLAUSESORTER
#define CLAUSESORTER

#include "QbfFormula.hpp"

class ClauseSorter
{
public:
  ClauseSorter(const QbfFormula &formula);
  bool operator()(std::vector<std::int32_t> left, std::vector<std::int32_t> right);
protected:
  const QbfFormula &formula;
private:
  virtual bool sort(std::vector<std::int32_t> left, std::vector<std::int32_t> right) = 0;
};

class BasicClauseSorter : public ClauseSorter
{
public:
  BasicClauseSorter(const QbfFormula &formula);
private:
  bool sort(std::vector<std::int32_t> left, std::vector<std::int32_t> right) override;
};

class CountedBinariesClauseSorter : public ClauseSorter
{
public:
  CountedBinariesClauseSorter(const QbfFormula &formula);
private:
  bool sort(std::vector<std::int32_t> left, std::vector<std::int32_t> right) override;
  std::size_t getNewBinariesCount(std::vector<std::int32_t> literals);
};

#endif
