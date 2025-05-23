#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib") // 链接 Winsock 库

int main() {
	// 初始化 Winsock 2.2 库（必须步骤）
	WSADATA wsadata{};
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) == SOCKET_ERROR)
	{
		std::cout << "WSAStartup 初始化失败，错误号：" << WSAGetLastError() << std::endl;
		return -1;
	}
	// 创建 socket（TCP 协议 + IPv4 地址族）
	SOCKET clntSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clntSock == INVALID_SOCKET)
	{
		std::cout << "socket func error, error num is : " << WSAGetLastError() << std::endl;
		WSACleanup();
		return -1;
	}

	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;            // 使用 IPv4
	servAddr.sin_port = htons(2345);            // 端口号（转为网络字节序）
	inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr.S_un.S_addr); // 本地回环地址（localhost）
	if (connect(clntSock, (sockaddr*)&servAddr, sizeof(sockaddr)) == SOCKET_ERROR)
	{
		std::cout << "connect 连接失败，错误号：" << WSAGetLastError() << std::endl;
		closesocket(clntSock);
		WSACleanup();
		return -1;
	}

	char recvBuff[128] = { 0 };
	int recvLen = recv(clntSock, recvBuff, 128, 0);
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
			closesocket(clntSock);
			WSACleanup();
			return -1;
		}
	}

	if (send(clntSock, recvBuff, recvLen, 0) == SOCKET_ERROR)
	{
		std::cout << "send 发送失败，错误号：" << WSAGetLastError() << std::endl;
		closesocket(clntSock);
		WSACleanup();
		return -1;
	}

	std::cout << "接收到来自服务器的消息：" << recvBuff << std::endl;

	closesocket(clntSock);
	WSACleanup();
	system("pause");
	return 0;
}