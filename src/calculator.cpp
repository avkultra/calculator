#include "calculator.h"
#include <stdexcept>
#include <string>
#include <libmath.h>
#include "logger.h"

namespace app
{

    int CCalculator::add(int a, int b) const
    {
        CLogger::getInstance().debug("Add " + std::to_string(a) + " + " + std::to_string(b));
        int result = libmath::addition(a, b);
        CLogger::getInstance().debug("Result: " + std::to_string(result));
        return result;
    }

    int CCalculator::subtract(int a, int b) const
    {
        CLogger::getInstance().debug("Subtract " + std::to_string(a) + " - " + std::to_string(b));
        int result = libmath::subtraction(a, b);
        CLogger::getInstance().debug("Result: " + std::to_string(result));
        return result;
    }

    int CCalculator::multiply(int a, int b) const
    {
        CLogger::getInstance().debug("Multiply " + std::to_string(a) + " * " + std::to_string(b));
        int result = libmath::multiplication(a, b);
        CLogger::getInstance().debug("Result: " + std::to_string(result));
        return result;
    }

    int CCalculator::divide(int a, int b) const
    {
        if (b == 0)
        {
            CLogger::getInstance().error("Division by zero");
            throw std::runtime_error("Division by zero");
        }

        CLogger::getInstance().debug("Divid " + std::to_string(a) + " / " + std::to_string(b));
        int result;
        int status = libmath::division(a, b, result);

        if (status != 0)
        {
            throw std::runtime_error("Division error");
        }

        CLogger::getInstance().debug("Result: " + std::to_string(result));
        return result;
    }

    int CCalculator::power(int base, int exponent) const
    {
        if (exponent < 0)
        {
            CLogger::getInstance().error("Negative exponent: " + std::to_string(exponent));
            throw std::invalid_argument("Negative exponent not supported");
        }

        CLogger::getInstance().debug("Calculating power " + std::to_string(base) + "^" + std::to_string(exponent));
        int result = libmath::power(base, exponent);
        CLogger::getInstance().debug("Result: " + std::to_string(result));
        return result;
    }

    int CCalculator::factorial(int n) const
    {
        if (n < 0)
        {
            CLogger::getInstance().error("Negative factorial: " + std::to_string(n));
            throw std::invalid_argument("Factorial of negative number is not defined");
        }

        CLogger::getInstance().debug("Calculating factorial " + std::to_string(n) + "!");
        int result = libmath::factorial(n);
        CLogger::getInstance().debug("Result: " + std::to_string(result));
        return result;
    }

}
