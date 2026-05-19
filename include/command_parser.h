#pragma once

#include <string>

namespace app
{
    struct TCalculationCommand
    {
        TCalculationCommand() : m_eOperation(TCalculationCommand::eOperation::e_last){}
        enum class eOperation
        {
            ADD = 0, //Сложение
            SUB,     //Вычитание
            MUL,     //Умножение
            DIV,     //Деление
            POW,     //Возведение в степень
            FACT,    //Факториал
            e_last   //Заглушка
        };

        eOperation m_eOperation;
        int m_iOprnd1;
        int m_iOprnd2;
    };


    class CCommandParser
    {
      public:
        TCalculationCommand parse(const std::string& jsonString);
    };

}
