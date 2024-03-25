#include "Core/Core.h"
#include <thread>

int User_id;
std::string UserName;


// only for client 


//only for server

// common variables


void Server() {
	mysql::ConnectToMySql("tcp://127.0.0.1:3306", "root", "Anish@123");
	mongo::SetIpAddressOfServer(opengl::MyIpAddress.c_str());  
	network::TCPServer MyServer(network::io_context, "0.0.0.0", "3335");

	network::io_context.run();
}

void opengl_code() {
	opengl::WindowHeight = 900;
	opengl::WindowWidth = 1200;
	opengl::CreateWindowClient();
}

void Client() {

	std::cout << "Enter User Id" << std::endl; 
	std::cin >> opengl::UserId; 
	std::cout << "Enter User Name" << std::endl; 
	std::cin >> opengl::UserName; 
	
	opengl::ServerIpAddress = mongo::GetIpAddressOfServer();  

	std::thread opengl_thread(opengl_code);

	network::TCPClient MyClient(network::io_context,opengl::ServerIpAddress, "3335");
	opengl::MyClientGui = &MyClient;


	network::io_context.run();
}





int main()
{
	opengl::MyIpAddress = network::GetMyPrivateIpAddress();
  
	Server(); 
	// \
	Client(); 
	
	
}