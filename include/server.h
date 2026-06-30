#pragma once

#include <boost/asio.hpp>
#include <atomic>
#include <string>

namespace app
{
    class CServer
    {
      public:
        explicit CServer(unsigned short port);
        ~CServer();

        void run(std::atomic<bool>& running);
        void stop();

      private:
        void handle_client(boost::asio::ip::tcp::socket socket);
        std::string process_request(const std::string& json);

        boost::asio::io_context m_io_context;
        boost::asio::ip::tcp::acceptor m_acceptor;
        boost::asio::ip::tcp::socket m_socket;
    };
}
