#pragma once
#include <iostream>
#include <boost/asio.hpp>
#include <unordered_map>

namespace network {

    std::string GetMyPrivateIpAddress();

    std::string GetMyPublicIpAddress();
    struct WriteSession {
        std::shared_ptr<boost::asio::ip::tcp::socket> sock;
        std::string buf;
        std::size_t total_bytes_written;
    };

    struct ReadSession {
        std::shared_ptr<boost::asio::ip::tcp::socket> sock;
        std::unique_ptr<char[]> buf;
        std::size_t total_bytes_read;
        unsigned int buf_size;
    };

    class TCPClient {
        public:
            using ReadHandler = std::function<void(const std::string&)>;

            TCPClient(boost::asio::io_context& io_context, const std::string& server_address, const std::string& port);

            void sendData(const std::string& data);

            void startReading();

            void processDataInClient(const std::string& data);

            void cancel();

            void close();

            std::string write_buffer_;
        private:
            boost::asio::io_context& io_context_;
            boost::asio::ip::tcp::socket socket_;
            std::string read_buffer_;
    };

    class TCPServer {
        public:
            TCPServer(boost::asio::io_context& io_context, const std::string& address, const std::string& port);

            void acceptConnection();

            void readData(std::shared_ptr<boost::asio::ip::tcp::socket> socket);

            void processData(const std::string& data, std::shared_ptr<boost::asio::ip::tcp::socket> source_socket);

            void sendData(const std::string& data, std::shared_ptr<boost::asio::ip::tcp::socket> socket);

            void cancel(std::shared_ptr<boost::asio::ip::tcp::socket> socket);

            void shutdown(std::shared_ptr<boost::asio::ip::tcp::socket> socket);

            void close(std::shared_ptr<boost::asio::ip::tcp::socket> socket);

            std::vector<std::shared_ptr<boost::asio::ip::tcp::socket>>& getSockets();

        private:
            boost::asio::io_context& io_context_;
            boost::asio::ip::tcp::acceptor acceptor_;
            std::vector<std::shared_ptr<boost::asio::ip::tcp::socket>> sockets_;
            std::string server_buffer_;
            std::string server_buffer_2;
    };

    extern boost::asio::io_context io_context;
    extern std::unordered_map<std::string, std::shared_ptr<boost::asio::ip::tcp::socket>> SocketLists;

    std::string IPandUserIDtoJSON(const std::string& cmd, const std::string& userId, const std::string& raw_ip_address);
    std::string ConversationIDandMessageTOjson(const std::string& cmd, const std::string& senderId, const std::string& conversationId, const std::string& message);
    std::string StrIntPairTOjson(const std::vector<std::pair<std::string, int>>& StrIntData);  
    std::string StrPairTOjson(const std::vector<std::pair<std::string, std::string>> StrData); 
    std::vector<std::pair<std::string, int>> jsonToStrIntPair(const std::string& jsonString); 
    std::vector<std::pair<std::string, std::string>> jsonToStrPair(const std::string& jsonString);

    int getCommandValueFromJson(const std::string& jsonstring);
    
    
}

