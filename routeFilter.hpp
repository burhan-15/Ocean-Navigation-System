#ifndef ROUTEFILTER_HPP
#define ROUTEFILTER_HPP

#include <vector>
#include <set>
#include <string>
#include "Graph.hpp"
#include "pathFinding.h"
#include "linkedList.h"
#include "timeUtils.h"

class RouteFilter {
public:
    // Get all unique companies from the graph
    static std::vector<std::string> getAllCompanies(const Graph& graph) {
        std::set<std::string> companySet;
        
        for (int i = 0; i < graph.size; i++) {
            LinkedList<Route>::Node* node = graph.vertices[i].routes.head;
            while (node != nullptr) {
                companySet.insert(node->data.company);
                node = node->next;
            }
        }
        
        return std::vector<std::string>(companySet.begin(), companySet.end());
    }

    // Find all routes from origin to destination that match preferences
    // preferredPorts: ports that can be used for layovers (intermediate stops)
    // preferredCompanies: companies that can be used for route segments
    static std::vector<PathFinding::PathResult*> findFilteredRoutes(
        Graph& graph,
        int originIndex,
        int destinationIndex,
        const std::vector<int>& preferredPorts,
        const std::vector<std::string>& preferredCompanies) {
        
        std::vector<PathFinding::PathResult*> filteredPaths;
        
        if (originIndex < 0 || destinationIndex < 0 || 
            originIndex >= graph.size || destinationIndex >= graph.size) {
            return filteredPaths;
        }
        
        // Convert to sets for faster lookup
        std::set<std::string> companySet(preferredCompanies.begin(), preferredCompanies.end());
        std::set<int> layoverPortSet(preferredPorts.begin(), preferredPorts.end());
        
        // Use modified Dijkstra/BFS to find all valid paths
        // We'll find multiple paths by exploring different routes
        filteredPaths = findAllPathsWithPreferences(
            graph, originIndex, destinationIndex, companySet, layoverPortSet);
        
        return filteredPaths;
    }

private:
    // Find all paths from start to end using preferred companies and layover ports
    static std::vector<PathFinding::PathResult*> findAllPathsWithPreferences(
        Graph& graph,
        int startIndex,
        int endIndex,
        const std::set<std::string>& preferredCompanies,
        const std::set<int>& preferredLayoverPorts) {
        
        std::vector<PathFinding::PathResult*> allPaths;
        
        // Use BFS to find multiple paths
        // We'll limit to reasonable number of paths to avoid explosion
        const int MAX_PATHS = 20;
        const int MAX_DEPTH = 10; // Maximum number of stops
        
        // PathState structure for BFS
        struct PathState {
            int current;
            std::vector<int> path;
            std::vector<Route> routes;
            int cost;
            long long arrivalTime;
            
            PathState() : current(-1), cost(0), arrivalTime(0) {}
        };
        
        std::vector<PathState> queue;
        PathState initial;
        initial.current = startIndex;
        initial.path.push_back(startIndex);
        initial.cost = 0;
        initial.arrivalTime = 0;
        queue.push_back(initial);
        
        while (!queue.empty() && allPaths.size() < MAX_PATHS) {
            PathState state = queue[0];
            queue.erase(queue.begin());
            
            // Check if we reached destination
            if (state.current == endIndex && state.path.size() > 1) {
                PathFinding::PathResult* result = new PathFinding::PathResult();
                result->found = true;
                
                // Build path
                for (int portIdx : state.path) {
                    result->path.insertEnd(portIdx);
                }
                
                // Build routes
                for (const Route& route : state.routes) {
                    result->routes.insertEnd(route);
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
                for (int visited : state.path) {
                    if (visited == destIndex) {
                        alreadyVisited = true;
                        break;
                    }
                }
                if (alreadyVisited) {
                    routeNode = routeNode->next;
                    continue;
                }
                
                // Check company preference
                bool matchesCompany = preferredCompanies.empty() || 
                                     preferredCompanies.find(route.company) != preferredCompanies.end();
                if (!matchesCompany) {
                    routeNode = routeNode->next;
                    continue;
                }
                
                // Check layover port preference
                // If destination is the final destination, allow it
                // If destination is an intermediate stop, it must be in preferred layover ports
                bool canUsePort = true;
                if (destIndex != endIndex && !preferredLayoverPorts.empty()) {
                    canUsePort = (preferredLayoverPorts.find(destIndex) != preferredLayoverPorts.end());
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
                    PathState newState = state;
                    newState.current = destIndex;
                    newState.path.push_back(destIndex);
                    newState.routes.push_back(route);
                    newState.cost += route.cost;
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
