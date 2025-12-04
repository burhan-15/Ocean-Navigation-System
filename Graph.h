#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include "Vertex.h"

class Graph {
public:
    Vertex* vertices;
    int size;

    Graph();
    ~Graph();

    void addPorts(std::string file);
    void addRoutes(std::string file);
    int findPort(const std::string& name);
};

#endif
