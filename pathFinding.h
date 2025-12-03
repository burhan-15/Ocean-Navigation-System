#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "priorityQueue.cpp"
#include "linkedList.h"
#include "timeUtils.h"
#include "project.cpp"
#include <string>
#include <cmath>

using namespace std;

struct PathResult {
    LinkedList<int> path;  // Sequence of port indices
    LinkedList<Route> routes;  // Actual routes taken
    int totalCost;
    int totalTime;
    bool found;
    
    PathResult() : totalCost(0), totalTime(0), found(false) {}
};

class Pathfinding {
public:
    // Dijkstra's algorithm - finds cheapest route
    static PathResult dijkstraCheapest(Graph& graph, int startIdx, int endIdx, 
                                       const string& targetDate) {
        PathResult result;
        
        if (startIdx < 0 || endIdx < 0 || startIdx >= graph.size || endIdx >= graph.size) {
            return result;
        }
        
        // Initialize distances
        int* dist = new int[graph.size];
        int* parent = new int[graph.size];
        Route* parentRoute = new Route[graph.size];
        bool* visited = new bool[graph.size];
        
        for (int i = 0; i < graph.size; i++) {
            dist[i] = 2147483647; // Infinity
            parent[i] = -1;
            visited[i] = false;
        }
        
        dist[startIdx] = 0;
        PriorityQueue<int> pq;
        pq.push(startIdx, 0);
        
        while (!pq.isEmpty()) {
            int u = pq.pop();
            
            if (visited[u]) continue;
            visited[u] = true;
            
            if (u == endIdx) break;
            
            // Explore neighbors
            auto* current = graph.vertices[u].routes.head;
            while (current) {
                Route& route = current->data;
                int v = graph.findPort(route.dest.name);
                
                if (v != -1 && !visited[v]) {
                    // Check if date matches or is later
                    int newCost = dist[u] + route.cost;
                    
                    // Add port charges if there's a layover
                    if (parent[u] != -1) {
                        // Calculate layover time
                        Route& prevRoute = parentRoute[u];
                        int layoverMins = TimeUtils::timeDifference(
                            prevRoute.date, prevRoute.arrTime,
                            route.date, route.deptTime
                        );
                        
                        // If layover > 12 hours, add port charges (per day)
                        if (layoverMins > 12 * 60) {
                            int layoverDays = (layoverMins + 1439) / (24 * 60); // Ceiling division
                            newCost += graph.vertices[u].port.portCharge * layoverDays;
                        }
                    }
                    
                    if (newCost < dist[v]) {
                        dist[v] = newCost;
                        parent[v] = u;
                        parentRoute[v] = route;
                        pq.push(v, newCost);
                    }
                }
                current = current->next;
            }
        }
        
        // Reconstruct path
        if (dist[endIdx] != 2147483647) {
            result.found = true;
            result.totalCost = dist[endIdx];
            
            // Build path backwards
            LinkedList<int> reversePath;
            LinkedList<Route> reverseRoutes;
            int curr = endIdx;
            
            while (parent[curr] != -1) {
                reversePath.insertEnd(curr);
                reverseRoutes.insertEnd(parentRoute[curr]);
                curr = parent[curr];
            }
            reversePath.insertEnd(startIdx);
            
            // Reverse the path
            for (int i = reversePath.getSize() - 1; i >= 0; i--) {
                result.path.insertEnd(reversePath.get(i));
            }
            
            for (int i = reverseRoutes.getSize() - 1; i >= 0; i--) {
                result.routes.insertEnd(reverseRoutes.get(i));
            }
        }
        
        delete[] dist;
        delete[] parent;
        delete[] parentRoute;
        delete[] visited;
        
        return result;
    }
    
    // A* algorithm - finds shortest time route
    static PathResult aStarShortestTime(Graph& graph, int startIdx, int endIdx,
                                        const string& targetDate,
                                        float* positions) {
        PathResult result;
        
        if (startIdx < 0 || endIdx < 0 || startIdx >= graph.size || endIdx >= graph.size) {
            return result;
        }
        
        // Initialize
        int* gScore = new int[graph.size];  // Actual time from start
        int* fScore = new int[graph.size];  // gScore + heuristic
        int* parent = new int[graph.size];
        Route* parentRoute = new Route[graph.size];
        bool* visited = new bool[graph.size];
        
        for (int i = 0; i < graph.size; i++) {
            gScore[i] = 2147483647;
            fScore[i] = 2147483647;
            parent[i] = -1;
            visited[i] = false;
        }
        
        gScore[startIdx] = 0;
        
        // Heuristic: straight-line distance (scaled to time estimate)
        auto heuristic = [&](int idx) -> int {
            if (!positions) return 0;
            float dx = positions[idx * 2] - positions[endIdx * 2];
            float dy = positions[idx * 2 + 1] - positions[endIdx * 2 + 1];
            float dist = sqrt(dx * dx + dy * dy);
            return (int)(dist * 10); // Scale factor for time estimation
        };
        
        fScore[startIdx] = heuristic(startIdx);
        
        PriorityQueue<int> pq;
        pq.push(startIdx, fScore[startIdx]);
        
        while (!pq.isEmpty()) {
            int u = pq.pop();
            
            if (visited[u]) continue;
            visited[u] = true;
            
            if (u == endIdx) break;
            
            // Explore neighbors
            auto* current = graph.vertices[u].routes.head;
            while (current) {
                Route& route = current->data;
                int v = graph.findPort(route.dest.name);
                
                if (v != -1 && !visited[v]) {
                    // Calculate travel time in minutes
                    int travelTime = TimeUtils::timeDifference(
                        route.date, route.deptTime,
                        route.date, route.arrTime
                    );
                    if (travelTime < 0) travelTime += 24 * 60; // Next day arrival
                    
                    int newGScore = gScore[u] + travelTime;
                    
                    // Add layover time if there's a previous route
                    if (parent[u] != -1) {
                        Route& prevRoute = parentRoute[u];
                        int layoverTime = TimeUtils::timeDifference(
                            prevRoute.date, prevRoute.arrTime,
                            route.date, route.deptTime
                        );
                        newGScore += layoverTime;
                    }
                    
                    if (newGScore < gScore[v]) {
                        gScore[v] = newGScore;
                        fScore[v] = newGScore + heuristic(v);
                        parent[v] = u;
                        parentRoute[v] = route;
                        pq.push(v, fScore[v]);
                    }
                }
                current = current->next;
            }
        }
        
        // Reconstruct path
        if (gScore[endIdx] != 2147483647) {
            result.found = true;
            result.totalTime = gScore[endIdx];
            
            // Calculate total cost
            int totalCost = 0;
            LinkedList<int> reversePath;
            LinkedList<Route> reverseRoutes;
            int curr = endIdx;
            
            while (parent[curr] != -1) {
                reversePath.insertEnd(curr);
                reverseRoutes.insertEnd(parentRoute[curr]);
                totalCost += parentRoute[curr].cost;
                curr = parent[curr];
            }
            reversePath.insertEnd(startIdx);
            
            result.totalCost = totalCost;
            
            // Reverse the path
            for (int i = reversePath.getSize() - 1; i >= 0; i--) {
                result.path.insertEnd(reversePath.get(i));
            }
            
            for (int i = reverseRoutes.getSize() - 1; i >= 0; i--) {
                result.routes.insertEnd(reverseRoutes.get(i));
            }
        }
        
        delete[] gScore;
        delete[] fScore;
        delete[] parent;
        delete[] parentRoute;
        delete[] visited;
        
        return result;
    }
};

#endif