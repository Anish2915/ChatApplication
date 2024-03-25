#include "Network.h"
#include <iostream>
#include "boost/asio.hpp"
#include <Windows.h>
#include <winhttp.h>
#include <vector>
#include "../MySql_code/MySql.h"
#include "../OpenGL_code/opengl.h"
#include <rapidjson.h>
#include <document.h>
#include <writer.h>
#include <stringbuffer.h>

#pragma comment(lib, "winhttp.lib")

boost::asio::io_context network::io_context;
std::unordered_map<std::string, std::shared_ptr<boost::asio::ip::tcp::socket>> network::SocketLists;

std::string network::GetMyPrivateIpAddress()
{
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::resolver resolver(io_context);

    // Get a list of network interfaces
    boost::asio::ip::tcp::resolver::query query(boost::asio::ip::host_name(), "");
    boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);
    boost::asio::ip::tcp::resolver::iterator end;

    try {
        while (iter != end) {
            boost::asio::ip::address addr = iter->endpoint().address();
            if (addr.is_v4()) {
                std::cout << "IPv4 Address: " << addr.to_string() << std::endl;
                return addr.to_string();
            }
            ++iter;
        }
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return "";
}

std::string network::GetMyPublicIpAddress()
{
    HINTERNET hSession = WinHttpOpen(L"User-Agent", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (hSession) {
        HINTERNET hConnect = WinHttpConnect(hSession, L"api.ipify.org", INTERNET_DEFAULT_HTTPS_PORT, 0);
        if (hConnect) {
            HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", L"?format=json", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
            if (hRequest) {
                if (WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
                    if (WinHttpReceiveResponse(hRequest, NULL)) {
                        DWORD bytesRead = 0;
                        BYTE buffer[1024];
                        std::string ipData;

                        while (WinHttpReadData(hRequest, buffer, sizeof(buffer), &bytesRead)) {
                            if (bytesRead > 0) {
                                ipData.append(reinterpret_cast<char*>(buffer), bytesRead);
                            }
                            else {
                                break;
                            }
                        }
                        std::string::size_type ipPos = ipData.find("\"ip\":\"");
                        if (ipPos != std::string::npos) {
                            // Extract the IP address
                            std::string::size_type endPos = ipData.find("\"", ipPos + 6); // Start from ipPos + 6 to skip "\"ip\":\""
                            if (endPos != std::string::npos) {
                                std::string ipAddress = ipData.substr(ipPos + 6, endPos - (ipPos + 6));
                                std::cout << "Public IP address: " << ipAddress << std::endl;
                                return ipAddress;
                            }
                        }
                        else {
                            std::cerr << "IP address not found in response." << std::endl;
                        }
                    }
                    else {
                        std::cerr << "Error receiving response." << std::endl;
                    }
                }
                else {
                    std::cerr << "Error sending request." << std::endl;
                }

                WinHttpCloseHandle(hRequest);
            }
            else {
                std::cerr << "Error opening request." << std::endl;
            }

            WinHttpCloseHandle(hConnect);
        }
        else {
            std::cerr << "Error connecting to server." << std::endl;
        }

        WinHttpCloseHandle(hSession);
    }
    else {
        std::cerr << "Error initializing WinHTTP." << std::endl;
    }
    return "";
}

std::string network::IPandUserIDtoJSON(const std::string& cmd, const std::string& userId, const std::string& raw_ip_address) {
    rapidjson::Document doc;
    doc.SetObject();

    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    rapidjson::Value keyCmd, keyUserId, keyip, valueCmd, valueUserId, valueip;
    keyCmd.SetString("Command", allocator);
    keyUserId.SetString("UserID", allocator);
    keyip.SetString("IPaddress", allocator);
    valueCmd.SetString(cmd.c_str(), static_cast<rapidjson::SizeType>(cmd.length()), allocator);
    valueUserId.SetString(userId.c_str(), static_cast<rapidjson::SizeType>(userId.length()), allocator);
    valueip.SetString(raw_ip_address.c_str(), static_cast<rapidjson::SizeType>(raw_ip_address.length()), allocator);

    doc.AddMember(keyCmd, valueCmd, allocator);
    doc.AddMember(keyUserId, valueUserId, allocator);
    doc.AddMember(keyip, valueip, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return buffer.GetString();
}

std::string network::ConversationIDandMessageTOjson(const std::string& cmd, const std::string& senderId, const std::string& conversationId, const std::string& message) {
    rapidjson::Document doc;
    doc.SetObject();

    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    rapidjson::Value kCmd, kSend, kConv, kMessage, vCmd, vSend, vConv, vMessage;
    kCmd.SetString("Command", allocator);
    kSend.SetString("SenderID", allocator);
    kConv.SetString("ConversationID", allocator);
    kMessage.SetString("Message", allocator);
    vCmd.SetString(cmd.c_str(), static_cast<rapidjson::SizeType>(cmd.length()), allocator);
    vSend.SetString(senderId.c_str(), static_cast<rapidjson::SizeType>(senderId.length()), allocator);
    vConv.SetString(conversationId.c_str(), static_cast<rapidjson::SizeType>(conversationId.length()), allocator);
    vMessage.SetString(message.c_str(), static_cast<rapidjson::SizeType>(message.length()), allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return buffer.GetString();
}

std::string network::StrIntPairTOjson(const std::vector<std::pair<std::string, int>>& StrIntData) {
    std::cout << "test 1 called" << std::endl;
    rapidjson::Document doc;
    doc.SetObject();

    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    for (const auto& pair : StrIntData) {
        rapidjson::Value key, value;
        key.SetString(pair.first.c_str(), static_cast<rapidjson::SizeType>(pair.first.length()), allocator);
        value.SetInt(pair.second);

        doc.AddMember(key, value, allocator);
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    std::cout << "test 1 finish" << std::endl;
    return buffer.GetString();
}

std::string network::StrPairTOjson(const std::vector<std::pair<std::string, std::string>> StrData) {
    rapidjson::Document doc;
    doc.SetObject();

    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    for (const auto pair : StrData) {
        rapidjson::Value key, value;
        key.SetString(pair.first.c_str(), static_cast<rapidjson::SizeType>(pair.first.length()), allocator);
        value.SetString(pair.second.c_str(), static_cast<rapidjson::SizeType>(pair.second.length()), allocator);

        doc.AddMember(key, value, allocator);
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    std::string ret = buffer.GetString();
    return ret;
}


// JSON to data
std::vector<std::pair<std::string, int>> network::jsonToStrIntPair(const std::string& jsonString) {
    std::vector<std::pair<std::string, int>> result;
    rapidjson::GenericMemberIterator<false, rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>> begin;

    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.IsObject()) {
        if (doc.HasMember("Command")) begin = doc.MemberBegin() + 1;
        else begin = doc.MemberBegin();
        for (auto it = begin; it != doc.MemberEnd(); ++it) {
            // if (it->value.IsInt()) {
            result.push_back(std::make_pair(it->name.GetString(), it->value.GetInt()));
            // }
        }
    }

    return result;
}

std::vector<std::pair<std::string, std::string>> network::jsonToStrPair(const std::string& jsonString) {
    std::vector<std::pair<std::string, std::string>> result;
    rapidjson::GenericMemberIterator<false, rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>> begin;

    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.IsObject()) {
        if (doc.HasMember("Command")) begin = doc.MemberBegin() + 1;

        else begin = doc.MemberBegin();
        for (auto it = begin; it != doc.MemberEnd(); ++it) {
            // if (it->value.IsString()) {
            result.push_back(std::make_pair(it->name.GetString(), it->value.GetString()));
            // }
        }
    }
    return result;
}

int network::getCommandValueFromJson(const std::string& jsonstring)
{
    int cmd = -1;
    rapidjson::Document doc;
    doc.Parse(jsonstring.c_str());

    if (doc.HasMember("Command")) { 
        if (doc["Command"].IsString()) { 
            cmd = std::stoi(doc["Command"].GetString()); 
        }
        else if (doc["Command"].IsInt()) { 
            cmd = doc["Command"].GetInt(); 
        }
    }
    return cmd;
}

// Client code
network::TCPClient::TCPClient(boost::asio::io_context& io_context, const std::string& server_address, const std::string& port )
    : io_context_(io_context), socket_(io_context) {
    boost::asio::ip::tcp::resolver resolver(io_context_);
    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(server_address, port);

    // Connect to server
    boost::asio::async_connect(socket_, endpoints,
        [this](boost::system::error_code ec, boost::asio::ip::tcp::endpoint endpoint) {
            if (!ec) {
                std::cout << "the user id opengl" << opengl::UserId << std::endl;
                write_buffer_ = network::IPandUserIDtoJSON(std::to_string(0), std::to_string(opengl::UserId), network::GetMyPublicIpAddress());
                sendData(write_buffer_);
                std::cout << "Connected to server: " << endpoint << std::endl;
            }
            else {
                std::cerr << "Failed to connect to server: " << ec.message() << std::endl;
            }
        });
}

void network::TCPClient::sendData(const std::string& data)
{
    std::cout << "The data sending is: " << data << std::endl;
    boost::asio::async_write(socket_, boost::asio::buffer(data),
        [this](boost::system::error_code ec, std::size_t length) {
            if (!ec) {
                std::cout << "Sent data successfully." << std::endl;
                startReading();
            }
            else {
                std::cerr << "Error while sending data: " << ec.message() << std::endl;
            }

    });
}

void network::TCPClient::startReading()
{
    auto buffer = std::make_shared<std::vector<char>>(1024);
    std::cout << "reading called" << std::endl;
    socket_.async_read_some(boost::asio::buffer(*buffer),
        [this, buffer](boost::system::error_code ec, std::size_t length) {
            if (!ec) {
                std::cout << "reading " << std::endl;
                std::string dataReadToClient(buffer->begin(), buffer->begin() + length);
                processDataInClient(dataReadToClient);
                startReading();
            }
            else {
                std::cerr << "Error while reading the data: " << ec.message() << std::endl;
            }
        });
}

void network::TCPClient::processDataInClient(const std::string& data)
{
    int comand = network::getCommandValueFromJson(data);
    if (comand == 0) {
        std::vector<std::pair<std::string, std::string>> DataAchieved = network::jsonToStrPair(data);
        std::vector<std::pair<std::string, int>> resultVector;
        for (const auto& pair : DataAchieved) {
            resultVector.emplace_back(pair.first, std::stoi(pair.second));
        }
        opengl::ConversationList = resultVector; 
        
    }
    else if (comand == 1) {
        std::vector<std::pair<std::string, std::string>> DataAchieved = network::jsonToStrPair(data);
        opengl::Messages = DataAchieved;
    }
    else if (comand == 2) {
        std::vector<std::pair<std::string, std::string>> DataAchieved = network::jsonToStrPair(data);
        if (opengl::COnvId == std::stoi(DataAchieved[2].second)) {
            opengl::Messages.push_back(std::make_pair(DataAchieved[0].second, DataAchieved[1].second));
        }
        for (int i = 0; i < opengl::ConversationList.size(); i++) {
            if (opengl::ConversationList[i].second == std::stoi(DataAchieved[2].second)) {
                auto temp = opengl::ConversationList[i];
                opengl::ConversationList[i] = opengl::ConversationList[0];
                opengl::ConversationList[0] = temp;
                break;
            }
        }
    }
    else if (comand == 3) {
        std::vector<std::pair<std::string, std::string>> DataAchieved = network::jsonToStrPair(data); 
        opengl::ConversationList.insert(opengl::ConversationList.begin(), { DataAchieved[1].second,std::stoi(DataAchieved[0].second) }); 
    }
    else if (comand == 5) {
        std::vector<std::pair<std::string, std::string>> DataAchieved = network::jsonToStrPair(data);  
        opengl::ConversationList.insert(opengl::ConversationList.begin(), { DataAchieved[2].second,std::stoi(DataAchieved[0].second) }); 
    }
}

void network::TCPClient::cancel()
{
    socket_.cancel();
}

void network::TCPClient::close()
{
    socket_.close();
}



// Server Code
network::TCPServer::TCPServer(boost::asio::io_context& io_context, const std::string& address, const std::string& port)
    : io_context_(io_context), acceptor_(io_context) {
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(address, port).begin();

    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    acceptConnection();
}

void network::TCPServer::acceptConnection()
{
    std::cout << "accept connection called" << std::endl;
    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(io_context_);
    acceptor_.async_accept(*socket, [this, socket](boost::system::error_code ec) {
        if (!ec) {
            std::cout << "Accepted connection from: " << socket->remote_endpoint() << std::endl;
            sockets_.push_back(socket);
            readData(socket);
        }
        else {
            std::cerr << "Error accepting connection: " << ec.message() << std::endl;
        }
        acceptConnection(); // Accept another connection
    });
}

void network::TCPServer::readData(std::shared_ptr<boost::asio::ip::tcp::socket> socket)
{
    auto buffer = std::make_shared<std::vector<char>>(1024);
    socket->async_read_some(boost::asio::buffer(*buffer),
        [this, socket, buffer](boost::system::error_code ec, std::size_t bytes_transferred) {
            if (!ec) {
                std::string data(buffer->begin(), buffer->begin() + bytes_transferred);
                std::cout << "Received data from " << socket->remote_endpoint() << ": " << data << std::endl;
                processData(data, socket);
                readData(socket);
            }
            else {
                std::cerr << "Error reading data: " << ec.message() << std::endl;
                close(socket); // Close the socket in case of error
            }
        });
}

void network::TCPServer::processData(const std::string& data, std::shared_ptr<boost::asio::ip::tcp::socket> source_socket)
{
    int cmd = network::getCommandValueFromJson(data);
    std::cout << cmd << std::endl;
    if (cmd == 0) {
        std::vector<std::pair<std::string, std::string>> ipANDuid = jsonToStrPair(data);
        //mysql::SetUserIpAddress(std::stoi(ipANDuid[0].second), ipANDuid[1].second);
        std::cout << ipANDuid[0].second << std::endl;
        std::vector<std::pair<std::string, int>> resultVector  = mysql::GetConversationListFromUserId(std::stoi(ipANDuid[0].second));
        std::vector<std::pair<std::string, std::string>> conversationData;
        for (const auto& pair : resultVector) {
            conversationData.emplace_back(pair.first, std::to_string(pair.second));
        }
        conversationData.insert(conversationData.begin(), std::make_pair(std::string("Command"), std::string("0")));
        network::SocketLists[ipANDuid[0].second] = source_socket;
        server_buffer_ = StrPairTOjson(conversationData);
        sendData(server_buffer_, source_socket);
    }
    else if (cmd == 1) {
        std::vector<std::pair<std::string, std::string>> converId = jsonToStrPair(data);
        int conversationid = std::stoi(converId[0].second);
        std::vector<std::pair<std::string, std::string>> TempMessages = mysql::GetMessagesFromConversationId(conversationid);
        TempMessages.insert(TempMessages.begin(), std::make_pair("Command", "1"));
        server_buffer_ = StrPairTOjson(TempMessages);
        sendData(server_buffer_, source_socket);
    }
    else if (cmd == 2) {
        std::vector<std::pair<std::string, std::string>> MessageToWrite = jsonToStrPair(data);
        int TempSenderId = std::stoi(MessageToWrite[0].second);
        int TempConversationID = std::stoi(MessageToWrite[1].second);
        std::string TempMessages = MessageToWrite[2].second;
        mysql::AddMessage(TempSenderId, TempConversationID, TempMessages);

        std::vector<std::string> IpList = mysql::GetUserIdFromConservationId(TempConversationID);
        std::vector<std::pair<std::string, std::string>> temp; 
        temp.push_back(std::make_pair("Command", "2")); 
        temp.push_back(std::make_pair("username", mysql::GetUsername(TempSenderId)));  
        temp.push_back(std::make_pair("message", TempMessages)); 
        temp.push_back(std::make_pair("id", std::to_string(TempConversationID))); 
        server_buffer_ = network::StrPairTOjson(temp);
        for (auto& it : IpList) {
            auto itr = network::SocketLists.find(it);
            if (itr != network::SocketLists.end()) {
                sendData(server_buffer_, itr->second);
            }
        }

    }
    else if (cmd == 3) {
        std::vector<std::pair<std::string, std::string>> MessageToWrite = jsonToStrPair(data);   
        std::pair<std::pair<std::string, std::string>, int> temp1 = mysql::NewChat(MessageToWrite[0].second, MessageToWrite[1].second);
        if (temp1.first.first.empty()) {
            return;
        }
        std::vector<std::pair<std::string, std::string>> temp; 
        temp.push_back(std::make_pair("Command", "3")); 
        temp.push_back(std::make_pair("Conversation_id",std::to_string(temp1.second)));
        temp.push_back(std::make_pair("other_name", temp1.first.second));
        server_buffer_ = network::StrPairTOjson(temp);
        sendData(server_buffer_, source_socket);
        temp[2].second = temp1.first.first;
        server_buffer_2 = network::StrPairTOjson(temp);  
        auto itr = network::SocketLists.find(MessageToWrite[1].second); 
        if (itr != network::SocketLists.end()) { 
            sendData(server_buffer_2, itr->second); 
        }
    }
    else if (cmd == 4) {
        std::vector<std::pair<std::string, std::string>> MessageToWrite = jsonToStrPair(data); 
        
        int conveid = mysql::CreateGroup(MessageToWrite[1].second, std::stoi(MessageToWrite[0].second)); 

        std::vector<std::pair<std::string, std::string>> temp;  
        temp.push_back(std::make_pair("Command", "3"));
        temp.push_back(std::make_pair("Conversation_id", std::to_string(conveid))); 
        temp.push_back(std::make_pair("other_name", MessageToWrite[1].second)); 
        server_buffer_ = network::StrPairTOjson(temp);
        sendData(server_buffer_, source_socket); 
    }
    else if (cmd == 5) {
        std::vector<std::pair<std::string, std::string>> MessageToWrite = jsonToStrPair(data); 
        if (mysql::AddParticipant(MessageToWrite[0].second, MessageToWrite[1].second)) { // conv id and user id 
            MessageToWrite.insert(MessageToWrite.begin(), {"Command","5"});
            MessageToWrite.push_back({"grpName",mysql::GetConversationNameFromId(MessageToWrite[1].second)}); 
            server_buffer_ = network::StrPairTOjson(MessageToWrite);
            auto itr = network::SocketLists.find(MessageToWrite[2].second);  
            if (itr != network::SocketLists.end()) {  
                sendData(server_buffer_, itr->second); 
            }
        }
    }
}

void network::TCPServer::sendData(const std::string& data, std::shared_ptr<boost::asio::ip::tcp::socket> socket)
{
    // network::msgToSendServer = data;
    std::cout << "The data sending is: " << data << std::endl;
    boost::asio::async_write(*socket, boost::asio::buffer(data),
        [this, socket](boost::system::error_code ec, std::size_t length) {
            if (!ec) {
                std::cout << "Sent data successfully [Inside send_data of SERVER]." << std::endl;
            }
            else {
                std::cerr << "Error while sending data: " << ec.message() << std::endl;
            }
        });
}

void network::TCPServer::cancel(std::shared_ptr<boost::asio::ip::tcp::socket> socket)
{
    socket->cancel();
}

void network::TCPServer::shutdown(std::shared_ptr<boost::asio::ip::tcp::socket> socket)
{
    socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
}

void network::TCPServer::close(std::shared_ptr<boost::asio::ip::tcp::socket> socket)
{
    socket->close();
    // Remove the socket from the list when closed
    sockets_.erase(std::remove(sockets_.begin(), sockets_.end(), socket), sockets_.end());
}

std::vector<std::shared_ptr<boost::asio::ip::tcp::socket>>& network::TCPServer::getSockets()
{
    return sockets_;
}
