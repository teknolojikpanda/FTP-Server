//
// Created by dogaucak on 12/22/2020.
//

#ifndef FTP_SERVER_MAIN_H
#define FTP_SERVER_MAIN_H

#include "Script.h"
#include "INI_Parser.h"

//General Libs
#include <iostream>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <pwd.h>

#define BUFF_SIZE 2000

using namespace std;

int CreateSocket();
void CommandListener(int socket);
void Commands(int socket, char command[BUFF_SIZE]);
void ConnectionHandler(int socket_desc);
int CheckSUDO();
int CheckSysPassword(string username, string password);
int CheckSysUser(string name);
int CheckINIUser(string username);
int CheckINIPassword(string password);
string GetDefDir(string name);
string GetINIDir();
string GetCurrentUser();
int CheckDir(string dir, string username);
string exec(string cmd, string username);
void generate_config();

#endif
