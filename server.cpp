#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstddef>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

int main(int argc, char *argv[])
{
    int port = 25;
    // SMTP 伺服器監聽的埠口
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-p") == 0)
        {
            port = atoi(argv[++i]);
        }
    }

    int sockfd, newsockfd;
    socklen_t clilen;
    char buffer[1024];
    struct sockaddr_in serv_addr, cli_addr;

    // 建立套接字
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error opening socket");
        cout << "Error opening socket" << endl;
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    memset(&(serv_addr.sin_zero), 0, 8);

    // 綁定套接字到指定的埠口
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }
    // 監聽連接
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    // 等待客戶端連接
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

    if (newsockfd < 0)
    {
        perror("Error on accept");
        return 1;
    }
    // 連接建立後，可以開始接收和處理 SMTP 命令

    // 接收並列印客戶端發送的命令
    while (1)
    {
        bzero(buffer, 1024);
        if (recv(newsockfd, buffer, 1024, 0) <= 0)
        {
            break;
        }
        std::cout << "Client: " << buffer;
    }

    // 關閉套接字
    close(newsockfd);
    close(sockfd);

    return 0;
}