#include "server.h"
#include "DB/databasemanager.h"
#include "cache.h"
#include "logger.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <signal.h>
#include <chrono>

std::atomic<bool> g_running{true};

int main(int argc, char* argv[])
{
    try
    {
        auto& logger = app::CLogger::getInstance();
        logger.info("Service starting...");

        // Инициализация БД и прогреваем ке
        auto& db = app::CDatabaseManager::getInstance();
        if (!db.initialize("dbname=calculator_db user=calculator password=0 host=localhost"))
        {
            logger.warn("Database not available, continuing without persistence");
        }

        if (db.isInitialized())
        {
            app::CCalculationCache::getInstance().warmup();
            logger.info("Cache warmed up from database");
        }

        // Блокируем сигналы в главном потоке
        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGTERM);
        sigaddset(&set, SIGINT);
        sigaddset(&set, SIGHUP);  // для перезагрузки
        pthread_sigmask(SIG_BLOCK, &set, nullptr);

        // Запускаем сервер в отдельном потоке
        app::CServer server(8080);
        std::thread server_thread([&server]() {
            server.run(g_running);
        });

        // Главный поток ждёт сигналы
        int sig;
        sigwait(&set, &sig);
        logger.info("Received signal " + std::to_string(sig) + ", shutting down...");

        // Останавливаем сервер
        g_running = false;
        server.stop();

        // Ждём завершения серверного потока
        if (server_thread.joinable())
            server_thread.join();

        logger.info("Service stopped");
        return 0;
    }
    catch (const std::exception& e)
    {
        app::CLogger::getInstance().error("Fatal error: " + std::string(e.what()));
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}
