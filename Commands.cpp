//
// Created by dogaucak on 12/22/2020.
//

#include "main.h"

bool after_login = false;

void CommandListener(int socket){
    int rcnt;
    char recv_buffer[BUFSIZ];
    char new_recv_buffer[BUFSIZ];
    do{
        rcnt = recv(socket, recv_buffer, BUFSIZ, 0);
        if (rcnt > 0) {
            strncat(new_recv_buffer,recv_buffer,strlen(recv_buffer)-2);
            printf("Full Command : %s\n", new_recv_buffer);
            Commands(socket, new_recv_buffer);

            memset(recv_buffer, 0, sizeof(recv_buffer));
            memset(new_recv_buffer, 0, sizeof(recv_buffer));
        }
    } while (rcnt > 0);
    close(socket);
}

void Commands(int socket, char command[BUFSIZ]){

    char* token = strtok(command, " ");
    char *username;
    char *password;
    char send_buffer[BUFSIZ];

    if (strcmp(token,"USER") == 0){
        token = strtok(nullptr, " ");
        username = token;
        if (username != nullptr && strcmp(username,"test") == 0){
            //True
            sprintf(send_buffer,"331 Password required \r\n");
            send(socket, send_buffer, strlen(send_buffer), 0);
        } else{
            //False
            sprintf(send_buffer,"430 User or password incorrect \r\n");
            send(socket, send_buffer, strlen(send_buffer), 0);
        }
    } else if (strcmp(token,"PASS") == 0) {
            token = strtok(nullptr, " ");
            password = token;
            if (password != nullptr && strcmp(password,"test") == 0){
                //True
                after_login = true;
                sprintf(send_buffer,"230 Public login successful \r\n");
                send(socket, send_buffer, strlen(send_buffer), 0);
            } else{
                //False
                sprintf(send_buffer,"430 User or password incorrect.\r\n");
                send(socket, send_buffer, strlen(send_buffer), 0);
        }
    } else if (strcmp(token,"PWD") == 0) {
        if (after_login == true){
            printf("%s","OK");
        } else if (after_login == false){
            printf("%s","FAIL");
        }
    } else if (strcmp(token,"GET") == 0) {
        if (after_login == true){
            printf("%s","OK");
        } else if (after_login == false){
            printf("%s","FAIL");
        }
    } else if (strcmp(token,"PUT") == 0) {
        if (after_login == true){
            printf("%s","OK");
        } else if (after_login == false){
            printf("%s","FAIL");
        }
    } else if (strcmp(token,"DELE") == 0) {
        if (after_login == true){
            printf("%s","OK");
        } else if (after_login == false){
            printf("%s","FAIL");
        }
    } else if (strcmp(token,"QUIT") == 0) {
        send(socket,"Goodbye!",strlen("Goodbye!"),0);
        close(socket);
    } else {
        sprintf(send_buffer,"502 Invalid command.\r\n");
        send(socket, send_buffer, strlen(send_buffer), 0);
    }
}
