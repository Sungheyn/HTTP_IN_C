#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main() {
    int Server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8001);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    
    bind(Server, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(Server, SOMAXCONN);
    
    while (1) {
        int client = accept(Server, NULL, NULL);
        char buf[4096];
        memset(buf, 0, sizeof(buf));
        ssize_t bytesReceived = recv(client, buf, sizeof(buf) - 1, 0);
        if (bytesReceived <= 0) {
            close(client);
            continue;
        }
        
        if (strncmp(buf, "POST", 4) == 0) {
            char *body = strstr(buf, "\r\n\r\n");
            if (body != NULL) {
                body += 4;  

                FILE *fp = fopen("output.txt", "w");
                if (fp != NULL) {
                    fprintf(fp, "%s", body);
                    fclose(fp);
                    printf("파일에 저장 성공: output.txt\n");
                } else {
                    perror("파일 열기 실패");
                }
                
                char response[] = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";
                send(client, response, strlen(response), 0);
            } else {
                char response[] = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
                send(client, response, strlen(response), 0);
            }
        } else if (strncmp(buf, "GET", 3) == 0) {
            char header[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: ";
            char content[] =
                "<!DOCTYPE html>\n"
                "<html lang=\"ko\">\n"
                "<head>\n"
                "    <meta charset=\"UTF-8\">\n"
                "    <title>로그인 및 본문 입력</title>\n"
                "    <style>\n"
                "        body { font-family: Arial, sans-serif; margin: 20px; }\n"
                "        .form-group { margin-bottom: 15px; }\n"
                "        label { display: block; margin-bottom: 5px; }\n"
                "        input { width: 300px; padding: 8px; }\n"
                "        button { padding: 8px 16px; }\n"
                "    </style>\n"
                "</head>\n"
                "<body>\n"
                "    <h1>로그인 및 본문 입력</h1>\n"
                "    <form action=\"http://127.0.0.1:8001\" method=\"POST\">\n"
                "        <div class=\"form-group\">\n"
                "            <label for=\"username\">유저 네임:</label>\n"
                "            <input type=\"text\" id=\"username\" name=\"username\" required>\n"
                "        </div>\n"
                "        <div class=\"form-group\">\n"
                "            <label for=\"password\">비밀 번호:</label>\n"
                "            <input type=\"password\" id=\"password\" name=\"password\" required>\n"
                "        </div>\n"
                "        <div class=\"form-group\">\n"
                "            <label for=\"content\">본문 입력:</label>\n"
                "            <input type=\"text\" id=\"content\" name=\"content\" required>\n"
                "        </div>\n"
                "        <div class=\"form-group\">\n"
                "            <button type=\"submit\">Login</button>\n"
                "        </div>\n"
                "    </form>\n"
                "</body>\n"
                "</html>";
            int contentLength = strlen(content);
            char headerBuffer[512];
            sprintf(headerBuffer, "%s%d\r\n\r\n", header, contentLength);
            char response[8192];
            snprintf(response, sizeof(response), "%s%s", headerBuffer, content);
            send(client, response, strlen(response), 0);
        }
        
        close(client);
    }
    
    close(Server);
    return 0;
}
