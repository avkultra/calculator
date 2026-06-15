#pragma once

#include <string>

namespace app
{

    /**
     * Обёртка для результата запроса в PostgreSQL
     */
    class CDatabaseResult
    {
      public:
        CDatabaseResult();
        explicit CDatabaseResult(void* result);  // PGresult*
        ~CDatabaseResult();

        CDatabaseResult(CDatabaseResult&& other) noexcept;
        CDatabaseResult& operator=(CDatabaseResult&& other) noexcept;

        int rows() const;
        int columns() const;
        std::string getValue(int row, int col) const;
        int getInt(int row, int col) const;
        bool isNull(int row, int col) const;
        std::string getErrorMessage() const;
        bool isError() const;

      private:
        void* m_result;  // PGresult*
    };

}
