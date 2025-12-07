#ifndef ROUTE_H
#define ROUTE_H

#include <string>
#include "Port.hpp"

struct Route {
    Port startPoint;
    Port dest;
    string date;
    string deptTime;
    string arrTime;
    int cost;
    string company;
};

#endif
