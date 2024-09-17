#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>



// Define some attributes
#define BUFF_SIZE 128
#define SERV_PORT 5050
#define MAX_CLIENT 10

// Function to handle SIGCHLD
#ifdef FORK
#include <sys/signal.h>
#include <sys/wait.h>

void signalHandler(int signo)
{
    pid_t pid;
    int stat;
    printf("Signo = %d\n", signo);
    pid = wait(&stat);
    printf("Child %d terminated.\n", pid);
    return;
}
#endif

#ifdef MULTI_THREAD
#include <pthread.h>
#include <sys/resource.h>

void *ThreadHandler(void *arg)
{
    printf("New thread created: ID %ld\n", pthread_self());
    int client = *(int *)arg;
    char buff[BUFF_SIZE + 1];
    pthread_mutex_t * mptr;

    struct sockaddr_in clientAddr;
    socklen_t peer_addr_len = sizeof(clientAddr);
    getpeername(client, (struct sockaddr*)&clientAddr, &peer_addr_len);

    while (1)
    {
        int len = recv(client, buff, sizeof(buff), 0);
        if (len <= 0)
        {
            //perror("Error receive to client: ");
            break;
        }

        buff[len] = 0;
        printf("New message from [%s:%d]: %s\n\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buff);

        len = send(client, buff, BUFF_SIZE, 0);
        if (len < 0)
        {
            perror("Error send to client: ");
        }
    }
    close(client);
    printf("Thread finished: ID %ld\n\n", pthread_self());
}
#endif

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

#ifdef FORK
    // wait for a child process to stop
    pid_t pid;
    signal(SIGCHLD, signalHandler);
#endif
#ifdef MULTI_THREAD
    pthread_t tid;
#endif

    // Step 4: Communicate with client
    socklen_t len = sizeof(clientAddr);
    while (1)
    {
        memset(buff, '\0', (strlen(buff) + 1));
        printf("\nWaiting request from client...\n");

        // Waiting for connect request from client
        connfd = accept(listenfd, (struct sockaddr *)&clientAddr, &len);
        // Handle EINTR errors
        if (connfd < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            else
            {
                perror("Error: ");
            }
        }

#ifdef FORK
        if (pid = fork() == 0)
        {
            // In child process
            close(listenfd); // It's not need

            while (1)
            {
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
            close(connfd);
            exit(0); // child terminates
        }
#endif

#ifdef MULTI_THREAD
        int ret = pthread_create(&tid, NULL, ThreadHandler, (void *)&connfd);
        if (ret != 0)
        {
            perror("Error create thread: ");
        }
        
        pthread_detach(pthread_self());
        sched_yield();
#endif
    }
    printf("Finish\n");
    close(listenfd);
    return 0;
}
