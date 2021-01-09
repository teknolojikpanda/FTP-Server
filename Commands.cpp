//
// Created by dogaucak on 12/22/2020.
//

#include "main.h"

bool after_login = false;
bool user_required = false;
bool LOGIN_status = false;
string get_val,put_val,put_filename,username,password,dir,del_filename;

void CommandListener(int socket)
{
    int rcnt;
    char recv_buffer[BUFF_SIZE];
    char new_recv_buffer[BUFF_SIZE];
    do
    {
        memset(recv_buffer, 0, sizeof(recv_buffer));
        memset(new_recv_buffer, 0, sizeof(recv_buffer));

        rcnt = recv(socket, recv_buffer, BUFF_SIZE, 0);
        if (rcnt > 0) 
        {
            strncat(new_recv_buffer,recv_buffer,strlen(recv_buffer)-1);
            cout << "Full Command : " << new_recv_buffer << endl;
            Commands(socket, new_recv_buffer);
        }
        else if (rcnt == 0)
            cout << "Connection closed by client" << endl;
        else  
        {
            cout << "Connection Terminated!" << endl;
            break;
        }
    } while (rcnt > 0);
    close(socket);
}

void Commands(int socket, char command[BUFF_SIZE])
{
    char* token = strtok(command, " ");
    char send_buffer[BUFF_SIZE];
    int USER_status;
    int PASS_status;

    
    if (strcmp(token,"USER") == 0)
    {
        token = strtok(NULL, " ");
        username = token;
        if (LOGIN_status == false)
        {
            if (CheckSUDO() == 0)
            {
                USER_status = CheckSysUser(username);
            } 
            else 
            {
                USER_status = CheckINIUser(username);
            }
            if (username.c_str() != NULL && USER_status == 0)
            {
                //True
                sprintf(send_buffer,"331 Password required \r\n");
                send(socket, send_buffer, strlen(send_buffer), 0);
                user_required = true;
            } 
            else
            {
                //False
                sprintf(send_buffer,"430 Username incorrect \r\n");
                send(socket, send_buffer, strlen(send_buffer), 0);
            }
        }
        else if (LOGIN_status == true)
        {
            sprintf(send_buffer,"530 You are already logged in.\r\n");
            send(socket, send_buffer, strlen(send_buffer), 0);
        }
         
    } 
    else if (strcmp(token,"PASS") == 0) 
    {
        if (LOGIN_status == false)
        {
            if (user_required == true)
            {
                token = strtok(NULL, " ");
                password = token;
                if (CheckSUDO() == 0)
                {
                    PASS_status = CheckSysPassword(username,password);
                } 
                else 
                {
                    PASS_status = CheckINIPassword(password);
                }
                if (password.c_str() != NULL && PASS_status == 0)
                {
                    //True
                    after_login = true;
                    LOGIN_status = true;
                    if (CheckSUDO() == 0)
                    {
                        dir = GetDefDir(username);
                    } 
                    else 
                    {
                        dir = GetINIDir();
                    }
                    sprintf(send_buffer,"230 Public login successful \r\n");
                    send(socket, send_buffer, strlen(send_buffer), 0);
                } 
                else
                {
                    //False
                    sprintf(send_buffer,"430 Password incorrect.\r\n");
                    send(socket, send_buffer, strlen(send_buffer), 0);
                }
            }
            else if (user_required == false)
            {
                sprintf(send_buffer,"332 Need account for login.\r\n");
                send(socket, send_buffer, strlen(send_buffer), 0);
            }
        }
        else if (LOGIN_status == true)
        {
            sprintf(send_buffer,"530 You are already logged in.\r\n");
            send(socket, send_buffer, strlen(send_buffer), 0);
        }
    } 
    else if (strcmp(token,"LIST") == 0) 
    {
        if (after_login == true)
        {
            sprintf(send_buffer,"257 \"%s\" is your current directory.\r\n", dir.c_str());
            send(socket, send_buffer, strlen(send_buffer), 0);

            string command = "cd "+dir+" && du -sh *";
            string result = exec(command,username);
            send(socket, result.c_str(), strlen(result.c_str()), 0);
            send(socket, ".\n", strlen(".\n"), 0);
        } 
        else if (after_login == false)
        {
            sprintf(send_buffer,"530 Not logged in.\r\n");
            send(socket, send_buffer, strlen(send_buffer), 0);
        }
    } 
    else if (strcmp(token,"CWD") == 0) 
    {
        string tmp = "";
        if (after_login == true)
        {
            token = strtok(NULL, " ");
            tmp = token;
            int result = CheckDir(tmp,username);
            if (tmp.c_str() != NULL && result == 0)
            {
                dir = tmp;
                sprintf(send_buffer,"257 \"%s\" is your current directory.\r\n", dir.c_str());
                send(socket, send_buffer, strlen(send_buffer), 0);
            } 
            else 
            {
                sprintf(send_buffer,"550 Directory not found.\r\n");
                send(socket, send_buffer, strlen(send_buffer), 0);
            }
        } 
        else if (after_login == false)
        {
            sprintf(send_buffer,"530 Not logged in.\r\n");
            send(socket, send_buffer, strlen(send_buffer), 0);
        }
    }  
    else if (strcmp(token,"GET") == 0) 
    {
        if (after_login == true)
        {
            token = strtok(NULL, " ");
            get_val = token;
            if (get_val.c_str() != NULL)
            {
                string command = "cat " + dir + "/" + get_val;
                string result = exec(command, username);
                if (result == "cat: "+ dir + "/" + get_val +": No such file or directory\n")
                {
                    send(socket, "550 ERROR File not found.\r\n", strlen("550 ERROR File not found.\r\n"), 0);
                } 
                else if (result == "cat: "+ dir + "/" + get_val +": Permission denied\n")
                {
                    send(socket, "550 ERROR No access.\r\n", strlen("550 ERROR No access.\r\n"), 0);
                } 
                else 
                {
                    send(socket, result.c_str(), strlen(result.c_str()), 0);
                    send(socket, "\n.\n", strlen("\n.\n"), 0);
                }
            } 
            else 
            {
                send(socket, "501 Syntax error in parameters or arguments.\r\n", strlen("501 Syntax error in parameters or arguments.\r\n"), 0);
            }
        } 
        else if (after_login == false)
        {
            sprintf(send_buffer,"530 Not logged in.\r\n");
            send(socket, send_buffer, strlen(send_buffer), 0);
        }
    } 
    else if (strcmp(token,"PUT") == 0) 
    {
        if (after_login == true)
        {
            token = strtok(NULL, " ");
            put_filename = token;
            token = strtok(NULL, " ");
            put_val = token;

            if (put_filename.c_str() != NULL && put_val.c_str() != NULL)
            {
                string command = "echo \""+put_val+"\" > "+dir+"/"+put_filename;
                string result = exec(command,username);

                if (result == "sh: 1: cannot create "+dir + "/" + put_filename+": Permission denied")
                {
                    send(socket, "550 ERROR No access.\r\n", strlen("550 ERROR No access.\r\n"), 0);
                } 
                else 
                {
                    send(socket, "250 Requested file action okay, completed.\r\n", strlen("250 Requested file action okay, completed.\r\n"), 0);
                }
            } 
            else 
            {
                send(socket, "501 Syntax error in parameters or arguments.\r\n", strlen("501 Syntax error in parameters or arguments.\r\n"), 0);
            }
        } 
        else if (after_login == false)
        {
            sprintf(send_buffer,"530 Not logged in.\r\n");
            send(socket, send_buffer, strlen(send_buffer), 0);
        }
    } 
    else if (strcmp(token,"DEL") == 0) 
    {
        if (after_login == true)
        {
            token = strtok(NULL, " ");
            del_filename = token;
            if (del_filename.c_str() != NULL)
            {
                string command = "rm "+dir+"/"+del_filename;
                string result = exec(command,username);
                if (result == "rm: cannot remove '"+dir+"/"+del_filename+"': No such file or directory\n")
                {
                    send(socket, "550 ERROR File not found.\r\n", strlen("550 ERROR File not found.\r\n"), 0);
                } 
                else if (result == "rm: cannot remove '"+dir+"/"+del_filename+"': Permission denied\n")
                {
                    send(socket, "550 ERROR No access.\r\n", strlen("550 ERROR No access.\r\n"), 0);
                } 
                else if (result == "rm: cannot remove '"+dir+"/"+del_filename+"': Is a directory\n")
                {
                    string command = "rm -r "+dir+"/"+del_filename;
                    exec(command,username);
                    send(socket, "250 Requested file action okay, completed.\r\n", strlen("250 Requested file action okay, completed.\r\n"), 0);
                } 
                else 
                {
                    send(socket, "250 Requested file action okay, completed.\r\n", strlen("250 Requested file action okay, completed.\r\n"), 0);
                }
            } 
            else 
            {
                send(socket, "501 Syntax error in parameters or arguments.\r\n", strlen("501 Syntax error in parameters or arguments.\r\n"), 0);
            }
        } 
        else if (after_login == false)
        {
            sprintf(send_buffer,"530 Not logged in.\r\n");
            send(socket, send_buffer, strlen(send_buffer), 0);
        }
    } 
    else if (strcmp(token,"QUIT") == 0)
    {
        after_login = false;
        send(socket,"Goodbye!",strlen("Goodbye!"),0);
        close(socket);
    } 
    else 
    {
        sprintf(send_buffer,"502 Invalid command.\r\n");
        send(socket, send_buffer, strlen(send_buffer), 0);
    }
}