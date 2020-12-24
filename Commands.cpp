//
// Created by dogaucak on 12/22/2020.
//

#include "main.h"

bool after_login = false;
bool user_required = false;

void CommandListener(int socket){
    int rcnt;
    char recv_buffer[BUFF_SIZE];
    char new_recv_buffer[BUFF_SIZE];
    do{
        memset(recv_buffer, 0, sizeof(recv_buffer));
        memset(new_recv_buffer, 0, sizeof(recv_buffer));

        rcnt = recv(socket, recv_buffer, BUFF_SIZE, 0);
        if (rcnt > 0) {
            strncat(new_recv_buffer,recv_buffer,strlen(recv_buffer)-1);
            printf("Full Command : %s\n", new_recv_buffer);
            Commands(socket, new_recv_buffer);
        }
        else if (rcnt == 0)
            printf("Connection closed by client\n");
        else  {
            printf("Connection Terminated!\n");
            break;
        }
    } while (rcnt > 0);
    close(socket);
}

void Commands(int socket, char command[BUFF_SIZE]){

    char* token = strtok(command, " ");
    char *username;
    char *password;
    char send_buffer[BUFF_SIZE];

    if (strcmp(token,"USER") == 0){
        token = strtok(nullptr, " ");
        username = token;
        if (username != nullptr && CheckUser(username) == 0){
            //True
            sprintf(send_buffer,"331 Password required \r\n");
            send(socket, send_buffer, strlen(send_buffer), 0);
            user_required = true;
        } else{
            //False
            sprintf(send_buffer,"430 Username incorrect \r\n");
            send(socket, send_buffer, strlen(send_buffer), 0);
        }
    } else if (strcmp(token,"PASS") == 0) {
        if (user_required == true){
            token = strtok(nullptr, " ");
            password = token;
            if (password != nullptr && CheckPassword("dogaucak",password) == 0){
                //True
                after_login = true;
                sprintf(send_buffer,"230 Public login successful \r\n");
                send(socket, send_buffer, strlen(send_buffer), 0);
            } else{
                //False
                sprintf(send_buffer,"430 Password incorrect.\r\n");
                send(socket, send_buffer, strlen(send_buffer), 0);
            }
        } else if (user_required == false){
            sprintf(send_buffer,"332 Need account for login.\r\n");
            send(socket, send_buffer, strlen(send_buffer), 0);
        }
    } else if (strcmp(token,"LIST") == 0) {
        if (after_login == true){
            sprintf(send_buffer,"257 \"/home/dogaucak/test_dir/\" is your current directory.\r\n");
            send(socket, send_buffer, strlen(send_buffer), 0);

            DIR *d;
            struct dirent *dir;
            struct stat buf;
            char root_path[BUFF_SIZE] = ROOT_PATH;
            char full_path[BUFF_SIZE];
            d = opendir(root_path);
            if (d) {
                while ((dir = readdir(d)) != nullptr && strcmp(dir->d_name,"..") != 0) {
                    strcpy(full_path,root_path);
                    strcat(full_path,dir->d_name);
                    sprintf(send_buffer,"%s", full_path);
                    send(socket, send_buffer, strlen(send_buffer), 0);
                    stat(full_path,&buf);
                    sprintf(send_buffer," %lo\r\n", buf.st_size);
                    send(socket, send_buffer, strlen(send_buffer), 0);
                }
                send(socket, ".\r\n", strlen(".\r\n"), 0);
                closedir(d);
            }
        } else if (after_login == false){
            sprintf(send_buffer,"530 Not logged in.\r\n");
            send(socket, send_buffer, strlen(send_buffer), 0);
        }
    } else if (strcmp(token,"CWD") == 0) {
        if (after_login == true){

        } else if (after_login == false){
            sprintf(send_buffer,"530 Not logged in.\r\n");
            send(socket, send_buffer, strlen(send_buffer), 0);
        }
    } else if (strcmp(token,"GET") == 0) {
        if (after_login == true){

        } else if (after_login == false){
            sprintf(send_buffer,"530 Not logged in.\r\n");
            send(socket, send_buffer, strlen(send_buffer), 0);
        }
    } else if (strcmp(token,"PUT") == 0) {
        if (after_login == true){

        } else if (after_login == false){
            sprintf(send_buffer,"530 Not logged in.\r\n");
            send(socket, send_buffer, strlen(send_buffer), 0);
        }
    } else if (strcmp(token,"DEL") == 0) {
        if (after_login == true){

        } else if (after_login == false){
            sprintf(send_buffer,"530 Not logged in.\r\n");
            send(socket, send_buffer, strlen(send_buffer), 0);
        }
    } else if (strcmp(token,"QUIT") == 0) {
        after_login = false;
        send(socket,"Goodbye!",strlen("Goodbye!"),0);
        close(socket);
    } else {
        sprintf(send_buffer,"502 Invalid command.\r\n");
        send(socket, send_buffer, strlen(send_buffer), 0);
    }
}
