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
		std::cout << "WSAStartup ��ʼ��ʧ�ܣ�����ţ�" << WSAGetLastError() << std::endl;
		return false;
	}
	//socket��TCP Э�� + IPv4 ��ַ�壩!
	clntSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clntSock == INVALID_SOCKET)
	{
		std::cout << "socket func error, error num is : " << WSAGetLastError() << std::endl;
		WSACleanup();
		return false;
	}

	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;            // ʹ�� IPv4
	servAddr.sin_port = htons(port);            // �˿ںţ�תΪ�����ֽ���
	inet_pton(AF_INET, ip, &servAddr.sin_addr.S_un.S_addr); // ���ػػ���ַ��localhost��
	if (connect(clntSock, (sockaddr*)&servAddr, sizeof(sockaddr)) == SOCKET_ERROR)
	{
		std::cout << "connect ����ʧ�ܣ�����ţ�" << WSAGetLastError() << std::endl;
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
		std::cout << "send ����ʧ�ܣ�����ţ�" << WSAGetLastError() << std::endl;
		return false;
	}
	return true;
}
bool Client::recvMsg()
{
	recvLen = recv(clntSock, recvBuf, 128, 0);
	if (recvLen == 0)
	{
		std::cout << "�ͻ������ŵعر�������" << std::endl;
	}
	if (recvLen < 0)
	{
		int errorNum = WSAGetLastError();
		if (errorNum == 10054)
		{
			std::cout << "�ͻ���ǿ�ƶϿ����ӣ���ֱ�ӹرճ���" << std::endl;
		}
		else
		{
			std::cout << "recv ����ʧ�ܣ�����ţ�" << errorNum << std::endl;
			return false;
		}
	}
	return true;
}
const char* Client::getRecvBuf()
{
	return recvBuf;
}