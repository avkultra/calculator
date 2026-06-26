#include "cache.h"
#include "DB/databasemanager.h"
#include <algorithm>
#include "logger.h"

namespace app
{
    CCalculationCache& CCalculationCache::getInstance()
    {
        static CCalculationCache instance;
        return instance;
    }

    bool CCalculationCache::isCommutative(char operation)
    {
        return operation == '+' || operation == '*';
    }

    std::string CCalculationCache::makeKey(int operand1, int operand2, char operation)
    {
        if (isCommutative(operation))
        {
            int a = std::min(operand1, operand2);
            int b = std::max(operand1, operand2);
            return std::to_string(a) + operation + std::to_string(b);
        }
        else
        {
            return std::to_string(operand1) + operation + std::to_string(operand2);
        }
    }

    std::string CCalculationCache::makeKey(int operand, char operation)
    {
        return std::to_string(operand) + operation;
    }

    void CCalculationCache::put(const std::string& key, int result, int status)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_cache[key] = {result, status};
        CLogger::getInstance().debug("Cache put: " + key + " = " + std::to_string(result) + " (status: " + std::to_string(status) + ")");
    }

    std::optional<std::pair<int, int>> CCalculationCache::get(const std::string& key)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_cache.find(key);
        if (it != m_cache.end())
        {
            CLogger::getInstance().debug("Cache hit: " + key);
            return std::make_pair(it->second.result, it->second.status);
        }
        CLogger::getInstance().debug("Cache miss: " + key);
        return std::nullopt;
    }

    bool CCalculationCache::contains(const std::string& key)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_cache.find(key) != m_cache.end();
    }

    void CCalculationCache::clear()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_cache.clear();
        CLogger::getInstance().info("Cache cleared");
    }

    size_t CCalculationCache::size() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_cache.size();
    }

    void CCalculationCache::warmup()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        CLogger::getInstance().info("Warming up cache from database...");

        auto& db = CDatabaseManager::getInstance();
        if (!db.isInitialized())
        {
            CLogger::getInstance().warn("Database not initialized, skipping cache warmup");
            return;
        }

        auto records = db.getAllOperations();
        for (const auto& rec : records)
        {
            if (rec.status == 0)
            {
                std::string key;
                if (rec.operation == '!')
                {
                    key = makeKey(rec.operand1, '!');
                }
                else
                {
                    key = makeKey(rec.operand1, rec.operand2, rec.operation);
                }
                m_cache[key] = {rec.result, rec.status};
            }
        }

        CLogger::getInstance().info("Cache warmup complete. Loaded " + std::to_string(m_cache.size()) + " entries");
    }
}
