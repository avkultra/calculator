#pragma once

namespace app
{
    class CCalculator
    {
      public:
        int add(int a, int b) const;
        int subtract(int a, int b) const;
        int multiply(int a, int b) const;
        int divide(int a, int b) const;
        int power(int base, int exponent) const;
        int factorial(int n) const;
    };
}
