#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
    // SMTP server listens on this port (usually 25)
    int port = 25;

    int sockfd, newsockfd;
    socklen_t clilen;
    char buffer[1024];
    struct sockaddr_in serv_addr, cli_addr;

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error opening socket");
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    // Bind the socket to the specified port
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error on binding");
        return 1;
    }

    // Listen for connections
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    printf("SMTP server listening on port %d\n", port);

    // Wait for client connections
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

    if (newsockfd < 0)
    {
        perror("Error on accept");
        return 1;
    }

    // After a connection is established, you can start receiving and processing SMTP commands.
    // This part involves parsing SMTP commands, executing corresponding actions, and sending responses to the client.

    // Example: Receive and print commands sent by the client
    while (1)
    {
        memset(buffer, 0, sizeof(buffer));
        if (recv(newsockfd, buffer, sizeof(buffer), 0) <= 0)
        {
            break;
        }
        printf("Client: %s", buffer);

        // Parse SMTP commands and execute corresponding actions here
    }

    // Close the socket
    close(newsockfd);
    close(sockfd);

    return 0;
}
