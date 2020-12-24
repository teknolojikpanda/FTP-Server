//
// Created by dogaucak on 12/24/2020.
//

#include "main.h"

int CheckUser(string name) {
    struct passwd *p;
    if ((p = getpwnam(name.c_str())) == NULL) {
        return 1;
    } else if (strcmp(p->pw_name,name.c_str()) == 0){
        return 0;
    }
}

int CheckPassword(string username, string password){
    int result;
    string script(PASS_CHECK_SCRIPT);
    while (script.find("chng_val_username") != string::npos)
        script.replace(script.find("chng_val_username"), strlen("chng_val_username"), username);
    while (script.find("chng_val_password") != string::npos)
        script.replace(script.find("chng_val_password"), strlen("chng_val_password"), password);
    result = WEXITSTATUS(system(script.c_str()));
    return result;
}

string GetDefDir(string name){
    struct passwd *p;
    string result;
    p = getpwnam(name.c_str());
    result = p->pw_dir;
    return result;
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

string exec(string cmd, string username) {
    string data;
    FILE * stream;
    string command = "runuser -l "+username+" -c '"+cmd+"'";
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