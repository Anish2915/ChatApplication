#pragma once
#include <iostream>



namespace mongo {
	std::string GetIpAddressOfServer();
	void SetIpAddressOfServer(const char* IP_Address);
}
