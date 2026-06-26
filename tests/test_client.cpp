#include <string>
#include <iostream>
#include <boost/asio.hpp>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: test_client \"json_request\"" << std::endl;
        return 1;
    }

    try
    {
        boost::asio::io_context io;
        boost::asio::ip::tcp::resolver resolver(io);
        auto endpoints = resolver.resolve("127.0.0.1", std::to_string(8080));
        boost::asio::ip::tcp::socket socket(io);
        boost::asio::connect(socket, endpoints);

        std::string request = argv[1];
        request += "\n";
        boost::asio::write(socket, boost::asio::buffer(request));

        boost::asio::streambuf buf;
        boost::asio::read_until(socket, buf, '\n');
        std::string response = boost::asio::buffer_cast<const char*>(buf.data());
        if (!response.empty() && response.back() == '\n')
            response.pop_back();

        std::cout << response << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Client error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
