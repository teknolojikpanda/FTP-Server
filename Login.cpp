//
// Created by dogaucak on 12/24/2020.
//

#include "main.h"

int CheckSysUser(string name) {
    struct passwd *p;
    if ((p = getpwnam(name.c_str())) == NULL) {
        return 1;
    } else if (strcmp(p->pw_name,name.c_str()) == 0){
        return 0;
    }
}

string GetCurrentUser(){
    struct passwd *pw;
    uid_t uid;
    int c;

    uid = geteuid ();
    pw = getpwuid (uid);
    return pw->pw_name;
}

int CheckSysPassword(string username, string password){
    int result;
    string script(PASS_CHECK_SCRIPT);
    while (script.find("chng_val_username") != string::npos)
        script.replace(script.find("chng_val_username"), strlen("chng_val_username"), username);
    while (script.find("chng_val_password") != string::npos)
        script.replace(script.find("chng_val_password"), strlen("chng_val_password"), password);
    int sys_return = system(script.c_str());
    result = WEXITSTATUS(sys_return);
    return result;
}

int CheckINIUser(string username){
    config cfg("config.ini");
    if (strcmp(username.c_str(),cfg.get_value("config", "username").c_str()) == 0){
        return 0;
    } else {
        return 1;
    }
}

int CheckINIPassword(string password){
    config cfg("config.ini");
    if (strcmp(password.c_str(),cfg.get_value("config", "password").c_str()) == 0){
        return 0;
    } else {
        return 1;
    }
}