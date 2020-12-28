//
// Created by dogaucak on 12/28/2020.
//

#include "main.h"

int CheckSUDO(){
    int user = getuid();
    if (user == 0){
        return 0;
    } else{
        return 1;
    }
}

string GetDefDir(string name){
    struct passwd *p;
    string result;
    p = getpwnam(name.c_str());
    result = p->pw_dir;
    return result;
}

string GetINIDir(){
    config cfg("config.ini");
    return cfg.get_value("config", "pwd");
}

int CheckDir(string dir, string username) {
    string command = "cd " + dir;
    string result;
    result = exec(command, username);
    if (strcmp(result.c_str(),"") == 0)
        return 0;
    else
        return 1;
}

void generate_config() {
    struct stat buffer;
    if (stat ("config.ini", &buffer) != 0){
        ofstream ostrm;
        ostrm.open("config.ini");
        if (ostrm) {
            ostrm << "[protocol]\n"
                     "version = 6\n"
                     "\n"
                     "[config]\n"
                     "username = test\n"
                     "password = test\n"
                     "pwd = /home/dogaucak/test_dir\n"
                     "port = 4467";
        }
    } else {
        cout << "Config file exists!" << endl;
    }
}

string exec(string cmd, string username) {
    string data;
    FILE * stream;
    string command;
    if (CheckSUDO() == 0){
        command = "runuser -l "+username+" -c '"+cmd+"'";
    } else {
        command = "runuser -l "+GetCurrentUser()+" -c '"+cmd+"'";
    }
    const int max_buffer = 256;
    char buffer[max_buffer];
    cmd.append(" 2>&1");

    stream = popen(cmd.c_str(), "r");

    if (stream) {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
        pclose(stream);
    }
    return data;
}