#include "command_parser.h"
#include "logger.h"
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>

namespace app
{

TCalculationCommand CCommandParser::parse(const std::string& jsonString)
{
    CLogger::getInstance().debug("Parsing JSON: " + jsonString);

    try
    {
        nlohmann::json json = nlohmann::json::parse(jsonString);

        TCalculationCommand cmd;

        if (!json.contains("operation"))
        {
            throw std::invalid_argument("Empty 'operation'");
        }

        std::string op = json["operation"];

        if (op == "+" || op == "add")
        {
            cmd.m_eOperation = TCalculationCommand::eOperation::ADD;
        }
        else if (op == "-" || op == "subtract")
        {
            cmd.m_eOperation = TCalculationCommand::eOperation::SUB;
        }
        else if (op == "*" || op == "multiply")
        {
            cmd.m_eOperation = TCalculationCommand::eOperation::MUL;
        }
        else if (op == "/" || op == "divide")
        {
            cmd.m_eOperation = TCalculationCommand::eOperation::DIV;
        }
        else if (op == "^" || op == "power")
        {
            cmd.m_eOperation = TCalculationCommand::eOperation::POW;
        }
        else if (op == "!" || op == "factorial")
        {
            cmd.m_eOperation = TCalculationCommand::eOperation::FACT;
        }
        else
        {
            throw std::invalid_argument("Unknown operation: " + op);
        }

        if (cmd.m_eOperation == TCalculationCommand::eOperation::FACT)
        {
            if (!json.contains("operand"))
            {
                throw std::invalid_argument("Empty 'operand' for factorial");
            }
            cmd.m_iOprnd1 = json["operand"];
            cmd.m_iOprnd2 = 0;
        }
        else
        {
            if (!json.contains("operand1"))
            {
                throw std::invalid_argument("Empty 'operand1'");
            }
            if (!json.contains("operand2"))
            {
                throw std::invalid_argument("v 'operand2'");
            }
            cmd.m_iOprnd1 = json["operand1"];
            cmd.m_iOprnd2 = json["operand2"];
        }

        CLogger::getInstance().debug("Success parsing");
        return cmd;

    }
    catch (const nlohmann::json::exception& e)
    {
        CLogger::getInstance().error("JSON parse error: " + std::string(e.what()));
        throw std::runtime_error("Invalid JSON format: " + std::string(e.what()));
    }
}

}
