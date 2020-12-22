//
// Created by dogaucak on 12/22/2020.
//

#ifndef UNTITLED1_MAIN_H
#define UNTITLED1_MAIN_H

#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <pthread.h>
#include <arpa/inet.h>

#define BUFSIZ 2000
#define SERVER_PORT 4467

int CreateSocket();
void CommandListener(int socket);
void Commands(int socket, char command[BUFSIZ]);
void* ConnectionHandler(void* socket_desc);

#endif //UNTITLED1_MAIN_H
