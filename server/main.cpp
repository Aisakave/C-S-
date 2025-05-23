#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib") // 链接 Winsock 库

int main() {
    WSADATA wsadata{};
    // 初始化 Winsock 2.2 库（必须步骤）
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) == SOCKET_ERROR)
    {
        std::cout << "WSAStartup 初始化失败，错误号：" << WSAGetLastError() << std::endl;
        return -1;
    }

    // 创建 socket（TCP 协议 + IPv4 地址族）
    SOCKET servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (servSock == INVALID_SOCKET)
    {
        std::cout << "socket func error, error num is : " << WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    }

    // 准备绑定的本地地址结构
    sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;            // 使用 IPv4
    servAddr.sin_port = htons(2345);            // 端口号（转为网络字节序）
    inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr.S_un.S_addr); // 本地回环地址（localhost）

    // 将 socket 与地址绑定
    if (bind(servSock, (sockaddr*)&servAddr, sizeof(sockaddr)) == SOCKET_ERROR)
    {
        std::cout << "bind 绑定失败，错误号：" << WSAGetLastError() << std::endl;
        closesocket(servSock);
        WSACleanup();
        return -1;
    }

    // 开始监听（最多同时允许 128 个连接等待）
    if (listen(servSock, 128) == -1)
    {
        std::cout << "listen 监听失败，错误号：" << WSAGetLastError() << std::endl;
        closesocket(servSock);
        WSACleanup();
        return -1;
    }

    std::cout << "等待客户端连接..." << std::endl;

    // 接受客户端连接（阻塞式）
    SOCKET clntSock = accept(servSock, nullptr, nullptr);
    if (clntSock == INVALID_SOCKET)
    {
        std::cout << "accept 接受失败，错误号：" << WSAGetLastError() << std::endl;
        closesocket(servSock);
        WSACleanup();
        return -1;
    }
    std::cout << "客户端连接成功！" << std::endl;

    // 向客户端发送欢迎消息
    const char* msg = "Hello my first client";

    if (send(clntSock, msg, strlen(msg), 0) < 0)
    {
        std::cout << "send 发送失败，错误号：" << WSAGetLastError() << std::endl;
        closesocket(servSock);
        closesocket(clntSock);
        WSACleanup();
        return -1;
    }

    // 接收客户端发来的消息
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
        {			// 其他错误
            std::cout << "recv 接收失败，错误号：" << errorNum << std::endl;
            closesocket(servSock);
            closesocket(clntSock);
            WSACleanup();
            return -1;
        }
    }

    // 打印收到的消息
    std::cout << "recvBuff: " << recvBuff << std::endl;

    // 关闭连接和 socket
    closesocket(servSock);
    closesocket(clntSock);
    WSACleanup();
    std::cout << "服务器退出" << std::endl;
	system("pause");
    return 0;
}
