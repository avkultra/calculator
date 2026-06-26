#pragma once

#include <vector>
#include <string>
#include <mutex>
#include <memory>

namespace app
{
    class CDatabaseConnection;

    /**
     * Менеджер БД
     */
    class CDatabaseManager
    {
      public:
        static CDatabaseManager& getInstance();

        bool initialize(const std::string& connString = "dbname=calculator_db user=postgres password=postgres host=localhost");
        bool isInitialized() const;

        bool insertOperation(int operand1, int operand2, char operation, int result, int status);
        bool insertFactorial(int operand, int result, int status);

        struct TOperationRecord
        {
            int id;
            int operand1;
            int operand2;
            char operation;
            int result;
            int status;
            std::string createdAt;
        };

        std::vector<TOperationRecord> getAllOperations();
        bool operationExists(int operand1, int operand2, char operation);

      private:
        CDatabaseManager() = default;
        ~CDatabaseManager() = default;

        bool createTables();

        std::unique_ptr<CDatabaseConnection> m_conn;
        bool m_initialized = false;
        std::mutex m_db_mutex;
    };

}
