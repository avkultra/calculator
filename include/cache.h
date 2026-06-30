#pragma once

#include <unordered_map>
#include <string>
#include <optional>
#include <mutex>

namespace app
{
    class CCalculationCache
    {
      public:
        static CCalculationCache& getInstance();

        static std::string makeKey(int operand1, int operand2, char operation);

        static std::string makeKey(int operand, char operation);

        void put(const std::string& key, int result, int status);

        std::optional<std::pair<int, int>> get(const std::string& key);

        bool contains(const std::string& key);

        void clear();

        size_t size() const;

        void warmup();

      private:
        CCalculationCache() = default;
        ~CCalculationCache() = default;

        struct CacheEntry
        {
            int result;
            int status;
        };

        std::unordered_map<std::string, CacheEntry> m_cache;
        static bool isCommutative(char operation);
        mutable std::mutex m_mutex;
    };

}
