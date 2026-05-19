#include "logger.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace app
{

    class CLogger::Impl
    {
      public:
        Impl()
        {
            logger = spdlog::stdout_color_mt("calculator");
            logger->set_pattern("[%H:%M:%S] [%^%l%$] %v");
            logger->set_level(spdlog::level::info);
        }

        std::shared_ptr<spdlog::logger> logger;
    };

    CLogger& CLogger::getInstance()
    {
        static CLogger instance;
        return instance;
    }

    CLogger::CLogger() : m_upImpl(std::make_unique<Impl>())
    {

    }

    void CLogger::info(const std::string& message)
    {
        m_upImpl->logger->info(message);
    }

    void CLogger::warn(const std::string& message)
    {
        m_upImpl->logger->warn(message);
    }

    void CLogger::error(const std::string& message)
    {
        m_upImpl->logger->error(message);
    }

    void CLogger::debug(const std::string& message)
    {
        m_upImpl->logger->debug(message);
    }

}
