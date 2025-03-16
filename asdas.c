#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {

        char Len[100];
        char Leng[] = "\r\n";
            char buff[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: ";
            char content[] = "<!DOCTYPE html><html>\r\n<head>\r\n<input type=\"text\" name=\"username\">\r\n<input type=\"password\" name=\"password\">\r\n<button type=\"submit\">Login</button></head> </html>";

            sprintf(Len, "%ld", sizeof(content));
            int i = strlen(buff) + strlen(content) + strlen(Len) + strlen(Leng);
            char* buffer = (char*)malloc(sizeof(char) * i);
            strcpy(buffer, buff);
            strcat(buffer, Len);
            strcat(buffer, Leng);
            strcat(buffer, content);
            printf("%s", buffer);
    return 0;
}