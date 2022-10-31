#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/select.h>

#define TRUE 1
#define FALSE 0

int main(int argc, char *argv[])
{
    const char *port = "55555";
    char *host;
    struct addrinfo hints, *server;
    int r, sockfd, done;
    char buffer[BUFSIZ];
    fd_set read_fd;

    if (argc < 2)
    {
        fprintf(stderr, "command: client hostname\n");
        exit(1);
    }
    host = argv[1];

    printf("Looking for chat server on %s...", host);
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    r = getaddrinfo(host, port, &hints, &server);
    if (r != 0)
    {
        perror("failed");
        exit(1);
    }
    puts("found");

    sockfd = socket(server->ai_family, server->ai_socktype, server->ai_protocol);
    if (sockfd == -1)
    {
        perror("failed");
        exit(1);
    }

    /* connect to the socket */
    r = connect(sockfd, server->ai_addr, server->ai_addrlen);
    freeaddrinfo(server);
    if (r == -1)
    {
        perror("failed");
        exit(1);
    }

    done = FALSE;
    while (!done)
    {

        FD_ZERO(&read_fd);
        FD_SET(sockfd, &read_fd);
        FD_SET(0, &read_fd);
        r = select(sockfd + 1, &read_fd, NULL, NULL, 0);
        if (r == -1)
        {
            perror("failed");
            exit(1);
        }

        if (FD_ISSET(sockfd, &read_fd))
        {
            r = recv(sockfd, buffer, BUFSIZ, 0);

            if (r < 1)
            {
                puts("Connection closed by peer");
                break;
            }

            buffer[r] = '\0';
            printf("%s", buffer);
        }

        /* local input */
        if (FD_ISSET(0, &read_fd))
        {

            if (fgets(buffer, BUFSIZ, stdin) == NULL)
            {
                putchar('\n');
            }

            else if (strcmp(buffer, "close\n") == 0)
            {
                done = TRUE;
            }

            else
            {
                send(sockfd, buffer, strlen(buffer), 0);
            }
        }
    }

    printf("Disconnected\nBye!\n");
    close(sockfd);

    return (0);
}
