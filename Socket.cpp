//
// Created by dogaucak on 12/22/2020.
//

#include "main.h"

void PANIC(char* msg);
#define PANIC(msg)  { perror(msg); exit(-1); }

int CreateSocket(){
    int sd,optval,client,n;

    struct sockaddr_in addr;
    pid_t pid;

    if ( (sd = socket(PF_INET, SOCK_STREAM, 0)) < 0 )
        PANIC("Socket");

    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    // set SO_REUSEADDR on a socket to true (1):
    optval = 1;
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

    if ( bind(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
        PANIC("Bind");
    if ( listen(sd, SOMAXCONN) != 0 )
        PANIC("Listen");

    printf("System ready on port %d\n",ntohs(addr.sin_port));

    while(1) {  // main accept() loop
        int addr_size = sizeof(addr);
        if ((client = accept(sd, (struct sockaddr*)&addr, reinterpret_cast<socklen_t *>(&addr_size))) == -1) {
            perror("Accept Problem!");
            continue;
        }

        printf("Server: got connection from %s\n", inet_ntoa(addr.sin_addr));

        /* If fork create Child, take control over child and close on server side */
        if ((pid=fork()) == 0) {
            close(sd);
            ConnectionHandler(client);
            close(client);
            exit(0);
        }
    }
    printf("USER LIMIT REACHED!");
}

void ConnectionHandler(int socket_desc)
{
    int	socket = socket_desc;

    char server_response[BUFF_SIZE];
    pid_t pid;

    sprintf(server_response,"220 Welcome to FTP Server\r\n");
    send(socket, server_response, strlen(server_response), 0);

    // Command Listener Thread Initialization
    if ((pid = fork()) == 0){
        CommandListener(socket);
    }
}