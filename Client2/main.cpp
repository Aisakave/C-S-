#include "Client.h"
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

int main() {
	Client client;
	if (!client.start("127.0.0.1", 2345))
	{
		return -1;
	}
	if (!client.recvMsg())
	{
		return -1;
	}
	if (!client.sendMsg())
	{
		return -1;
	}
	std::cout << "Received message: " << client.getRecvBuf() << std::endl;
	system("pause");
	return 0;
}