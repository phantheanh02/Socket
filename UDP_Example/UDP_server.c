#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFF_SIZE 128
#define SERV_PORT 5050

int main(int argc, char const *argv[])
{

    int sockfd, receiveBytes, sendBytes;
    socklen_t len;
    char buff[BUFF_SIZE + 1];
    struct sockaddr_in serverAddr, clientAddr;

    // Step 1: Constructor socket
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0)
    {
        perror("Error create socket: ");
        return 0;
    }


    // Step 2: Bind address to socket
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(SERV_PORT);
    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)))
    {
        perror("Error bind addres to socket: ");
        return 0;
    }

    printf("Server started. \n");

    //Step 3: Communicate with client
    while (1)
    {
        memset(buff,'\0',(strlen(buff)+1));
        printf("Waiting request from client...\n");
        len = sizeof(clientAddr);
        receiveBytes = recvfrom(sockfd, buff, BUFF_SIZE, 0, (struct sockaddr *)&clientAddr, &len);
        if (receiveBytes < 0)
        {
            perror("Error recevie from client: ");
            return 0;
        }
        buff[receiveBytes] = '\0';
        printf("New message: ");
        printf("[%s:%d]: %s\n\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buff);
        sendBytes = sendto(sockfd, buff, BUFF_SIZE, 0, (struct sockaddr *)&clientAddr, len);
        if (sendBytes < 0)
        {
            perror("Error send to client: ");
            return 0;
        }
    }

    return 0;
}
