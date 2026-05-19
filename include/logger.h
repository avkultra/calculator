#pragma once

#include <memory>
#include <string>

namespace app
{
    class CLogger
    {
      public:
        static CLogger& getInstance();

        void info(const std::string& message);
        void warn(const std::string& message);
        void error(const std::string& message);
        void debug(const std::string& message);

        //ctor
        CLogger(const CLogger&) = delete;
        CLogger& operator=(const CLogger&) = delete;
        CLogger(CLogger&&) = delete;
        CLogger& operator=(CLogger&&) = delete;

      private:
        CLogger();
        ~CLogger() = default;

        class Impl;
        std::unique_ptr<Impl> m_upImpl;
    };
}

