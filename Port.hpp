#ifndef PORT_HPP
#define PORT_HPP

#include <string>
using namespace std;

struct Port {
    string name;
    int portCharge;

    Port();
    Port(string n, int chrg);
};

// Implementation
Port::Port() {
    name = "";
    portCharge = 0;
}

Port::Port(string n, int chrg) {
    name = n;
    portCharge = chrg;
}

#endif


