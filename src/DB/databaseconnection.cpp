#include <libpq-fe.h>
#include "DB/databaseconnection.h"
#include <string>
#include <stdexcept>

namespace app
{
    CDatabaseConnection::CDatabaseConnection(const std::string& connString)
    {
        m_conn = PQconnectdb(connString.c_str());
        if (PQstatus(static_cast<PGconn*>(m_conn)) != CONNECTION_OK)
        {
            std::string error = PQerrorMessage(static_cast<PGconn*>(m_conn));
            PQfinish(static_cast<PGconn*>(m_conn));
            throw std::runtime_error("Couldn't connect to the database: " + error);
        }
    }

    CDatabaseConnection::~CDatabaseConnection()
    {
        if (m_conn)
        {
            PQfinish(static_cast<PGconn*>(m_conn));
        }
    }

    CDatabaseConnection::CDatabaseConnection(CDatabaseConnection&& other) noexcept
        : m_conn(other.m_conn)
    {
        other.m_conn = nullptr;
    }

    CDatabaseConnection& CDatabaseConnection::operator=(CDatabaseConnection&& other) noexcept
    {
        if (this != &other)
        {
            if (m_conn)
            {
                PQfinish(static_cast<PGconn*>(m_conn));
            }
            m_conn = other.m_conn;
            other.m_conn = nullptr;
        }
        return *this;
    }

    bool CDatabaseConnection::isConnected() const
    {
        return m_conn && PQstatus(static_cast<PGconn*>(m_conn)) == CONNECTION_OK;
    }
}
