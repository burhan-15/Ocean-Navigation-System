#include "Vertex.h"

Vertex::Vertex() {
    minCost = 2147483647;
    parentIndex = -1;
}

void Vertex::addPort(Port p1) {
    port = p1;
}
