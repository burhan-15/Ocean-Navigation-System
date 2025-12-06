#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <fstream>
#include <sstream>
#include "Vertex.hpp"
using namespace std;

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

// Implementation
Graph::Graph() {
    vertices = nullptr;
    size = 0;
}

Graph::~Graph() {
    delete[] vertices;
}

void Graph::addPorts(string dest) {
    fstream myFile(dest);
    string portName;
    int charge;

    int count = 0;
    while (myFile >> portName >> charge) count++;

    size = count;
    vertices = new Vertex[size];

    myFile.clear();
    myFile.seekg(0, ios::beg);

    count = 0;
    while (myFile >> portName >> charge) {
        vertices[count++].addPort(Port(portName, charge));
    }
    myFile.close();
}

int Graph::findPort(const string& name) {
    for (int i = 0; i < size; i++)
        if (vertices[i].port.name == name) return i;
    return -1;
}

void Graph::addRoutes(string fileName) {
    ifstream file(fileName);
    string start, dest, date, dept, arr, company;
    int cost;

    while (file >> start >> dest >> date >> dept >> arr >> cost >> company) {
        int u = findPort(start);
        int v = findPort(dest);

        if (u == -1 || v == -1) continue;

        Route r;
        r.startPoint = vertices[u].port;
        r.dest = vertices[v].port;
        r.date = date;
        r.deptTime = dept;
        r.arrTime = arr;
        r.cost = cost;
        r.company = company;

        vertices[u].routes.insertEnd(r);
    }
}

#endif


