#include "DB/databaseresult.h"
#include <libpq-fe.h>
#include <stdexcept>

namespace app
{

    CDatabaseResult::CDatabaseResult() : m_result(nullptr) {}

    CDatabaseResult::CDatabaseResult(void* result) : m_result(result) {}

    CDatabaseResult::~CDatabaseResult()
    {
        if (m_result)
        {
            PQclear(static_cast<PGresult*>(m_result));
        }
    }

    CDatabaseResult::CDatabaseResult(CDatabaseResult&& other) noexcept
        : m_result(other.m_result)
    {
        other.m_result = nullptr;
    }

    CDatabaseResult& CDatabaseResult::operator=(CDatabaseResult&& other) noexcept
    {
        if (this != &other)
        {
            if (m_result)
            {
                PQclear(static_cast<PGresult*>(m_result));
            }
            m_result = other.m_result;
            other.m_result = nullptr;
        }
        return *this;
    }

    int CDatabaseResult::rows() const
    {
        return m_result ? PQntuples(static_cast<PGresult*>(m_result)) : 0;
    }

    int CDatabaseResult::columns() const
    {
        return m_result ? PQnfields(static_cast<PGresult*>(m_result)) : 0;
    }

    std::string CDatabaseResult::getValue(int row, int col) const
    {
        if (!m_result || row < 0 || row >= rows() || col < 0 || col >= columns())
        {
            return "";
        }
        return PQgetvalue(static_cast<PGresult*>(m_result), row, col);
    }

    int CDatabaseResult::getInt(int row, int col) const
    {
        std::string val = getValue(row, col);
        return val.empty() ? 0 : std::stoi(val);
    }

    bool CDatabaseResult::isNull(int row, int col) const
    {
        if (!m_result || row < 0 || row >= rows() || col < 0 || col >= columns())
        {
            return true;
        }
        return PQgetisnull(static_cast<PGresult*>(m_result), row, col);
    }

    std::string CDatabaseResult::getErrorMessage() const
    {
        return m_result ? PQresultErrorMessage(static_cast<PGresult*>(m_result)) : "Нет результата";
    }

    bool CDatabaseResult::isError() const
    {
        return m_result && PQresultStatus(static_cast<PGresult*>(m_result)) == PGRES_FATAL_ERROR;
    }
}
