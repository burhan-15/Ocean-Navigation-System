#include "Port.h"

Port::Port() {
    name = "";
    portCharge = 0;
}

Port::Port(string n, int chrg) {
    name = n;
    portCharge = chrg;
}
