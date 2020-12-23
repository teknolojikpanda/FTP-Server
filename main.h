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
#include <dirent.h>

#define BUFF_SIZE 2000
#define SERVER_PORT 4467
#define ROOT_PATH "/home/dogaucak/test_dir/"

int CreateSocket();
void CommandListener(int socket);
void Commands(int socket, char command[BUFF_SIZE]);
void ConnectionHandler(int socket_desc);

#endif //UNTITLED1_MAIN_H
