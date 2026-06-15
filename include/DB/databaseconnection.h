#pragma once

#include <string>

namespace app
{

/**
 * Обёртка для соединения с PostgreSQL (libpq)
 */
class CDatabaseConnection
{
  public:
    explicit CDatabaseConnection(const std::string& connString);
    ~CDatabaseConnection();

    CDatabaseConnection(const CDatabaseConnection&) = delete;
    CDatabaseConnection& operator=(const CDatabaseConnection&) = delete;
    CDatabaseConnection(CDatabaseConnection&& other) noexcept;
    CDatabaseConnection& operator=(CDatabaseConnection&& other) noexcept;

    bool isConnected() const;

  private:
    friend class CDatabaseResult;
    friend class CDatabaseStatement;
    friend class CDatabaseManager;

    void* m_conn;
};

}
