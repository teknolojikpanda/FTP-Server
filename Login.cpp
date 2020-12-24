//
// Created by dogaucak on 12/24/2020.
//

#include "main.h"

int CheckUser(char *name) {
    struct passwd *p;
    if ((p = getpwnam(name)) == NULL) {
        return 1;
    } else if (strcmp(p->pw_name,name) == 0){
        return 0;
    }
}

int CheckPassword(char username[2000], char password[2000]){
    int result;
    string script(PASS_CHECK_SCRIPT);
    while (script.find("chng_val_username") != string::npos)
        script.replace(script.find("chng_val_username"), strlen("chng_val_username"), username);
    while (script.find("chng_val_password") != string::npos)
        script.replace(script.find("chng_val_password"), strlen("chng_val_password"), password);
    result = WEXITSTATUS(system(script.c_str()));
    return result;
}