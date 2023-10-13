#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

int main()
{
    // SMTP 伺服器資訊
    char *server = "smtp.example.com";
    int port = 25;

    // 寄件人和收件人的電子郵件地址
    char *from = "your_email@example.com";
    char *to = "recipient@example.com";

    // 你的郵件主題和內容
    char *subject = "Hello, SMTP Client";
    char *body = "This is a test email sent using a simple SMTP client.";

    int sockfd;
    struct hostent *server_info;
    struct sockaddr_in server_addr;
    char buffer[1024];

    // 建立套接字
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error opening socket");
        return 1;
    }

    server_info = gethostbyname(server);
    if (server_info == NULL)
    {
        perror("Error getting server host");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    bcopy((char *)server_info->h_addr, (char *)&server_addr.sin_addr.s_addr, server_info->h_length);

    // 連接到 SMTP 伺服器
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr) < 0) )
    {
        perror("Error connecting to server");
        return 1;
    }

    // 讀取伺服器回應
    recv(sockfd, buffer, sizeof(buffer), 0);
    printf("Server: %s", buffer);

    // 發送 EHLO 命令
    char *ehlo_command = "EHLO client.example.com\r\n";
    send(sockfd, ehlo_command, strlen(ehlo_command), 0);
    recv(sockfd, buffer, sizeof(buffer), 0);
    printf("Server: %s", buffer);

    // 發送寄件人命令
    char from_command[256];
    snprintf(from_command, sizeof(from_command), "MAIL FROM:<%s>\r\n", from);
    send(sockfd, from_command, strlen(from_command), 0);
    recv(sockfd, buffer, sizeof(buffer), 0);
    printf("Server: %s", buffer);

    // 發送收件人命令
    char to_command[256];
    snprintf(to_command, sizeof(to_command), "RCPT TO:<%s>\r\n", to);
    send(sockfd, to_command, strlen(to_command), 0);
    recv(sockfd, buffer, sizeof(buffer), 0);
    printf("Server: %s", buffer);

    // 發送郵件內容
    char data_command[] = "DATA\r\n";
    send(sockfd, data_command, sizeof(data_command), 0);
    recv(sockfd, buffer, sizeof(buffer), 0);
    printf("Server: %s", buffer);

    // 郵件主題和內容
    char email_content[1024];
    snprintf(email_content, sizeof(email_content), "Subject: %s\r\n\r\n%s\r\n.\r\n", subject, body);
    send(sockfd, email_content, strlen(email_content), 0);
    recv(sockfd, buffer, sizeof(buffer), 0);
    printf("Server: %s", buffer);

    // 退出 SMTP 會話
    char quit_command[] = "QUIT\r\n";
    send(sockfd, quit_command, sizeof(quit_command), 0);
    recv(sockfd, buffer, sizeof(buffer), 0);
    printf("Server: %s", buffer);

    // 關閉套接字
    close(sockfd);

    return 0;
}
