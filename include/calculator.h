#pragma once

#include <cstddef>
#include <string>

namespace app
{
class CCalculator
{
  public:
    // Базовые операции
    int add(int a, int b) const;
    int subtract(int a, int b) const;
    int multiply(int a, int b) const;
    int divide(int a, int b) const;
    int power(int base, int exponent) const;
    int factorial(int n) const;

    // Кеширующие операции
    int addCached(int a, int b);
    int subtractCached(int a, int b);
    int multiplyCached(int a, int b);
    int divideCached(int a, int b);
    int powerCached(int a, int b);
    int factorialCached(int n);

    // Управление кешем
    size_t getCacheSize() const;
    void clearCache();
    void warmupCache();

  private:
    // Вспомогательные методы для работы с БД
    void storeInDatabase(int operand1, int operand2, char operation, int result, int status);
    void storeInDatabase(int operand, char operation, int result, int status);
};
}
