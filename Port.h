#ifndef PORT_H
#define PORT_H

#include <string>
using namespace std;

struct Port {
    string name;
    int portCharge;

    Port();
    Port(string n, int chrg);
};

#endif
