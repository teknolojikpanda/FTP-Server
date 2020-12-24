//
// Created by dogaucak on 12/18/2020.
//

#include "main.h"

int main()
{
    int user;
    user = getuid();
    if (user == 0){
        CreateSocket();
    } else{
        cout << "Root privileges needed!" << endl;
    }
    return 0;
}