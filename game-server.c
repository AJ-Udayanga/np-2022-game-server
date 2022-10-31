#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <math.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/select.h>

#define TRUE 1
#define FALSE 0

int main()
{
    // Variable declaration
    const char *port = "55555";    // client and server port
    const int client_ip_size = 32; // client ip size
    char client_name[client_ip_size];
    char buffer[BUFSIZ], sendstr[BUFSIZ];
    const int queue_size = 10;                   // max number of connections
    char connection[queue_size][client_ip_size]; // store ipv4 address
    socklen_t address_len = sizeof(struct sockaddr);
    struct addrinfo hints, *server;
    struct sockaddr address;
    int r, max_connect, fd, x, done;
    fd_set recv_fd, tmp_fd;
    int server_fd, client_fd;

    int number, guess, numberofguess;

    char *message;

    // server implimention
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;       // family
    hints.ai_socktype = SOCK_STREAM; // socket type
    hints.ai_flags = AI_PASSIVE;     // accespting all connections
    r = getaddrinfo(0, port, &hints, &server);
    if (r != 0)
    {
        perror("failed");
        exit(1);
    }

    /* create a socket */
    server_fd = socket(server->ai_family, server->ai_socktype, server->ai_protocol);
    if (server_fd == -1)
    {
        perror("failed");
        exit(1);
    }

    /* bind to a port */
    r = bind(server_fd, server->ai_addr, server->ai_addrlen);
    if (r == -1)
    {
        perror("failed");
        exit(1);
    }

    /* listen for a connection*/
    puts("Chat Server is listening...");
    r = listen(server_fd, queue_size);
    if (r == -1)
    {
        perror("failed");
        exit(1);
    }

    // controlling multiple connections
    max_connect = queue_size;
    FD_ZERO(&recv_fd);
    FD_SET(server_fd, &recv_fd); // server file descriptor

    done = FALSE;
    while (!done)
    {
        // assigning original file descriptor to tmp descriptor
        tmp_fd = recv_fd;

        // scanning connections
        r = select(max_connect + 1, &tmp_fd, NULL, NULL, 0);
        if (r == -1)
        {
            perror("failed");
            exit(1);
        }

        // checking active connections
        for (fd = 1; fd <= max_connect; fd++)
        {

            if (FD_ISSET(fd, &tmp_fd))
            {
                // check for new connections
                if (fd == server_fd)
                {
                    // connecting new client
                    client_fd = accept(
                        server_fd,
                        (struct sockaddr *)&address,
                        &address_len);
                    if (client_fd == -1)
                    {
                        perror("failed");
                        exit(1);
                    }
                    // establishing connection
                    r = getnameinfo(
                        (struct sockaddr *)&address,
                        address_len,
                        client_name,
                        client_ip_size,
                        0,
                        0,
                        NI_NUMERICHOST);

                    strcpy(connection[client_fd], client_name);

                    FD_SET(client_fd, &recv_fd);

                    strcpy(buffer, "SERVER> Welcome ");
                    strcat(buffer, connection[client_fd]);
                    strcat(buffer, " to the 'Guess the number' game server\n");
                    strcat(buffer, "SERVER> Type 'play' to play the game; 'close' to disconnect\n");
                    send(client_fd, buffer, strlen(buffer), 0);

                    strcpy(buffer, "SERVER> ");
                    strcat(buffer, connection[client_fd]);
                    strcat(buffer, " has joined the server\n");

                    for (x = server_fd + 1; x < max_connect; x++)
                    {
                        if (FD_ISSET(x, &recv_fd))
                            send(x, buffer, strlen(buffer), 0);
                    }

                    printf("%s", buffer);
                }
                else
                {

                    r = recv(fd, buffer, BUFSIZ, 0);

                    if (r < 1)
                    {
                        FD_CLR(fd, &recv_fd);
                        close(fd);

                        strcpy(buffer, "SERVER> ");
                        strcat(buffer, connection[fd]);
                        strcat(buffer, " disconnected\n");

                        for (x = server_fd + 1; x < max_connect; x++)
                        {
                            if (FD_ISSET(x, &recv_fd))
                            {
                                send(x, buffer, strlen(buffer), 0);
                            }
                        }

                        printf("%s", buffer);
                    }

                    else
                    {
                        buffer[r] = '\0';

                        if (strcmp(buf fer, "play\n") == 0)
                        {

                            int userid;
                            char nickname[BUFSIZ];
                            userid = rand() % 9999; // genrating user id
                            FILE *filePointer = fopen("playerDetails.dat", "a");
                            strcpy(sendstr, "Plese enter your username:\n");

                            for (x = server_fd + 1; x < max_connect; x++)
                            {
                                /* check for an active file descriptor */
                                if (FD_ISSET(x, &recv_fd))
                                {
                                    /* send the built string */
                                    send(x, sendstr, strlen(sendstr), 0);
                                }
                            }

                            buffer[r] = '\0';
                            r = recv(fd, buffer, BUFSIZ, 0);
                            strcpy(nickname, buffer);

                            int N = 100;
                            int score = 1000;

                            // string message;
                            srand(time(NULL));

                            // Generate a random number
                            number = rand() % N;
                            numberofguess = 1;

                            strcpy(sendstr, "Guess a number between 1 and 100\n");

                            for (x = server_fd + 1; x < max_connect; x++)
                            {
                                /* check for an active file descriptor */
                                if (FD_ISSET(x, &recv_fd))
                                {
                                    /* send the built string */
                                    send(x, sendstr, strlen(sendstr), 0);
                                }
                            }
                            // write(client_fd, message, strlen(message));

                            do
                            {
                                if (numberofguess > 9)
                                {
                                    strcpy(sendstr, "You Loose!\n");
                                    for (x = server_fd + 1; x < max_connect; x++)
                                    {

                                        if (FD_ISSET(x, &recv_fd))
                                        {

                                            send(x, sendstr, strlen(sendstr), 0);
                                        }
                                    }
                                    break;
                                }

                                // Input by user

                                buffer[r] = '\0';
                                r = recv(fd, buffer, BUFSIZ, 0);

                                guess = atoi(buffer);

                                // When user guesses lower
                                // than actual number
                                if (guess > number)
                                {

                                    strcpy(sendstr, "Lower number please!\n");

                                    numberofguess++;
                                    score = score - (numberofguess * 49);
                                }

                                else if (number > guess)

                                {
                                    strcpy(sendstr, "Higher number  please!\n");

                                    numberofguess++;
                                    score = score - (numberofguess * 23);
                                }

                                // Printing number of times
                                // user has taken to guess
                                // the number
                                else
                                {

                                    strcpy(sendstr, "You Won!\n");
                                }

                                for (x = server_fd + 1; x < max_connect; x++)
                                {

                                    if (FD_ISSET(x, &recv_fd))
                                    {

                                        send(x, sendstr, strlen(sendstr), 0);
                                    }
                                }

                            } while (guess != number);

                            fprintf(filePointer, "%d %d %s", userid, score, nickname); // printing data into files.
                            fclose(filePointer);

                            strcpy(sendstr, "your score: ");

                            for (x = server_fd + 1; x < max_connect; x++)
                            {

                                if (FD_ISSET(x, &recv_fd))
                                {

                                    send(x, sendstr, strlen(sendstr), 0);
                                }
                            }
                        }

                        else
                        {

                            strcpy(sendstr, connection[fd]);
                            strcat(sendstr, "> ");
                            strcat(sendstr, buffer);

                            for (x = server_fd + 1; x < max_connect; x++)
                            {

                                if (FD_ISSET(x, &recv_fd))
                                {

                                    send(x, sendstr, strlen(sendstr), 0);
                                }
                            }

                            printf("%s", sendstr);
                        }
                    }
                }
            }
        }
    }

    close(server_fd);
    freeaddrinfo(server);
    return (0);
}