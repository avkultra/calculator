#include "app.h"
#include "DB/databasemanager.h"
#include "cache.h"
#include "logger.h"
#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        // Инициализация логгера
        auto& logger = app::CLogger::getInstance();
        logger.info("Application starting...");

        // Инициализация базы данных
        auto& db = app::CDatabaseManager::getInstance();
        if (!db.initialize("dbname=calculator_db user=calculator password=0 host=localhost"))
        {
            logger.warn("Database not available, continuing without persistence");
        }

        // Прогрев кеша из базы данных
        if (db.isInitialized())
        {
            app::CCalculationCache::getInstance().warmup();
            logger.info("Cache warmed up from database");
        }

        // Запуск основного приложения
        app::CApplication app;
        int result = app.run(argc, argv);

        // Вывод статистики кеша
        logger.info("Cache size at exit: " + std::to_string(app::CCalculationCache::getInstance().size()));

        return result;
    }
    catch (const std::exception& e)
    {
        app::CLogger::getInstance().error("Fatal error: " + std::string(e.what()));
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}
