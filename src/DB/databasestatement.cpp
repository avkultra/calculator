#include <memory>
#include <libpq-fe.h>
#include <stdexcept>
#include "DB/databaseresult.h"
#include "DB/databasestatement.h"
#include "DB/databaseconnection.h"

namespace app
{
    CDatabaseStatement::CDatabaseStatement(CDatabaseConnection& conn, const std::string& name, const std::string& sql)
        : m_conn(conn), m_name(name)
    {
        PGresult* res = PQprepare(static_cast<PGconn*>(conn.m_conn), name.c_str(), sql.c_str(), 0, nullptr);
        if (PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            std::string error = PQerrorMessage(static_cast<PGconn*>(conn.m_conn));
            PQclear(res);
            throw std::runtime_error("Failed to prepare statement: " + error);
        }
        PQclear(res);
    }

    CDatabaseStatement::~CDatabaseStatement()
    {
    }

    CDatabaseResult CDatabaseStatement::execute(const std::vector<std::string>& params)
    {
        auto paramValues = std::make_unique<const char*[]>(params.size());
        for (size_t i = 0; i < params.size(); ++i)
        {
            paramValues[i] = params[i].c_str();
        }

        PGresult* res = PQexecPrepared(static_cast<PGconn*>(m_conn.m_conn),
                                       m_name.c_str(),
                                       static_cast<int>(params.size()),
                                       paramValues.get(),
                                       nullptr, nullptr, 0);

        if (PQresultStatus(res) == PGRES_FATAL_ERROR)
        {
            std::string error = PQerrorMessage(static_cast<PGconn*>(m_conn.m_conn));
            PQclear(res);
            throw std::runtime_error("Statement execution failed: " + error);
        }

        return CDatabaseResult(res);
    }
}
