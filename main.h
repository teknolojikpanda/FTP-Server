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

int CreateSocket();
void CommandListener(int socket);
void Commands(int socket, char command[BUFF_SIZE]);
void ConnectionHandler(int socket_desc);
int CheckPassword(string username, string password);
int CheckUser(string name);
string GetDefDir(string name);
int CheckDir(string dir, string username);
string exec(string cmd, string username);

#endif //UNTITLED1_MAIN_H
