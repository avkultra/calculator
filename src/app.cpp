#include "app.h"
#include <string>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include "command_parser.h"
#include "calculator.h"
#include "logger.h"

namespace app
{
    class COutputWriter
    {
      public:
        void writeInt(int num)
        {
            std::string str = std::to_string(num);
            write(str);
        }

        void writeChar(char c)
        {
            write(std::string(1, c));
        }

        void writeStr(const std::string& str)
        {
            write(str);
        }

        void writeLine(const std::string& str)
        {
            write(str + "\n");
        }

      private:
        void write(const std::string& str)
        {
            std::cout << str;
        }
    };

    int CApplication::run(int argc, char* argv[])
    {
        try
        {
            CLogger::getInstance().info("Application started");

            // Проверка колличества аргументов
            if (argc != 2)
            {
                std::string usage = "Использование: " + std::string(argv[0]) + " '<json_string>'\n"
                                                                       "Пример: " + std::string(argv[0]) + " '{\"operation\":\"+\",\"operand1\":10,\"operand2\":5}'\n"
                                                                        "Факториал: " + std::string(argv[0]) + " '{\"operation\":\"!\",\"operand\":5}'\n";
                std::cout << usage;
                return 1;
            }

            std::string jsonString = argv[1];
            CCommandParser parser;
            TCalculationCommand command = parser.parse(jsonString);

            CCalculator calculator;
            COutputWriter writer;
            int result = 0;

            switch (command.m_eOperation)
            {
                case TCalculationCommand::eOperation::ADD:
                {
                    result = calculator.add(command.m_iOprnd1, command.m_iOprnd2);
                    writer.writeInt(command.m_iOprnd1);
                    writer.writeChar(' ');
                    writer.writeChar('+');
                    writer.writeChar(' ');
                    writer.writeInt(command.m_iOprnd2);
                    writer.writeStr(" = ");
                    writer.writeInt(result);
                    writer.writeChar('\n');
                }
                break;

                case TCalculationCommand::eOperation::SUB:
                {
                    result = calculator.subtract(command.m_iOprnd1, command.m_iOprnd2);
                    writer.writeInt(command.m_iOprnd1);
                    writer.writeChar(' ');
                    writer.writeChar('-');
                    writer.writeChar(' ');
                    writer.writeInt(command.m_iOprnd2);
                    writer.writeStr(" = ");
                    writer.writeInt(result);
                    writer.writeChar('\n');
                }
                break;

                case TCalculationCommand::eOperation::MUL:
                {
                    result = calculator.multiply(command.m_iOprnd1, command.m_iOprnd2);
                    writer.writeInt(command.m_iOprnd1);
                    writer.writeChar(' ');
                    writer.writeChar('*');
                    writer.writeChar(' ');
                    writer.writeInt(command.m_iOprnd2);
                    writer.writeStr(" = ");
                    writer.writeInt(result);
                    writer.writeChar('\n');
                }
                break;

                case TCalculationCommand::eOperation::DIV:
                {
                    result = calculator.divide(command.m_iOprnd1, command.m_iOprnd2);
                    writer.writeInt(command.m_iOprnd1);
                    writer.writeChar(' ');
                    writer.writeChar('/');
                    writer.writeChar(' ');
                    writer.writeInt(command.m_iOprnd2);
                    writer.writeStr(" = ");
                    writer.writeInt(result);
                    writer.writeChar('\n');
                }
                break;

                case TCalculationCommand::eOperation::POW:
                {
                    result = calculator.power(command.m_iOprnd1, command.m_iOprnd2);
                    writer.writeInt(command.m_iOprnd1);
                    writer.writeChar('^');
                    writer.writeInt(command.m_iOprnd2);
                    writer.writeStr(" = ");
                    writer.writeInt(result);
                    writer.writeChar('\n');
                }
                break;

                case TCalculationCommand::eOperation::FACT:
                {
                    result = calculator.factorial(command.m_iOprnd1);
                    writer.writeInt(command.m_iOprnd1);
                    writer.writeChar('!');
                    writer.writeStr(" = ");
                    writer.writeInt(result);
                    writer.writeChar('\n');
                }
                break;
            }

            CLogger::getInstance().info("Calculation success");
            return 0;

        }
        catch (const std::exception& e)
        {
            CLogger::getInstance().error("Error: " + std::string(e.what()));
            std::cout << "Error: " << e.what() << std::endl;
            return 1;
        }
    }

}
