#pragma once
#include <WinSock2.h>

class Client
{
public:
	Client();
	~Client();
	void close();
	bool init(const char* ip, unsigned short port);
	bool start(const char* ip, unsigned short port);
	bool sendMsg();
	bool recvMsg();
	const char* getRecvBuf();

private:
	SOCKET clntSock;
	char recvBuf[128] = { 0 };
	int recvLen = 0;
};