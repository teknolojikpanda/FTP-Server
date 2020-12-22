//
// Created by dogaucak on 12/22/2020.
//

#include "main.h"

void CommandListener(int socket){
    int rcnt;
    char recv_buffer[BUFSIZ];
    char sub_recv_buffer[BUFSIZ];
    do{
        rcnt = recv(socket, recv_buffer, BUFSIZ, 0);
        if (rcnt > 0) {
            for (int i = 0; i <= strlen(recv_buffer)-2; i++) {
                sub_recv_buffer[i] = recv_buffer[i];
            }
            Commands(socket, sub_recv_buffer);
            memset(recv_buffer, 0, sizeof(recv_buffer));
        }
    } while (rcnt > 0);
    close(socket);
}

void Commands(int socket, char command[BUFSIZ]){
    char* token = strtok(command, " ");
    char *username = NULL;
    char *password = NULL;
    char send_buffer[BUFSIZ];

    printf("Command : %s\n",token);

    if (strcmp(token,"USER") == 0){
        token = strtok(NULL, " ");
        username = token;
        if (strcmp(username,"test") == 0){
            //True
            token = strtok(NULL, " ");
            password = token;
            if (strcmp(password,"test") == 0){
                //True
                sprintf(send_buffer,"230 Public login successful \r\n");
                send(socket, send_buffer, strlen(send_buffer), 0);
            } else{
                //False
                sprintf(send_buffer,"\n400 User or password incorrect.\r\n\n");
                send(socket, send_buffer, strlen(send_buffer), 0);
            }
        } else{
            //False
            sprintf(send_buffer,"400 User or password incorrect \r\n");
            send(socket, send_buffer, strlen(send_buffer), 0);
        }
    } else if (strcmp(token,"LIST") == 0) {

    } else if (strcmp(token,"GET") == 0) {

    } else if (strcmp(token,"PUT") == 0) {

    } else if (strcmp(token,"DEL") == 0) {

    } else if (strcmp(token,"QUIT") == 0) {
        send(socket,"Goodbye!",strlen("Goodbye!"),0);
        close(socket);
    } else {
        printf("Invalid Command!\n");
    }
}
