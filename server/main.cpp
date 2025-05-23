#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib") // ���� Winsock ��

int main() {
    WSADATA wsadata{};
    // ��ʼ�� Winsock 2.2 �⣨���벽�裩
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) == SOCKET_ERROR)
    {
        std::cout << "WSAStartup ��ʼ��ʧ�ܣ�����ţ�" << WSAGetLastError() << std::endl;
        return -1;
    }

    // ���� socket��TCP Э�� + IPv4 ��ַ�壩
    SOCKET servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (servSock == INVALID_SOCKET)
    {
        std::cout << "socket func error, error num is : " << WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    }

    // ׼���󶨵ı��ص�ַ�ṹ
    sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;            // ʹ�� IPv4
    servAddr.sin_port = htons(2345);            // �˿ںţ�תΪ�����ֽ���
    inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr.S_un.S_addr); // ���ػػ���ַ��localhost��

    // �� socket ���ַ��
    if (bind(servSock, (sockaddr*)&servAddr, sizeof(sockaddr)) == SOCKET_ERROR)
    {
        std::cout << "bind ��ʧ�ܣ�����ţ�" << WSAGetLastError() << std::endl;
        closesocket(servSock);
        WSACleanup();
        return -1;
    }

    // ��ʼ���������ͬʱ���� 128 �����ӵȴ���
    if (listen(servSock, 128) == -1)
    {
        std::cout << "listen ����ʧ�ܣ�����ţ�" << WSAGetLastError() << std::endl;
        closesocket(servSock);
        WSACleanup();
        return -1;
    }

    std::cout << "�ȴ��ͻ�������..." << std::endl;

    // ���ܿͻ������ӣ�����ʽ��
    SOCKET clntSock = accept(servSock, nullptr, nullptr);
    if (clntSock == INVALID_SOCKET)
    {
        std::cout << "accept ����ʧ�ܣ�����ţ�" << WSAGetLastError() << std::endl;
        closesocket(servSock);
        WSACleanup();
        return -1;
    }
    std::cout << "�ͻ������ӳɹ���" << std::endl;

    // ��ͻ��˷��ͻ�ӭ��Ϣ
    const char* msg = "Hello my first client";

    if (send(clntSock, msg, strlen(msg), 0) < 0)
    {
        std::cout << "send ����ʧ�ܣ�����ţ�" << WSAGetLastError() << std::endl;
        closesocket(servSock);
        closesocket(clntSock);
        WSACleanup();
        return -1;
    }

    // ���տͻ��˷�������Ϣ
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
        {			// ��������
            std::cout << "recv ����ʧ�ܣ�����ţ�" << errorNum << std::endl;
            closesocket(servSock);
            closesocket(clntSock);
            WSACleanup();
            return -1;
        }
    }

    // ��ӡ�յ�����Ϣ
    std::cout << "recvBuff: " << recvBuff << std::endl;

    // �ر����Ӻ� socket
    closesocket(servSock);
    closesocket(clntSock);
    WSACleanup();
    std::cout << "�������˳�" << std::endl;
	system("pause");
    return 0;
}
