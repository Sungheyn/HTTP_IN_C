#include <stdio.h>
#include <WinSock2.h>
#include <string.h>
#pragma comment(lib, "ws2_32")

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET Server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    SOCKADDR_IN serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(8001);

    bind(Server, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    listen(Server, SOMAXCONN);
    char buf[1023];
	memset(buf, 0, sizeof(buf));
    while (1) {
        SOCKET client = accept(Server, NULL, NULL);
        recv(client, buf, sizeof(buf), 0);
        printf("%s\n", buf);
        char* b = strtok(buf, " ");
        char as[3] = { 'G','E','T' };
        if (buf[0] == as[0] && buf[1] == as[1] && buf[2] == as[2]) {
            char buff[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 11\r\n\r\nhello worl";
            buff[sizeof(buff) - 1] = 'd';
            printf("%s", buff);
            send(client, buff, strlen(buff), 0);
        }
    }
	WSACleanup();
	return 0;
}