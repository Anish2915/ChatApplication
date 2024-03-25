#pragma once
#include <iostream>
#include <vector>
#include "../Network_code/Network.h"

class GLFWwindow;



namespace opengl {
	extern std::vector<std::pair<std::string, int>> ConversationList;
	extern std::vector<std::pair<std::string, std::string>> Messages;

	extern std::string UserName;
	extern std::string MyIpAddress;
	extern int UserId;
	extern std::string ServerIpAddress;
	extern std::string ChatName;

	extern network::TCPClient* MyClientGui;
	extern int COnvId;
	extern float WindowWidth;
	extern float WindowHeight;
	void CreateWindowClient();
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void StartOpenGLWindowLoop(GLFWwindow* window);
	void SetCallback(GLFWwindow* window);

	void SetUserId(int a);
}