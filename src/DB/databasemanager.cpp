#include "DB/databasemanager.h"
#include "DB/databaseconnection.h"
#include <stdexcept>
#include "logger.h"
#include <libpq-fe.h>
#include <sstream>


namespace app
{
    CDatabaseManager& CDatabaseManager::getInstance()
    {
        static CDatabaseManager instance;
        return instance;
    }

    bool CDatabaseManager::initialize(const std::string& connString)
    {
        try
        {
            m_conn = std::make_unique<CDatabaseConnection>(connString);
            if (!createTables())
            {
                CLogger::getInstance().error("Failed to create tables");
                return false;
            }
            m_initialized = true;
            CLogger::getInstance().info("Database initialized successfully");
            return true;
        }
        catch (const std::exception& e)
        {
            CLogger::getInstance().error("Database initialization failed: " + std::string(e.what()));
            return false;
        }
    }

    bool CDatabaseManager::createTables()
    {
        const char* sql =
            "CREATE TABLE IF NOT EXISTS operations ("
            "    id SERIAL PRIMARY KEY,"
            "    operand1 INTEGER NOT NULL,"
            "    operand2 INTEGER,"
            "    operation CHAR(1) NOT NULL,"
            "    result INTEGER,"
            "    status INTEGER NOT NULL,"
            "    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
            ");"
            "CREATE INDEX IF NOT EXISTS idx_operation_key ON operations (operand1, operation, operand2);";

        PGresult* res = PQexec(static_cast<PGconn*>(m_conn->m_conn), sql);
        bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);
        PQclear(res);
        return success;
    }

    bool CDatabaseManager::insertOperation(int operand1, int operand2, char operation, int result, int status)
    {
        if (!isInitialized())
            return false;

        std::string sql = "INSERT INTO operations (operand1, operand2, operation, result, status) VALUES (" +
                          std::to_string(operand1) + ", " +
                          (operation == '!' ? "NULL" : std::to_string(operand2)) + ", '" +
                          std::string(1, operation) + "', " +
                          std::to_string(result) + ", " +
                          std::to_string(status) + ")";

        PGresult* res = PQexec(static_cast<PGconn*>(m_conn->m_conn), sql.c_str());
        bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);
        PQclear(res);

        if (success)
        {
            CLogger::getInstance().debug("Inserted operation to database: " +
                                        std::to_string(operand1) + " " + operation + " " + std::to_string(operand2) +
                                        " = " + std::to_string(result));
        }
        return success;
    }

    bool CDatabaseManager::insertFactorial(int operand, int result, int status)
    {
        return insertOperation(operand, 0, '!', result, status);
    }

    std::vector<CDatabaseManager::TOperationRecord> CDatabaseManager::getAllOperations()
    {
        std::vector<TOperationRecord> records;
        if (!isInitialized()) return records;

        PGresult* res = PQexec(static_cast<PGconn*>(m_conn->m_conn),
                               "SELECT id, operand1, operand2, operation, result, status, created_at FROM operations ORDER BY id");

        if (PQresultStatus(res) == PGRES_TUPLES_OK)
        {
            int rows = PQntuples(res);
            for (int i = 0; i < rows; ++i)
            {
                TOperationRecord rec;
                rec.id = std::stoi(PQgetvalue(res, i, 0));
                rec.operand1 = std::stoi(PQgetvalue(res, i, 1));
                if (PQgetisnull(res, i, 2))
                {
                    rec.operand2 = 0;
                }
                else
                {
                    rec.operand2 = std::stoi(PQgetvalue(res, i, 2));
                }
                rec.operation = PQgetvalue(res, i, 3)[0];
                rec.result = PQgetisnull(res, i, 4) ? 0 : std::stoi(PQgetvalue(res, i, 4));
                rec.status = std::stoi(PQgetvalue(res, i, 5));
                rec.createdAt = PQgetvalue(res, i, 6);
                records.push_back(rec);
            }
        }

        PQclear(res);
        CLogger::getInstance().debug("Loaded records from database " + std::to_string(records.size()));
        return records;
    }

    bool CDatabaseManager::isInitialized() const
    {
        return m_initialized && m_conn && m_conn->isConnected();
    }
}
