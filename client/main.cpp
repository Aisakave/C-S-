#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib") // ���� Winsock ��

int main() {
	// ��ʼ�� Winsock 2.2 �⣨���벽�裩
	WSADATA wsadata{};
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) == SOCKET_ERROR)
	{
		std::cout << "WSAStartup ��ʼ��ʧ�ܣ�����ţ�" << WSAGetLastError() << std::endl;
		return -1;
	}
	// ���� socket��TCP Э�� + IPv4 ��ַ�壩
	SOCKET clntSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clntSock == INVALID_SOCKET)
	{
		std::cout << "socket func error, error num is : " << WSAGetLastError() << std::endl;
		WSACleanup();
		return -1;
	}

	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;            // ʹ�� IPv4
	servAddr.sin_port = htons(2345);            // �˿ںţ�תΪ�����ֽ���
	inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr.S_un.S_addr); // ���ػػ���ַ��localhost��
	if (connect(clntSock, (sockaddr*)&servAddr, sizeof(sockaddr)) == SOCKET_ERROR)
	{
		std::cout << "connect ����ʧ�ܣ�����ţ�" << WSAGetLastError() << std::endl;
		closesocket(clntSock);
		WSACleanup();
		return -1;
	}

	char recvBuff[128] = { 0 };
	int recvLen = recv(clntSock, recvBuff, 128, 0);
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
			closesocket(clntSock);
			WSACleanup();
			return -1;
		}
	}

	if (send(clntSock, recvBuff, recvLen, 0) == SOCKET_ERROR)
	{
		std::cout << "send ����ʧ�ܣ�����ţ�" << WSAGetLastError() << std::endl;
		closesocket(clntSock);
		WSACleanup();
		return -1;
	}

	std::cout << "���յ����Է���������Ϣ��" << recvBuff << std::endl;

	closesocket(clntSock);
	WSACleanup();
	system("pause");
	return 0;
}