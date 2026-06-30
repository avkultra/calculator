#include "server.h"
#include "app.h"
#include "calculator.h"
#include "command_parser.h"
#include "DB/databasemanager.h"
#include "logger.h"
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>
#include <iostream>
#include <thread>

namespace app
{
    CServer::CServer(unsigned short port)
        : m_acceptor(m_io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
        m_socket(m_io_context)
    {
        CLogger::getInstance().info("Server listening on port " + std::to_string(port));
    }

    CServer::~CServer()
    {
        stop();
    }

    void CServer::run(std::atomic<bool>& running)
    {
        CLogger::getInstance().info("Server thread started");
        while (running)
        {
            try
            {
                CLogger::getInstance().debug("Waiting for client...");
                // Принимаем одно соединение
                m_acceptor.accept(m_socket);
                CLogger::getInstance().info("Client connected");

                // Читаем запрос до символа '\n'
                boost::asio::streambuf buf;
                boost::asio::read_until(m_socket, buf, '\n');
                std::string request = boost::asio::buffer_cast<const char*>(buf.data());
                if (!request.empty() && request.back() == '\n')
                    request.pop_back();

                CLogger::getInstance().debug("Received: " + request);

                // Обрабатываем запрос
                std::string response = process_request(request);
                response += "\n";

                // Отправляем ответ
                boost::asio::write(m_socket, boost::asio::buffer(response));

                // Закрываем соединение
                m_socket.close();
                CLogger::getInstance().debug("Client disconnected");
            }
            catch (const boost::system::system_error& e)
            {
                if (running)
                    CLogger::getInstance().error("Server error: " + std::string(e.what()));
            }
            catch (const std::exception& e)
            {
                CLogger::getInstance().error("Server error: " + std::string(e.what()));
            }
        }
        CLogger::getInstance().info("Server thread stopped");
    }

    void CServer::stop()
    {
        boost::system::error_code ec;
        m_acceptor.close(ec);
        m_socket.close(ec);
        m_io_context.stop();
    }

    std::string CServer::process_request(const std::string& json)
    {
        try
        {
            CCommandParser parser;
            TCalculationCommand cmd = parser.parse(json);

            CCalculator calculator;
            int result = 0;
            bool bOk = true;
            std::string error_msg;

            try
            {
                switch (cmd.m_eOperation)
                {
                    case TCalculationCommand::eOperation::ADD:
                    {
                        result = calculator.addCached(cmd.m_iOprnd1, cmd.m_iOprnd2);
                    }
                    break;
                    case TCalculationCommand::eOperation::SUB:
                    {
                        result = calculator.subtract(cmd.m_iOprnd1, cmd.m_iOprnd2);
                    }
                    break;
                    case TCalculationCommand::eOperation::MUL:
                    {
                        result = calculator.multiply(cmd.m_iOprnd1, cmd.m_iOprnd2);
                    }
                    break;
                    case TCalculationCommand::eOperation::DIV:
                    {
                        result = calculator.divide(cmd.m_iOprnd1, cmd.m_iOprnd2);
                    }
                    break;
                    case TCalculationCommand::eOperation::POW:
                    {
                        result = calculator.power(cmd.m_iOprnd1, cmd.m_iOprnd2);
                    }
                    break;
                    case TCalculationCommand::eOperation::FACT:
                    {
                        result = calculator.factorial(cmd.m_iOprnd1);

                    }
                    break;
                    default: throw std::runtime_error("Unknown operation");
                }
            }
            catch (const std::exception& e)
            {
                bOk = false;
                error_msg = e.what();
            }

                   // Формируем ответ (можно JSON или просто строку)
            if (bOk)
            {
                return std::to_string(result);
            }
            else
            {
                return "ERROR: " + error_msg;
            }
        }
        catch (const std::exception& e)
        {
            CLogger::getInstance().error("Request processing error: " + std::string(e.what()));
            return "ERROR: Invalid request";
        }
    }
}
