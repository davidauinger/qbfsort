#ifndef LITERALSORTER
#define LITERALSORTER

#include "QbfFormula.hpp"

class LiteralSorter
{
public:
  LiteralSorter(const QbfFormula &formula);
  bool operator()(std::int32_t left, std::int32_t right);
protected:
  const QbfFormula &formula;
private:
  virtual bool sort(std::int32_t left, std::int32_t right) = 0;
};

class BasicLiteralSorter : public LiteralSorter
{
public:
  BasicLiteralSorter(const QbfFormula &formula);
private:
  bool sort(std::int32_t left, std::int32_t right) override;
};

class CountedBinariesLiteralSorter : public LiteralSorter
{
public:
  CountedBinariesLiteralSorter(const QbfFormula &formula);
private:
  bool sort(std::int32_t left, std::int32_t right) override;
  std::size_t getNewBinariesCount(std::int32_t literal);
};

#endif
