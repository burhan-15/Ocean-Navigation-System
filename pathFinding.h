#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "Graph.h"
#include "priorityQueue.h"
#include "linkedList.h"
#include "timeUtils.h"
#include <limits.h>

class PathFinding {
public:
    // Structure to hold path result
    struct PathResult {
        LinkedList<int> path;       // Custom List
        LinkedList<Route> routes;   // Custom List
        int totalCost;
        int totalTime;  // in minutes
        bool found;
        
        PathResult() : totalCost(0), totalTime(0), found(false) {}
    };
    
    // [FIX] Return a Pointer (PathResult*) to avoid copying the LinkedList
    static PathResult* findCheapestPath(Graph& graph, int startIndex, int endIndex) {
        // [FIX] Allocate on heap
        PathResult* result = new PathResult();
        
        if (startIndex < 0 || endIndex < 0 || startIndex >= graph.size || endIndex >= graph.size) {
            return result;
        }
        
        // Initialize arrays
        int* distances = new int[graph.size];
        int* parents = new int[graph.size];
        bool* visited = new bool[graph.size];
        Route* parentRoutes = new Route[graph.size];
        
        for (int i = 0; i < graph.size; i++) {
            distances[i] = INT_MAX;
            parents[i] = -1;
            visited[i] = false;
        }
        
        distances[startIndex] = graph.vertices[startIndex].port.portCharge;
        
        PriorityQueue<int> pq;
        pq.push(startIndex, distances[startIndex]);
        
        while (!pq.isEmpty()) {
            int current = pq.pop();
            
            if (visited[current]) continue;
            visited[current] = true;
            
            if (current == endIndex) break;
            
            LinkedList<Route>::Node* routeNode = graph.vertices[current].routes.head;
            while (routeNode != nullptr) {
                Route& route = routeNode->data;
                int destIndex = graph.findPort(route.dest.name);
                
                if (destIndex != -1 && !visited[destIndex]) {
                    int newCost = distances[current] + route.cost + route.dest.portCharge;
                    
                    if (newCost < distances[destIndex]) {
                        distances[destIndex] = newCost;
                        parents[destIndex] = current;
                        parentRoutes[destIndex] = route;
                        pq.push(destIndex, newCost);
                    }
                }
                routeNode = routeNode->next;
            }
        }
        
        if (distances[endIndex] != INT_MAX) {
            result->found = true;
            result->totalCost = distances[endIndex];
            
            // Build path backwards (using custom LinkedList)
            LinkedList<int> reversePath;
            LinkedList<Route> reverseRoutes;
            
            int current = endIndex;
            while (current != startIndex && current != -1) {
                reversePath.insertEnd(current);
                if (parents[current] != -1) {
                    reverseRoutes.insertEnd(parentRoutes[current]);
                    
                    Route& r = parentRoutes[current];
                    int timeDiff = TimeUtils::timeDifference(r.date, r.deptTime, r.date, r.arrTime);
                    if (timeDiff < 0) timeDiff += 24 * 60; 
                    result->totalTime += timeDiff;
                }
                current = parents[current];
            }
            reversePath.insertEnd(startIndex);
            
            // Reverse into the result
            for (int i = reversePath.getSize() - 1; i >= 0; i--) {
                result->path.insertEnd(reversePath.get(i));
            }
            
            for (int i = reverseRoutes.getSize() - 1; i >= 0; i--) {
                result->routes.insertEnd(reverseRoutes.get(i));
            }
        }
        
        delete[] distances;
        delete[] parents;
        delete[] visited;
        delete[] parentRoutes;
        
        return result;
    }
    
    // [FIX] Return Pointer here too
    static PathResult* findShortestTimePath(Graph& graph, int startIndex, int endIndex) {
        PathResult* result = new PathResult();
        
        if (startIndex < 0 || endIndex < 0 || startIndex >= graph.size || endIndex >= graph.size) {
            return result;
        }
        
        int* distances = new int[graph.size];
        int* parents = new int[graph.size];
        bool* visited = new bool[graph.size];
        Route* parentRoutes = new Route[graph.size];
        
        for (int i = 0; i < graph.size; i++) {
            distances[i] = INT_MAX;
            parents[i] = -1;
            visited[i] = false;
        }
        
        distances[startIndex] = 0;
        
        PriorityQueue<int> pq;
        pq.push(startIndex, 0);
        
        while (!pq.isEmpty()) {
            int current = pq.pop();
            
            if (visited[current]) continue;
            visited[current] = true;
            
            if (current == endIndex) break;
            
            LinkedList<Route>::Node* routeNode = graph.vertices[current].routes.head;
            while (routeNode != nullptr) {
                Route& route = routeNode->data;
                int destIndex = graph.findPort(route.dest.name);
                
                if (destIndex != -1 && !visited[destIndex]) {
                    int travelTime = TimeUtils::timeDifference(route.date, route.deptTime, 
                                                               route.date, route.arrTime);
                    if (travelTime < 0) travelTime += 24 * 60; 
                    
                    int newTime = distances[current] + travelTime;
                    
                    if (newTime < distances[destIndex]) {
                        distances[destIndex] = newTime;
                        parents[destIndex] = current;
                        parentRoutes[destIndex] = route;
                        pq.push(destIndex, newTime);
                    }
                }
                routeNode = routeNode->next;
            }
        }
        
        if (distances[endIndex] != INT_MAX) {
            result->found = true;
            result->totalTime = distances[endIndex];
            
            LinkedList<int> reversePath;
            LinkedList<Route> reverseRoutes;
            
            int current = endIndex;
            while (current != startIndex && current != -1) {
                reversePath.insertEnd(current);
                if (parents[current] != -1) {
                    reverseRoutes.insertEnd(parentRoutes[current]);
                    Route& r = parentRoutes[current];
                    result->totalCost += r.cost + r.dest.portCharge;
                }
                current = parents[current];
            }
            reversePath.insertEnd(startIndex);
            result->totalCost += graph.vertices[startIndex].port.portCharge;
            
            for (int i = reversePath.getSize() - 1; i >= 0; i--) {
                result->path.insertEnd(reversePath.get(i));
            }
            
            for (int i = reverseRoutes.getSize() - 1; i >= 0; i--) {
                result->routes.insertEnd(reverseRoutes.get(i));
            }
        }
        
        delete[] distances;
        delete[] parents;
        delete[] visited;
        delete[] parentRoutes;
        
        return result;
    }
};

#endif