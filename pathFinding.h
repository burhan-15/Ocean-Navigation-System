#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "Graph.h"
#include "priorityQueue.h"
#include "linkedList.h"
#include "timeUtils.h"
#include <limits.h>

class PathFinding {
public:
    struct PathResult {
        LinkedList<int> path;       
        LinkedList<Route> routes;   
        int totalCost;
        int totalTime; 
        bool found;
        
        PathResult() : totalCost(0), totalTime(0), found(false) {}
    };
    
    // ---------------------------------------------------------
    // ALGORITHM 1: CHEAPEST PATH (Cost + Conditional Layover Fee)
    // ---------------------------------------------------------
    static PathResult* findCheapestPath(Graph& graph, int startIndex, int endIndex) {
        PathResult* result = new PathResult();
        if (startIndex < 0 || endIndex < 0 || startIndex >= graph.size || endIndex >= graph.size) return result;
        
        int* distances = new int[graph.size];
        int* parents = new int[graph.size];
        bool* visited = new bool[graph.size];
        Route* parentRoutes = new Route[graph.size];
        long long* arrivalTimes = new long long[graph.size]; 
        
        for (int i = 0; i < graph.size; i++) {
            distances[i] = INT_MAX;
            parents[i] = -1;
            visited[i] = false;
            arrivalTimes[i] = -1; 
        }
        
        distances[startIndex] = 0; 
        arrivalTimes[startIndex] = 0; 
        
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
                    long long depAbs = TimeUtils::toAbsoluteMinutes(route.date, route.deptTime);
                    long long arrAbs = TimeUtils::toAbsoluteMinutes(route.date, route.arrTime);
                    if (arrAbs < depAbs) arrAbs += 24 * 60; 

                    bool isTimeValid = true;
                    int layoverFee = 0;

                    if (current != startIndex) {
                        // Check if we arrived before this boat leaves
                        long long layoverMinutes = depAbs - arrivalTimes[current];
                        
                        // Must arrive 60 mins before departure
                        if (layoverMinutes < 60) isTimeValid = false; 
                        
                        // Apply Layover Fee if waiting > 12 hours
                        if (isTimeValid && layoverMinutes > 720) {
                            layoverFee = graph.vertices[current].port.portCharge;
                        }
                    }

                    if (isTimeValid) {
                        int newCost = distances[current] + route.cost + layoverFee;
                        if (newCost < distances[destIndex]) {
                            distances[destIndex] = newCost;
                            parents[destIndex] = current;
                            parentRoutes[destIndex] = route;
                            arrivalTimes[destIndex] = arrAbs; 
                            pq.push(destIndex, newCost);
                        }
                    }
                }
                routeNode = routeNode->next;
            }
        }
        
        if (distances[endIndex] != INT_MAX) {
            result->found = true;
            result->totalCost = distances[endIndex];
            
            LinkedList<int> reversePath;
            LinkedList<Route> reverseRoutes;
            int current = endIndex;
            while (current != startIndex && current != -1) {
                reversePath.insertEnd(current);
                if (parents[current] != -1) reverseRoutes.insertEnd(parentRoutes[current]);
                current = parents[current];
            }
            reversePath.insertEnd(startIndex);
            
            for (int i = reversePath.getSize() - 1; i >= 0; i--) result->path.insertEnd(reversePath.get(i));
            
            // Calculate Total Time
            long long startT = 0; 
            long long endT = 0;
            if(reverseRoutes.getSize() > 0) {
                 Route first = reverseRoutes.get(reverseRoutes.getSize()-1);
                 startT = TimeUtils::toAbsoluteMinutes(first.date, first.deptTime);
                 
                 Route last = reverseRoutes.get(0);
                 long long lastDep = TimeUtils::toAbsoluteMinutes(last.date, last.deptTime);
                 long long lastArr = TimeUtils::toAbsoluteMinutes(last.date, last.arrTime);
                 if (lastArr < lastDep) lastArr += 24*60;
                //  if (lastArr - lastDep < 720) lastArr += 24*60; // Heuristic for last leg
                 endT = lastArr;
                 
                 result->totalTime = (int)(endT - startT);
            }
            
            for (int i = reverseRoutes.getSize() - 1; i >= 0; i--) result->routes.insertEnd(reverseRoutes.get(i));
        }
        
        delete[] distances; delete[] parents; delete[] visited; delete[] parentRoutes; delete[] arrivalTimes;
        return result;
    }
    
    
    static PathResult* findShortestTimePath(Graph& graph, int startIndex, int endIndex) {
        PathResult* result = new PathResult();

        if (startIndex < 0 || endIndex < 0 || startIndex >= graph.size || endIndex >= graph.size) {
            return result;
        }

        long long* bestTime = new long long[graph.size]; 
        int* parents = new int[graph.size];
        bool* visited = new bool[graph.size];
        Route* parentRoutes = new Route[graph.size];

        // NEW: Store parent departure DATE (absolute minutes)
        long long* parentDepartureDates = new long long[graph.size];

        for (int i = 0; i < graph.size; i++) {
            bestTime[i] = LLONG_MAX;
            parents[i] = -1;
            visited[i] = false;
            parentDepartureDates[i] = -1;
        }

        // Start at time = 0
        bestTime[startIndex] = 0;

        PriorityQueue<long long> pq;
        pq.push(startIndex, 0);

        while (!pq.isEmpty()) {
            int current = pq.pop();
            if (visited[current]) continue;
            visited[current] = true;

            if (current == endIndex) break;

            auto* routeNode = graph.vertices[current].routes.head;
            while (routeNode != nullptr) {
                Route& route = routeNode->data;
                int destIndex = graph.findPort(route.dest.name);

                if (destIndex != -1 && !visited[destIndex]) {

                    long long depAbs = TimeUtils::toAbsoluteMinutes(route.date, route.deptTime);
                    long long arrAbs = TimeUtils::toAbsoluteMinutes(route.date, route.arrTime);

                    // if arrival time < departure time -> next day arrival
                    if (arrAbs < depAbs) arrAbs += 24 * 60;

                    bool canTake = false;

                    if (current == startIndex) {
                        // Starting node can always take route
                        canTake = true;
                    } else {
                        // Must arrive at least 60 minutes before departure
                        if (bestTime[current] <= depAbs - 60)
                            canTake = true;
                    }

                    if (canTake) {

                        long long parentDate = (current == startIndex)
                            ? depAbs
                            : parentDepartureDates[current];

                        if (parentDate == -1)
                            parentDate = depAbs;

                        // ---------- NEW LOGIC ----------
                        // Travel time = arrivalDate - parentDepartureDate
                        long long travelTime = arrAbs - parentDate;
                        // Time cannot be negative → invalid route
                        if (travelTime < 0) {
                            routeNode = routeNode->next;
                            continue;
                        }

                        long long newTime = bestTime[current] + travelTime;
                        // --------------------------------

                        if (newTime < bestTime[destIndex]) {
                            bestTime[destIndex] = newTime;
                            parents[destIndex] = current;
                            parentRoutes[destIndex] = route;

                            // Store THIS departure date for children
                            parentDepartureDates[destIndex] = depAbs;

                            pq.push(destIndex, newTime);
                        }
                    }
                }
                routeNode = routeNode->next;
            }
        }

        if (bestTime[endIndex] != LLONG_MAX) {
            result->found = true;

            LinkedList<int> revPath;
            LinkedList<Route> revRoutes;

            // Reconstruct path & routes (in reverse) first
            int cur = endIndex;
            while (cur != startIndex && cur != -1) {
                revPath.insertEnd(cur);
                if (parents[cur] != -1) revRoutes.insertEnd(parentRoutes[cur]);
                cur = parents[cur];
            }
            revPath.insertEnd(startIndex);

            // Build final path in correct order
            for (int i = revPath.getSize() - 1; i >= 0; i--)
                result->path.insertEnd(revPath.get(i));

            // If we have route legs, compute totalCost including layover fees and totalTime
            if (revRoutes.getSize() > 0) {
                // revRoutes is in reverse (last leg at index 0), so build a forward vector
                std::vector<Route> legs;
                for (int i = revRoutes.getSize() - 1; i >= 0; i--) legs.push_back(revRoutes.get(i));

                long long startT = TimeUtils::toAbsoluteMinutes(legs.front().date, legs.front().deptTime);
                long long prevArrival = TimeUtils::toAbsoluteMinutes(legs.front().date, legs.front().arrTime);
                if (prevArrival < startT) prevArrival += 24 * 60;

                int totalCost = 0;
                totalCost += legs.front().cost;

                // iterate remaining legs and compute waiting & layover fee same as cheapest
                for (size_t i = 1; i < legs.size(); ++i) {
                    Route &r = legs[i];

                    long long depAbs = TimeUtils::toAbsoluteMinutes(r.date, r.deptTime);
                    long long arrAbs = TimeUtils::toAbsoluteMinutes(r.date, r.arrTime);
                    if (arrAbs < depAbs) arrAbs += 24 * 60;

                    // Strict timing (you already enforced while searching), but recompute waiting:
                    long long waiting = depAbs - prevArrival;

                    // If waiting negative -> invalid path (shouldn't happen), skip adding fee and treat as invalid
                    if (waiting < 0) {
                        // defensive: treat path invalid (shouldn't happen)
                        totalCost = INT_MAX;
                        break;
                    }

                    // Apply layover fee same rule as cheapest
                    if (waiting > 720) {
                        
                        
                        int portIndex = -1;
                        portIndex = graph.findPort(legs[i-1].dest.name);
                        if (portIndex != -1) {
                            totalCost += graph.vertices[portIndex].port.portCharge;
                        } else {
                            // If for some reason portIndex unknown, still safe to skip fee or log (defensive)
                        }
                    }

                    totalCost += r.cost;

                    // update prevArrival for next leg (arrival of this leg)
                    prevArrival = arrAbs;
                }

                // If only one leg (single route), cost already added; prevArrival is arrival of that leg.

                // Now compute total time: prevArrival - startT
                long long  totalTimeLong = prevArrival - startT;
                if (totalCost == INT_MAX) {
                    // defensive: mark as not found if cost invalid
                    result->found = false;
                    result->totalCost = 0;
                    result->totalTime = 0;
                } else {
                    result->totalCost = totalCost;
                    result->totalTime = (int) totalTimeLong;
                }

                // Finally insert forward routes into result->routes
                for (const Route &r : legs) result->routes.insertEnd(r);
            }


            // Build final routes
            for (int i = revRoutes.getSize() - 1; i >= 0; i--)
                result->routes.insertEnd(revRoutes.get(i));
        }

        delete[] bestTime;
        delete[] parents;
        delete[] visited;
        delete[] parentRoutes;
        delete[] parentDepartureDates;

        return result;
    }

};

#endif