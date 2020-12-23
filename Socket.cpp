//
// Created by dogaucak on 12/22/2020.
//

#include "main.h"

void PANIC(char* msg);
#define PANIC(msg)  { perror(msg); exit(-1); }

int CreateSocket(){
    int sd,optval;
    int *new_sock;
    int i = 0;
    struct sockaddr_in addr;

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

    while (1)
    {
        int client, addr_size = sizeof(addr);
        pthread_t connection_handler;

        client = accept(sd, (struct sockaddr*)&addr, reinterpret_cast<socklen_t *>(&addr_size));
        *new_sock = client;
        printf("Connected: %s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

        if ( pthread_create(&connection_handler, NULL, ConnectionHandler, (void*) new_sock) != 0 )
            perror("Thread creation");
        else
            pthread_detach(connection_handler);
        i++;
    }
}

void *ConnectionHandler(void *socket_desc)
{
    int	socket = *(int*)socket_desc;
    int thread_stat;
    pthread_t command_listener;
    char	server_response[BUFF_SIZE];

    sprintf(server_response,"220 Welcome to FTP Server\r\n");
    send(socket, server_response, strlen(server_response), 0);

    // Command Listener Thread Initialization
    thread_stat = pthread_create(&command_listener, NULL, reinterpret_cast<void *(*)(void *)>(&CommandListener),
                                 reinterpret_cast<void *>(socket));
    if(thread_stat==0){
        printf("Command Listener Working.\n");
    }
    else{
        printf("Command Listener Not Working.\n");
    }

    return socket_desc;
}