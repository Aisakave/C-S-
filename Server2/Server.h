#pragma once
#include<WinSock2.h>

class Server
{
public:
	Server();
	~Server();
	void close();
	bool start(const char* ip, unsigned short port);
	bool acceptClient();
	bool sendMsg(const char* message);
	bool recvMsg();
	const char* getRecvBuf();
private:
	bool init(const char* ip, unsigned short port);
	SOCKET servSock;
	SOCKET clntSock;
	char recvBuf[128]{};
};