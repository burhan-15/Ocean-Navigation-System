#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "Port.hpp"
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

// Implementation
Vertex::Vertex() {
    minCost = 2147483647;
    parentIndex = -1;
}

void Vertex::addPort(Port p1) {
    port = p1;
}

#endif


