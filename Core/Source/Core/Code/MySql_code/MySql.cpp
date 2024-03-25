#include "MySql.h"
#include <iostream>
#include <vector>
#include "mysql_driver.h"
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>


sql::Connection* mysql::con;

void mysql::ConnectToMySql(const std::string server , const std::string username , const std::string password)
{
    sql::Driver* driver; 

    try
    {
        driver = get_driver_instance();
        con = driver->connect(server, username, password);
    }
    catch (sql::SQLException e)
    {
        std::cout << "Could not connect to server. Error message: " << e.what() << std::endl;
        system("pause");
        exit(1);
    }
}

std::vector<std::pair<std::string, int>> mysql::GetConversationListFromUserId(int userId)
{
    std::vector<std::pair<std::string, int>> ConversationList; 

    sql::Statement* stmt;
    sql::ResultSet* res;
    sql::ResultSet* res2;

    con->setSchema("chatdata");

    stmt = con->createStatement(); 
    std::string user_id_input = std::to_string(userId);
    std::string query1 = "SELECT conversation_id FROM participants WHERE user_id = " + user_id_input;
    res = stmt->executeQuery(query1);
    while (res->next()) {
        int conversation_id = res->getInt("conversation_id");
        std::string query2 = "SELECT isGroup FROM conversations WHERE conversation_id = " + std::to_string(conversation_id);
        res2 = stmt->executeQuery(query2);
        if (res2->next()) {
            int isGroup = res2->getInt("isGroup");
            if (isGroup == 1) {
                std::string query3 = "SELECT conversation_name FROM conversations WHERE conversation_id = " + std::to_string(conversation_id);
                res2 = stmt->executeQuery(query3);
                if (res2->next())
                    ConversationList.push_back({ res2->getString("conversation_name"),conversation_id });
            }
            else if (isGroup == 0) {
                std::string query3 = "SELECT user_id FROM participants WHERE conversation_id = " + std::to_string(conversation_id);
                res2 = stmt->executeQuery(query3);
                while (res2->next()) {
                    if (res2->getInt("user_id") != userId) {
                        std::string query4 = "SELECT username FROM users WHERE user_id = " + std::to_string(res2->getInt("user_id"));
                        res2 = stmt->executeQuery(query4);
                        if (res2->next()) {
                            ConversationList.push_back({ res2->getString("username") , conversation_id });
                            std::cout << res2->getString("username") << "my sql wala jo abhi likha " << std::endl;
                        }
                    }
                }
            }
        }

    }
    return ConversationList;
}

std::vector<std::pair<std::string, std::string>> mysql::GetMessagesFromConversationId(int Conversation_id)
{
    std::vector<std::pair<std::string, std::string>> Message;
    sql::Statement* stmt;
    sql::ResultSet* res;
    sql::ResultSet* res2;
    con->setSchema("chatdata");
    stmt = con->createStatement();
    //std::string query1 = "SELECT sender_id,message_body FROM messages WHERE conversation_id = " + std::to_string(Conversation_id) + " ORDER BY message_id DESC LIMIT 10;";
    std::string query1 = "SELECT sender_id, message_body FROM ( SELECT message_id, sender_id, message_body FROM messages WHERE conversation_id = " + std::to_string(Conversation_id) + " ORDER BY message_id DESC LIMIT 10) AS subquery ORDER BY message_id ASC;";
    res = stmt->executeQuery(query1);
    while (res->next()) {
        std::pair<std::string, std::string> temp;
        temp.second = res->getString("message_body");
        int uid = res->getInt("sender_id");
        std::string query2 = "SELECT username from users WHERE user_id = " + std::to_string(uid);
        res2 = stmt->executeQuery(query2);
        if (res2->next()) {
            temp.first = res2->getString("username");
        }
        Message.push_back(temp);
    }

    return Message;
}

void mysql::SetUserIpAddress(int UserId, const std::string& ipAddress)
{
    sql::PreparedStatement* pstmt; 

    pstmt = con->prepareStatement("UPDATE users SET ip_address = ? WHERE user_id = ?"); 
    pstmt->setString(1, ipAddress); 
    pstmt->setInt(2, UserId);

    pstmt->executeUpdate(); 

    delete pstmt;
}

void mysql::AddMessage(int sender_id, int conversation_id, std::string message)
{
    sql::PreparedStatement* pstmt;

    // Prepare SQL statement
    pstmt = con->prepareStatement("INSERT INTO messages (conversation_id, sender_id, message_body) VALUES (?, ?, ?)");
    pstmt->setInt(1, conversation_id);
    pstmt->setInt(2, sender_id);
    pstmt->setString(3, message);

    // Execute the insert statement
    pstmt->executeUpdate();

    delete pstmt;
}

std::vector<std::string> mysql::GetUserIdFromConservationId(int conversation_id)
{
    std::vector<std::string> userIds;

    sql::Statement* stmt;
    sql::ResultSet* res;

    // Query to retrieve user IDs from participants table
    stmt = con->createStatement();
    std::string query1 = "SELECT user_id FROM participants WHERE conversation_id = " + std::to_string(conversation_id);
    res = stmt->executeQuery(query1);

    // Iterate over the results to get IP addresses for each user ID
    while (res->next()) {
        int userId = res->getInt("user_id");
        userIds.push_back(std::to_string(userId));
    }

    delete res;
    delete stmt;

    return userIds;
}

std::string mysql::GetUsername(int userId)
{
    std::string username;

    sql::Statement* stmt; 
    sql::ResultSet* res;

    stmt = con->createStatement(); 
    std::string query = "SELECT username FROM users WHERE user_id = " + std::to_string(userId); 
    res = stmt->executeQuery(query); 

    if (res->next()) { 
        username = res->getString("username");
    }

    delete res;
    delete stmt;

    return username;
}

std::pair<std::pair<std::string, std::string>, int> mysql::NewChat(std::string creater_id, std::string adder_id)
{
    std::pair<std::pair<std::string, std::string>, int> temp; 
    sql::Statement* stmt;
    sql::ResultSet* res;

    stmt = con->createStatement();
    std::string query = "SELECT user_id FROM users WHERE user_id = " + adder_id;
    res = stmt->executeQuery(query);

    if (!res->next()) {
        delete res;
        delete stmt;
        return temp;
    }

    query = "INSERT INTO conversations (conversation_name, participants) VALUES ('conv1', 2)";
    stmt->executeUpdate(query);

    query = "SELECT LAST_INSERT_ID()";
    res = stmt->executeQuery(query);
    if (res->next()) {
        int conversation_id = res->getInt(1);

        query = "INSERT INTO participants (user_id, conversation_id) VALUES (" + creater_id + ", " + std::to_string(conversation_id) + ")";
        stmt->executeUpdate(query);

        query = "INSERT INTO participants (user_id, conversation_id) VALUES (" + adder_id + ", " + std::to_string(conversation_id) + ")";
        stmt->executeUpdate(query);

        query = "SELECT username FROM users WHERE user_id = " + creater_id; 
        res = stmt->executeQuery(query); 
        if (res->next()) { 
            temp.first.first = res->getString("username");
        }

        query = "SELECT username FROM users WHERE user_id = " + adder_id; 
        res = stmt->executeQuery(query); 
        if (res->next()) { 
            temp.first.second = res->getString("username");
        }

        temp.second = conversation_id;
    }

    delete res;
    delete stmt;
}

int mysql::CreateGroup(std::string conv_name, int creater_id)
{
    sql::Statement* stmt; 
    sql::ResultSet* res; 

    stmt = con->createStatement(); 
    std::string query = "INSERT INTO conversations (conversation_name, participants, isGroup) VALUES ('" + conv_name + "', 1 , 1)";
    stmt->executeUpdate(query); 

    query = "SELECT LAST_INSERT_ID()";
    res = stmt->executeQuery(query);
    int conversation_id=0;
    if (res->next()) { 
        conversation_id = res->getInt(1);

        query = "INSERT INTO participants (user_id, conversation_id, is_admin) VALUES (" + std::to_string(creater_id) + ", " + std::to_string(conversation_id) + ", 1)";
        stmt->executeUpdate(query); 
    }

    delete res; 
    delete stmt; 
    return conversation_id; 
}

bool mysql::AddParticipant(std::string conversation_id, std::string other_id)
{
    sql::Statement* stmt; 
    sql::ResultSet* res;

    stmt = con->createStatement();

    std::string query = "SELECT user_id FROM users WHERE user_id = " + other_id; 
    res = stmt->executeQuery(query);

    if (!res->next()) {
        delete res;
        delete stmt;
        return false;
    }

    query = "SELECT conversation_id FROM conversations WHERE conversation_id = " + conversation_id + " AND isGroup = 1";
    res = stmt->executeQuery(query); 

    if (!res->next()) {
        delete res;
        delete stmt;
        return false;
    }  

    query = "INSERT INTO participants (user_id, conversation_id) VALUES (" + other_id + ", " + conversation_id + ")";
    stmt->executeUpdate(query);

    delete stmt;
    return true;
}

bool mysql::Login(int user_id, std::string username)
{
    sql::Statement* stmt;
    sql::ResultSet* res;

    // Check if user_id and username exist in users table
    stmt = con->createStatement();
    std::string query = "SELECT * FROM users WHERE user_id = " + std::to_string(user_id) + " AND username = '" + username + "'";
    res = stmt->executeQuery(query);

    bool exists = res->next();  // If next() returns true, then the user exists

    delete res;
    delete stmt;

    return exists;
}

int mysql::SignUp(std::string username)
{
    sql::Statement* stmt; 
    sql::ResultSet* res; 

    // Insert a new user into users table
    stmt = con->createStatement(); 
    std::string query = "INSERT INTO users (username, password, email, ip_address) VALUES ('" + username + "', 'test', 'test', 'test')"; 
    stmt->executeUpdate(query); 

    // Get the auto generated user_id 
    query = "SELECT LAST_INSERT_ID()"; 
    res = stmt->executeQuery(query); 
    int user_id = 0; 
    if (res->next()) { 
        user_id = res->getInt(1);
    }

    delete res; 
    delete stmt; 

    return user_id; 
}

std::string mysql::GetConversationNameFromId(std::string ConveId)
{
    sql::Statement* stmt; 
    sql::ResultSet* res; 
    std::string conversation_name; 

    stmt = con->createStatement(); 

    std::string query = "SELECT conversation_name FROM conversations WHERE conversation_id = " + ConveId; 
    res = stmt->executeQuery(query); 

    if (res->next()) {
        conversation_name = res->getString("conversation_name"); 
    }

    delete res;
    delete stmt;

    return conversation_name; 
}
