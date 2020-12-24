//
// Created by dogaucak on 12/22/2020.
//

#ifndef UNTITLED1_MAIN_H
#define UNTITLED1_MAIN_H

#include "Script.h"

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
#include <pwd.h>
#include <iostream>

using namespace std;

#define BUFF_SIZE 2000
#define SERVER_PORT 4467
#define ROOT_PATH "/home/dogaucak/test_dir/"

int CreateSocket();
void CommandListener(int socket);
void Commands(int socket, char command[BUFF_SIZE]);
void ConnectionHandler(int socket_desc);
int CheckPassword(char username[2000], char password[2000]);
int CheckUser(char *name);

#endif //UNTITLED1_MAIN_H
