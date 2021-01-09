//
// Created by dogaucak on 12/22/2020.
//

#include "main.h"

void PANIC(char* msg);
#define PANIC(msg)  { perror(msg); exit(-1); }

int CreateSocket()
{
    int sd,optval,client;

    config cfg("config.ini");
    stringstream intValue(cfg.get_value("config", "port"));
    int PORT = 0;
    intValue >> PORT;

    struct sockaddr_in addr;
    pid_t pid;

    if ( (sd = socket(PF_INET, SOCK_STREAM, 0)) < 0 )
        PANIC("Socket");

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    optval = 1;
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

    if ( bind(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
        PANIC("Bind");
    if ( listen(sd, SOMAXCONN) != 0 )
        PANIC("Listen");

    cout << "System ready on port " << ntohs(addr.sin_port) << endl;

    while(1) 
    {
        int addr_size = sizeof(addr);
        if ((client = accept(sd, (struct sockaddr*)&addr, reinterpret_cast<socklen_t *>(&addr_size))) == -1) 
        {
            perror("Accept Problem!");
            continue;
        }

        cout << "Server: got connection from " << inet_ntoa(addr.sin_addr) << endl;

        if ((pid=fork()) == 0) 
        {
            close(sd);
            ConnectionHandler(client);
            close(client);
            exit(0);
        }
    }
}

void ConnectionHandler(int socket_desc)
{
    int	socket = socket_desc;

    char server_response[BUFF_SIZE];
    pid_t pid;

    sprintf(server_response,"220 Welcome to FTP Server\r\n");
    send(socket, server_response, strlen(server_response), 0);

    // Command Listener Thread Initialization
    if ((pid = fork()) == 0)
    {
        CommandListener(socket);
    }
}