//
// Created by dogaucak on 12/18/2020.
// Compile Command : g++ -std=c++11 Commands.cpp Functions.cpp INI_Parser.cpp INI_Parser.h Login.cpp main.cpp main.h Script.h Socket.cpp
//

#include "main.h"

int main()
{
    generate_config();
    int stat_SUDO = CheckSUDO();
    if (stat_SUDO == 0)
    {
        cout << "You are under root privileges. You can login with system user." << endl;
    } 
    else 
    {
        cout << "You are not under root privileges. You can only login with configured user." << endl;

    }
    CreateSocket();
    return 0;
}