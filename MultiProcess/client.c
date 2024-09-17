#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <netdb.h>

#define BUFF_SIZE 128
#define SERV_PORT 5050
#define SERV_ADDR "127.0.0.1"

int main(int argc, char const *argv[])
{
    int sockfd, receiveBytes, sendBytes;
    char buff[BUFF_SIZE + 1], receiveMsg[BUFF_SIZE + 1];
    struct sockaddr_in serverAddr;

    // Step 1: Constructor socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error create socket: ");
        return 0;
    }

    // Step 2: Define the address of the server
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERV_ADDR);
    serverAddr.sin_port = htons(SERV_PORT);

    // Step 3: Connect with server
    if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)))
    {
        perror("Error connect with server\n");
        return 0;
    }
    
    // Step 4: Communicate with server
    while (1)
    {
        memset(buff,'\0',(strlen(buff)+1));
        memset(receiveMsg,'\0',(strlen(buff)+1));

        printf("Send to server: ");
        scanf("%s", buff);

        sendBytes = send(sockfd, buff, BUFF_SIZE, 0);
        if (sendBytes < 0)
        {
            perror("Error send to server: ");
            return 0;
        }

        receiveBytes = recv(sockfd, receiveMsg, BUFF_SIZE, 0);
        if (receiveBytes < 0)
        {
            perror("Error recevie from server: ");
            return 0;
        }
        receiveMsg[receiveBytes] = '\0';
        printf("Response from server: %s\n\n", receiveMsg);
    }

    return 0;
}
