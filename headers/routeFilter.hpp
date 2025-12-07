#ifndef ROUTEFILTER_HPP
#define ROUTEFILTER_HPP

#include <string>
#include "Graph.hpp"
#include "pathFinding.h"
#include "linkedList.h"
#include "timeUtils.h"
#include "vector.h"

class RouteFilter {
public:
    // Get all unique companies from the graph
    static Vector<std::string> getAllCompanies(const Graph& graph) {
        Vector<std::string> companyList;
        
        for (int i = 0; i < graph.size; i++) {
            LinkedList<Route>::Node* node = graph.vertices[i].routes.head;
            while (node != nullptr) {
                // Check if company already exists (manual linear search)
                bool exists = false;
                for (int j = 0; j < companyList.size(); j++) {
                    if (companyList[j] == node->data.company) {
                        exists = true;
                        break;
                    }
                }
                if (!exists) {
                    companyList.push_back(node->data.company);
                }
                node = node->next;
            }
        }
        
        return companyList;
    }

    // Find all routes from origin to destination that match preferences
    // preferredPorts: ports that can be used for layovers (intermediate stops)
    // preferredCompanies: companies that can be used for route segments
    static Vector<PathFinding::PathResult*> findFilteredRoutes(
        Graph& graph,
        int originIndex,
        int destinationIndex,
        const Vector<int>& preferredPorts,
        const Vector<std::string>& preferredCompanies) {
        
        Vector<PathFinding::PathResult*> filteredPaths;
        
        if (originIndex < 0 || destinationIndex < 0 || 
            originIndex >= graph.size || destinationIndex >= graph.size) {
            return filteredPaths;
        }
        
        // Use modified Dijkstra/BFS to find all valid paths
        // We'll find multiple paths by exploring different routes
        filteredPaths = findAllPathsWithPreferences(
            graph, originIndex, destinationIndex, preferredCompanies, preferredPorts);
        
        return filteredPaths;
    }

private:
    // Find all paths from start to end using preferred companies and layover ports
    static Vector<PathFinding::PathResult*> findAllPathsWithPreferences(
        Graph& graph,
        int startIndex,
        int endIndex,
        const Vector<std::string>& preferredCompanies,
        const Vector<int>& preferredLayoverPorts) {
        
        Vector<PathFinding::PathResult*> allPaths;
        
        // Use BFS to find multiple paths
        // We'll limit to reasonable number of paths to avoid explosion
        const int MAX_PATHS = 20;
        const int MAX_DEPTH = 10; // Maximum number of stops
        
        // PathState structure for BFS
        struct PathState {
            int current;
            Vector<int> path;
            Vector<Route> routes;
            int cost;
            long long arrivalTime;
            
            PathState() : current(-1), cost(0), arrivalTime(0) {}
        };
        
        Vector<PathState> queue;
        PathState initial;
        initial.current = startIndex;
        initial.path.push_back(startIndex);
        initial.cost = 0;
        initial.arrivalTime = 0;
        queue.push_back(initial);
        
        while (queue.size() > 0 && allPaths.size() < MAX_PATHS) {
            PathState state = queue[0];
            queue.erase(0);
            
            // Check if we reached destination
            if (state.current == endIndex && state.path.size() > 1) {
                PathFinding::PathResult* result = new PathFinding::PathResult();
                result->found = true;
                
                // Build path
                for (int i = 0; i < state.path.size(); i++) {
                    result->path.insertEnd(state.path[i]);
                }
                
                // Build routes
                for (int i = 0; i < state.routes.size(); i++) {
                    result->routes.insertEnd(state.routes[i]);
                }
                
                // Calculate costs and time
                result->totalCost = state.cost;
                
                // Calculate total time
                if (state.routes.size() > 0) {
                    Route first = state.routes[0];
                    Route last = state.routes[state.routes.size() - 1];
                    
                    long long startT = TimeUtils::toAbsoluteMinutes(first.date, first.deptTime);
                    long long lastDep = TimeUtils::toAbsoluteMinutes(last.date, last.deptTime);
                    long long lastArr = TimeUtils::toAbsoluteMinutes(last.date, last.arrTime);
                    if (lastArr < lastDep) lastArr += 24 * 60;
                    
                    result->totalTime = (int)(lastArr - startT);
                }
                
                allPaths.push_back(result);
                continue;
            }
            
            // Limit depth
            if (state.path.size() > MAX_DEPTH) continue;
            
            // Explore neighbors
            LinkedList<Route>::Node* routeNode = graph.vertices[state.current].routes.head;
            while (routeNode != nullptr) {
                Route& route = routeNode->data;
                int destIndex = graph.findPort(route.dest.name);
                
                if (destIndex == -1) {
                    routeNode = routeNode->next;
                    continue;
                }
                
                // Check if we've already visited this node in this path
                bool alreadyVisited = false;
                for (int i = 0; i < state.path.size(); i++) {
                    if (state.path[i] == destIndex) {
                        alreadyVisited = true;
                        break;
                    }
                }
                if (alreadyVisited) {
                    routeNode = routeNode->next;
                    continue;
                }
                
                // Check company preference (linear search)
                bool matchesCompany = (preferredCompanies.size() == 0);
                if (!matchesCompany) {
                    for (int i = 0; i < preferredCompanies.size(); i++) {
                        if (preferredCompanies[i] == route.company) {
                            matchesCompany = true;
                            break;
                        }
                    }
                }
                if (!matchesCompany) {
                    routeNode = routeNode->next;
                    continue;
                }
                
                // Check layover port preference (linear search)
                // If destination is the final destination, allow it
                // If destination is an intermediate stop, it must be in preferred layover ports
                bool canUsePort = true;
                if (destIndex != endIndex && preferredLayoverPorts.size() > 0) {
                    canUsePort = false;
                    for (int i = 0; i < preferredLayoverPorts.size(); i++) {
                        if (preferredLayoverPorts[i] == destIndex) {
                            canUsePort = true;
                            break;
                        }
                    }
                }
                
                if (!canUsePort) {
                    routeNode = routeNode->next;
                    continue;
                }
                
                // Check timing constraints (must arrive 60 mins before departure)
                long long depAbs = TimeUtils::toAbsoluteMinutes(route.date, route.deptTime);
                long long arrAbs = TimeUtils::toAbsoluteMinutes(route.date, route.arrTime);
                if (arrAbs < depAbs) arrAbs += 24 * 60;
                
                bool timeValid = true;
                if (state.current != startIndex) {
                    // Must arrive at least 60 minutes before departure
                    if (state.arrivalTime > depAbs - 60) {
                        timeValid = false;
                    }
                }
                
                if (timeValid) {
                    PathState newState;
                    newState.current = destIndex;
                    // Deep copy vectors
                    newState.path = state.path;
                    newState.path.push_back(destIndex);
                    newState.routes = state.routes;
                    newState.routes.push_back(route);
                    newState.cost = state.cost + route.cost;
                    // Add layover fee if waiting > 12 hours
                    if (state.current != startIndex) {
                        long long layover = depAbs - state.arrivalTime;
                        if (layover > 720) {
                            newState.cost += graph.vertices[state.current].port.portCharge;
                        }
                    }
                    newState.arrivalTime = arrAbs;
                    
                    queue.push_back(newState);
                }
                
                routeNode = routeNode->next;
            }
        }
        
        return allPaths;
    }
};

#endif
