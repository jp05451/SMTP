#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstddef>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

using namespace std;

int main(int argc, char *argv[])
{
    // SMTP 伺服器資訊
    string server = "127.0.0.1";
    int port = 25;
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-p") == 0)
        // setting port number
        {
            port = atoi(argv[++i]);
        }
        if (strcmp(argv[i], "-h") == 0)
        // setting port number
        {
            server = argv[++i];
        }
    }

    // 寄件人和收件人的電子郵件地址
    string from, to;
    cout << "sender: ";
    cin >> from;
    cout << "reciever: ";
    cin >> to;

    // 你的郵件title和內容
    string subject = "Hello, SMTP Client";
    cout << "Please enter your title: ";
    cin >> subject;

    string body = "This is a test email sent using a simple SMTP client.";
    cout << "Please enter your email body: " << endl;
    cin >> body;

    // 建立套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error opening socket");
        return 1;
    }

    struct hostent *server_info = gethostbyname(server.c_str());
    if (server_info == NULL)
    {
        perror("Error getting server host");
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    bcopy((char *)server_info->h_addr, (char *)&server_addr.sin_addr.s_addr, server_info->h_length);

    // 連接到 SMTP 伺服器
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error connecting to server");
        return 1;
    }

    char buffer[1024];

    // 讀取伺服器回應
    recv(sockfd, buffer, sizeof(buffer), 0);
    cout << "Server: " << buffer;

    // 發送 HELO 命令
    string helo_command = "HELO client.example.com\r\n";
    cout << "HELO command" << endl
         << "Client: " << helo_command << endl;
    send(sockfd, helo_command.c_str(), helo_command.size(), 0);
    recv(sockfd, buffer, sizeof(buffer), 0);
    cout << "Server: " << buffer;

    // 發送寄件人命令
    string from_command = "MAIL FROM:<" + from + ">\r\n";
    cout << "FROM command" << endl
         << "Client: " << from_command << endl;

    send(sockfd, from_command.c_str(), from_command.size(), 0);
    recv(sockfd, buffer, sizeof(buffer), 0);
    cout << "Server: " << buffer;

    // 發送收件人命令
    string to_command = "RCPT TO:<" + to + ">\r\n";
    cout << "To command" << endl
         << "Client: " << to_command << endl;

    send(sockfd, to_command.c_str(), to_command.size(), 0);
    recv(sockfd, buffer, sizeof(buffer), 0);
    cout << "Server: " << buffer;

    // 發送郵件內容
    string data_command = "DATA\r\n";
    cout << "data command" << endl
         << "Client: " << data_command << endl;

    send(sockfd, data_command.c_str(), data_command.size(), 0);
    recv(sockfd, buffer, sizeof(buffer), 0);
    cout << "Server: " << buffer;

    // 郵件主題和內容
    string email_content = "Subject: " + subject + "\r\n\r\n" + body + "\r\n.\r\n";
    cout << "Client: " << email_content << endl;
    send(sockfd, email_content.c_str(), email_content.size(), 0);
    recv(sockfd, buffer, sizeof(buffer), 0);
    cout << "Server: " << buffer;

    // 退出 SMTP 會話
    string quit_command = "QUIT\r\n";
    cout << "quit command"
         << "Client: " << quit_command << endl;

    send(sockfd, quit_command.c_str(), quit_command.size(), 0);
    recv(sockfd, buffer, sizeof(buffer), 0);
    cout << "Server: " << buffer;

    // 關閉套接字
    close(sockfd);

    return 0;
}
