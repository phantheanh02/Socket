#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>

#define BUFF_SIZE 128
#define SERV_PORT 5050
#define MAX_CLIENT 10

int main(int argc, char const *argv[])
{
    int receiveBytes, sendBytes, listenfd, connfd;
    char buff[BUFF_SIZE + 1];
    struct sockaddr_in serverAddr, clientAddr;

    // Step 1: Constructor socket
    listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenfd < 0)
    {
        perror("Error create socket: ");
        return 0;
    }

    // Step 2: Bind address to socket
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(SERV_PORT);
    if (bind(listenfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)))
    {
        perror("Error bind addres to socket: ");
        return 0;
    }

    // Step 3: Listen request from client
    if (listen(listenfd, MAX_CLIENT))
    {
        perror("Error! Can't listen.");
    }

    printf("Server started. \n");

    // Step 4: Waiting for connect request from client
    connfd = accept(listenfd, (struct sockaddr *)&clientAddr, &len);

    // Step 5: Communicate with client
    while (1)
    {
        memset(buff, '\0', (strlen(buff) + 1));
        printf("Waiting request from client...\n");

        // receive message from client
        receiveBytes = recv(connfd, buff, BUFF_SIZE, 0);
        if (receiveBytes < 0)
        {
            perror("Error recevie from client: ");
        }
        else
        {
            buff[receiveBytes] = '\0';
            printf("New message: ");
            printf("[%s:%d]: %s\n\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buff);

            sendBytes = send(connfd, buff, BUFF_SIZE, 0);
            if (sendBytes < 0)
            {
                perror("Error send to client: ");
            }
        }
    }

    return 0;
}
