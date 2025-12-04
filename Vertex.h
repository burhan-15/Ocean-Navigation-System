#ifndef VERTEX_H
#define VERTEX_H

#include "Port.h"
#include "Route.h"
#include "linkedList.h"   // your provided linked list template

struct Vertex {
    Port port;
    LinkedList<Route> routes;

    int minCost;
    int parentIndex;

    Vertex();
    void addPort(Port p1);
};

#endif
