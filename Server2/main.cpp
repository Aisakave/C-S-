#include"Server.h"
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

int main()
{
	Server server;
	if (!server.start("127.0.0.1", 2345))
	{
		return -1;
	}
	if (!server.acceptClient())
	{
		return -1;
	}
	const char* msg = "hello Server2";
	if (!server.sendMsg(msg))
	{
		return -1;
	}
	if (!server.recvMsg())
	{
		return -1;
	}
	std::cout << "Received message: " << server.getRecvBuf() << std::endl;
	std::cout << "·şÎñÆ÷ÍË³ö" << std::endl;
	system("pause");
	return 0;
}