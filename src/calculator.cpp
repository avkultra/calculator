#include "calculator.h"
#include "cache.h"
#include "DB/databasemanager.h"
#include "logger.h"
#include <libmath.h>
#include <stdexcept>
#include <climits>

namespace app
{
    int CCalculator::add(int a, int b) const
    {
        CLogger::getInstance().debug("Adding " + std::to_string(a) + " + " + std::to_string(b));
        int result = libmath::addition(a, b);
        CLogger::getInstance().debug("Result: " + std::to_string(result));
        return result;
    }

    int CCalculator::subtract(int a, int b) const
    {
        CLogger::getInstance().debug("Subtracting " + std::to_string(a) + " - " + std::to_string(b));
        int result = libmath::subtraction(a, b);
        CLogger::getInstance().debug("Result: " + std::to_string(result));
        return result;
    }

    int CCalculator::multiply(int a, int b) const
    {
        CLogger::getInstance().debug("Multiplying " + std::to_string(a) + " * " + std::to_string(b));
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

        CLogger::getInstance().debug("Dividing " + std::to_string(a) + " / " + std::to_string(b));
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

        // Защита от бага в libmath при base == 0
        if (base == 0 && exponent > 0)
        {
            return 0;
        }

        if (exponent == 0)
        {
            return 1;
        }

        CLogger::getInstance().debug("Calculating power: " + std::to_string(base) + "^" + std::to_string(exponent));
        int result = libmath::power(base, exponent);

        if (result == -1)
        {
            throw std::runtime_error("Power calculation failed");
        }

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

        CLogger::getInstance().debug("Calculating factorial: " + std::to_string(n) + "!");
        int result = libmath::factorial(n);
        CLogger::getInstance().debug("Result: " + std::to_string(result));
        return result;
    }


    int CCalculator::addCached(int a, int b)
    {
        std::string key = CCalculationCache::makeKey(a, b, '+');

        // Проверяем кеш
        auto& cache = CCalculationCache::getInstance();
        auto cached = cache.get(key);

        if (cached.has_value())
        {
            auto [result, status] = cached.value();
            if (status == 0)
            {
                CLogger::getInstance().debug("Cache hit: " + key);
                return result;
            }
            else
            {
                throw std::runtime_error("Cached error for: " + key);
            }
        }

        // Вычисляем
        CLogger::getInstance().debug("Cache miss: calculating " + key);
        int result = add(a, b);
        int status = 0;

        // Сохраняем в кеш и БД
        cache.put(key, result, status);
        storeInDatabase(a, b, '+', result, status);

        return result;
    }

    int CCalculator::subtractCached(int a, int b)
    {
        std::string key = CCalculationCache::makeKey(a, b, '-');

        auto& cache = CCalculationCache::getInstance();
        auto cached = cache.get(key);

        if (cached.has_value())
        {
            auto [result, status] = cached.value();
            if (status == 0)
            {
                CLogger::getInstance().debug("Cache hit: " + key);
                return result;
            }
            else
            {
                throw std::runtime_error("Cached error for: " + key);
            }
        }

        CLogger::getInstance().debug("Cache miss: calculating " + key);
        int result = subtract(a, b);
        int status = 0;

        cache.put(key, result, status);
        storeInDatabase(a, b, '-', result, status);

        return result;
    }

    int CCalculator::multiplyCached(int a, int b)
    {
        std::string key = CCalculationCache::makeKey(a, b, '*');

        auto& cache = CCalculationCache::getInstance();
        auto cached = cache.get(key);

        if (cached.has_value())
        {
            auto [result, status] = cached.value();
            if (status == 0)
            {
                CLogger::getInstance().debug("Cache hit: " + key);
                return result;
            }
            else
            {
                throw std::runtime_error("Cached error for: " + key);
            }
        }

        CLogger::getInstance().debug("Cache miss: calculating " + key);
        int result = multiply(a, b);
        int status = 0;

        cache.put(key, result, status);
        storeInDatabase(a, b, '*', result, status);

        return result;
    }

    int CCalculator::divideCached(int a, int b)
    {
        std::string key = CCalculationCache::makeKey(a, b, '/');

        auto& cache = CCalculationCache::getInstance();
        auto cached = cache.get(key);

        if (cached.has_value())
        {
            auto [result, status] = cached.value();
            if (status == 0)
            {
                CLogger::getInstance().debug("Cache hit: " + key);
                return result;
            }
            else
            {
                throw std::runtime_error("Cached error for: " + key);
            }
        }

        CLogger::getInstance().debug("Cache miss: calculating " + key);
        int result = divide(a, b);
        int status = 0;

        cache.put(key, result, status);
        storeInDatabase(a, b, '/', result, status);

        return result;
    }

    int CCalculator::powerCached(int a, int b)
    {
        std::string key = CCalculationCache::makeKey(a, b, '^');

        auto& cache = CCalculationCache::getInstance();
        auto cached = cache.get(key);

        if (cached.has_value())
        {
            auto [result, status] = cached.value();
            if (status == 0)
            {
                CLogger::getInstance().debug("Cache hit: " + key);
                return result;
            }
            else
            {
                throw std::runtime_error("Cached error for: " + key);
            }
        }

        CLogger::getInstance().debug("Cache miss: calculating " + key);
        int result = power(a, b);
        int status = 0;

        cache.put(key, result, status);
        storeInDatabase(a, b, '^', result, status);

        return result;
    }

    int CCalculator::factorialCached(int n)
    {
        std::string key = CCalculationCache::makeKey(n, '!');

        auto& cache = CCalculationCache::getInstance();
        auto cached = cache.get(key);

        if (cached.has_value())
        {
            auto [result, status] = cached.value();
            if (status == 0)
            {
                CLogger::getInstance().debug("Cache hit: " + key);
                return result;
            }
            else
            {
                throw std::runtime_error("Cached error for: " + key);
            }
        }

        CLogger::getInstance().debug("Cache miss: calculating " + key);
        int result = factorial(n);
        int status = 0;

        cache.put(key, result, status);
        storeInDatabase(n, '!', result, status);

        return result;
    }


    size_t CCalculator::getCacheSize() const
    {
        return CCalculationCache::getInstance().size();
    }

    void CCalculator::clearCache()
    {
        CCalculationCache::getInstance().clear();
    }

    void CCalculator::warmupCache()
    {
        CCalculationCache::getInstance().warmup();
    }

    void CCalculator::storeInDatabase(int operand1, int operand2, char operation, int result, int status)
    {
        auto& db = CDatabaseManager::getInstance();
        if (db.isInitialized())
        {
            db.insertOperation(operand1, operand2, operation, result, status);
        }
    }

    void CCalculator::storeInDatabase(int operand, char operation, int result, int status)
    {
        auto& db = CDatabaseManager::getInstance();
        if (db.isInitialized())
        {
            db.insertFactorial(operand, result, status);
        }
    }
}
