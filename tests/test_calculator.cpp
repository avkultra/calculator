#include <gtest/gtest.h>
#include "calculator.h"
#include <stdexcept>

class CalculatorTest : public ::testing::Test
{
  protected:
    app::CCalculator calc;
};

TEST_F(CalculatorTest, AddPositiveNumbers)
{
    EXPECT_EQ(calc.add(5, 3), 8);
    EXPECT_EQ(calc.add(0, 5), 5);
    EXPECT_EQ(calc.add(100, 200), 300);
}

TEST_F(CalculatorTest, AddNegativeNumbers)
{
    EXPECT_EQ(calc.add(-5, -3), -8);
    EXPECT_EQ(calc.add(-10, 5), -5);
    EXPECT_EQ(calc.add(10, -5), 5);
}

TEST_F(CalculatorTest, SubtractNumbers)
{
    EXPECT_EQ(calc.subtract(10, 3), 7);
    EXPECT_EQ(calc.subtract(5, 10), -5);
    EXPECT_EQ(calc.subtract(0, 5), -5);
}

TEST_F(CalculatorTest, MultiplyNumbers)
{
    EXPECT_EQ(calc.multiply(5, 3), 15);
    EXPECT_EQ(calc.multiply(0, 100), 0);
    EXPECT_EQ(calc.multiply(-5, 3), -15);
    EXPECT_EQ(calc.multiply(-5, -3), 15);
}

TEST_F(CalculatorTest, DivideNumbers)
{
    EXPECT_EQ(calc.divide(10, 2), 5);
    EXPECT_EQ(calc.divide(7, 2), 3);
    EXPECT_EQ(calc.divide(0, 5), 0);
}

TEST_F(CalculatorTest, DivideByZeroThrows)
{
    EXPECT_THROW(calc.divide(10, 0), std::runtime_error);
}

TEST_F(CalculatorTest, PowerNumbers)
{
    EXPECT_EQ(calc.power(2, 3), 8);
    EXPECT_EQ(calc.power(5, 0), 1);
    EXPECT_EQ(calc.power(1, 100), 1);
    EXPECT_EQ(calc.power(0, 5), 0);
}

TEST_F(CalculatorTest, NegativeExponentThrows)
{
    EXPECT_THROW(calc.power(2, -1), std::invalid_argument);
}

TEST_F(CalculatorTest, Factorial)
{
    EXPECT_EQ(calc.factorial(0), 1);
    EXPECT_EQ(calc.factorial(1), 1);
    EXPECT_EQ(calc.factorial(5), 120);
    EXPECT_EQ(calc.factorial(7), 5040);
}

TEST_F(CalculatorTest, NegativeFactorialThrows)
{
    EXPECT_THROW(calc.factorial(-1), std::invalid_argument);
}
