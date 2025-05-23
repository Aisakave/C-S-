#include "Client.h"
#include <WS2tcpip.h>
#include <iostream>
Client::Client() :clntSock(INVALID_SOCKET)
{

}
Client::~Client()
{
	close();
}
void Client::close()
{
	if (clntSock != INVALID_SOCKET)
	{
		closesocket(clntSock);
		WSACleanup();
		clntSock = INVALID_SOCKET;
	}
}
bool Client::init(const char* ip, unsigned short port)
{
	WSADATA wsadata{};
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) == SOCKET_ERROR)
	{
		std::cout << "WSAStartup 初始化失败，错误号：" << WSAGetLastError() << std::endl;
		return false;
	}
	//socket（TCP 协议 + IPv4 地址族）
	clntSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clntSock == INVALID_SOCKET)
	{
		std::cout << "socket func error, error num is : " << WSAGetLastError() << std::endl;
		WSACleanup();
		return false;
	}

	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;            // 使用 IPv4
	servAddr.sin_port = htons(port);            // 端口号（转为网络字节序）
	inet_pton(AF_INET, ip, &servAddr.sin_addr.S_un.S_addr); // 本地回环地址（localhost）
	if (connect(clntSock, (sockaddr*)&servAddr, sizeof(sockaddr)) == SOCKET_ERROR)
	{
		std::cout << "connect 连接失败，错误号：" << WSAGetLastError() << std::endl;
		closesocket(clntSock);
		WSACleanup();
		return false;
	}
	return true;
}
bool Client::start(const char* ip, unsigned short port)
{
	if (!init(ip, port))
	{
		std::cout << "Client init failed" << std::endl;
		return false;
	}
	return true;
}
bool Client::sendMsg()
{
	if (send(clntSock, recvBuf, recvLen, 0) == SOCKET_ERROR)
	{
		std::cout << "send 发送失败，错误号：" << WSAGetLastError() << std::endl;
		return false;
	}
	return true;
}
bool Client::recvMsg()
{
	recvLen = recv(clntSock, recvBuf, 128, 0);
	if (recvLen == 0)
	{
		std::cout << "客户端优雅地关闭了连接" << std::endl;
	}
	if (recvLen < 0)
	{
		int errorNum = WSAGetLastError();
		if (errorNum == 10054)
		{
			std::cout << "客户端强制断开连接（如直接关闭程序）" << std::endl;
		}
		else
		{
			std::cout << "recv 接收失败，错误号：" << errorNum << std::endl;
			return false;
		}
	}
	return true;
}
const char* Client::getRecvBuf()
{
	return recvBuf;
}