#pragma once

#include <string>
#include <vector>

namespace app
{
    class CDatabaseConnection;
    class CDatabaseResult;

    /**
     * Обёртка для подготовленного запроса
     */
    class CDatabaseStatement
    {
      public:
        CDatabaseStatement(CDatabaseConnection& conn, const std::string& name, const std::string& sql);
        ~CDatabaseStatement();

        CDatabaseStatement(const CDatabaseStatement&) = delete;
        CDatabaseStatement& operator=(const CDatabaseStatement&) = delete;

        CDatabaseResult execute(const std::vector<std::string>& params = {});

      private:
        CDatabaseConnection& m_conn;
        std::string m_name;
    };
}
