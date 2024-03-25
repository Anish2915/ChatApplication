#pragma once
#include <iostream>
#include <vector>
#include "mysql_connection.h"

namespace mysql {
	
	extern sql::Connection* con; 
	void ConnectToMySql(const std::string server, const std::string username, const std::string password);
	std::vector<std::pair<std::string, int>> GetConversationListFromUserId(int userId);
	std::vector<std::pair<std::string, std::string>> GetMessagesFromConversationId(int Conversation_id);

	void SetUserIpAddress(int UserId, const std::string& ipAddress);
	void AddMessage(int sender_id, int conversation_id, std::string message);

	std::vector<std::string> GetUserIdFromConservationId(int conversation_id);
	std::string GetUsername(int userId);

	std::pair<std::pair<std::string, std::string>,int> NewChat(std::string creater_id, std::string adder_id);
	int CreateGroup(std::string conv_name, int creater_id);
	bool AddParticipant(std::string conversation_id, std::string other_id);

	bool Login(int user_id, std::string username);
	int SignUp(std::string username);

	std::string GetConversationNameFromId(std::string ConveId);
}
