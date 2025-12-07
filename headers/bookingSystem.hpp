#ifndef BOOKINGSYSTEM_HPP
#define BOOKINGSYSTEM_HPP

#include <string>
#include "pathFinding.h"
#include "Graph.hpp"
#include "timeUtils.h"
#include "vector.h"

struct BookedRoute {
    int originIndex;
    int destinationIndex;
    std::string departureDate;
    PathFinding::PathResult* path;
    
    BookedRoute() : originIndex(-1), destinationIndex(-1), path(nullptr) {}
    
    // 1. COPY CONSTRUCTOR (Fixes the Segfault)
    BookedRoute(const BookedRoute& other) {
        originIndex = other.originIndex;
        destinationIndex = other.destinationIndex;
        departureDate = other.departureDate;
        
        if (other.path) {
            path = new PathFinding::PathResult();
            path->found = other.path->found;
            path->totalCost = other.path->totalCost;
            path->totalTime = other.path->totalTime;
            
            // Deep copy path list
            for (int i = 0; i < other.path->path.getSize(); i++) {
                path->path.insertEnd(other.path->path.get(i));
            }
            
            // Deep copy routes list
            for (int i = 0; i < other.path->routes.getSize(); i++) {
                path->routes.insertEnd(other.path->routes.get(i));
            }
        } else {
            path = nullptr;
        }
    }

    // 2. ASSIGNMENT OPERATOR (Good practice)
    BookedRoute& operator=(const BookedRoute& other) {
        if (this != &other) {
            // Delete existing path
            if (path) delete path;
            
            originIndex = other.originIndex;
            destinationIndex = other.destinationIndex;
            departureDate = other.departureDate;
            
            if (other.path) {
                path = new PathFinding::PathResult();
                path->found = other.path->found;
                path->totalCost = other.path->totalCost;
                path->totalTime = other.path->totalTime;
                
                for (int i = 0; i < other.path->path.getSize(); i++) {
                    path->path.insertEnd(other.path->path.get(i));
                }
                for (int i = 0; i < other.path->routes.getSize(); i++) {
                    path->routes.insertEnd(other.path->routes.get(i));
                }
            } else {
                path = nullptr;
            }
        }
        return *this;
    }
    
    ~BookedRoute() {
        if (path) {
            delete path;
        }
    }
    
    // Check if this booking conflicts with a given route
    bool conflictsWith(const PathFinding::PathResult* route, const std::string& date) const {
        if (!path || !route) return false;
        if (!path->found || !route->found) return false;
        
        // Check if dates overlap
        if (departureDate != date) return false;
        
        // Compare the route legs
        int routeSize = route->routes.getSize();
        int pathSize = path->routes.getSize();
        
        if (routeSize <= 0 || pathSize <= 0) return false;
        
        for (int i = 0; i < routeSize; i++) {
            Route r1 = route->routes.get(i);
            for (int j = 0; j < pathSize; j++) {
                Route r2 = path->routes.get(j);
                
                if (r1.startPoint.name == r2.startPoint.name &&
                    r1.dest.name == r2.dest.name &&
                    r1.date == r2.date &&
                    r1.deptTime == r2.deptTime &&
                    r1.company == r2.company) {
                    return true; // Conflict found
                }
            }
        }
        return false;
    }
};

class BookingSystem {
public:
    static Vector<BookedRoute> bookedRoutes;
    
    static void addBooking(int originIndex, int destinationIndex, 
                          const std::string& departureDate,
                          PathFinding::PathResult* path) {
        BookedRoute booking;
        booking.originIndex = originIndex;
        booking.destinationIndex = destinationIndex;
        booking.departureDate = departureDate;
        
        // Create initial deep copy for the local variable
        booking.path = new PathFinding::PathResult();
        booking.path->found = path->found;
        booking.path->totalCost = path->totalCost;
        booking.path->totalTime = path->totalTime;
        
        int pathSize = path->path.getSize();
        for (int i = 0; i < pathSize; i++) {
            booking.path->path.insertEnd(path->path.get(i));
        }
        
        int routesSize = path->routes.getSize();
        for (int i = 0; i < routesSize; i++) {
            booking.path->routes.insertEnd(path->routes.get(i));
        }
        
        bookedRoutes.push_back(booking);
    }
    
    static bool isRouteAvailable(const PathFinding::PathResult* route, 
                                 const std::string& date) {
        for (int i = 0; i < bookedRoutes.size(); i++) {
            const BookedRoute& booking = bookedRoutes[i];
            if (booking.conflictsWith(route, date)) {
                return false;
            }
        }
        return true;
    }
    
    static Vector<PathFinding::PathResult*> filterAvailableRoutes(
        const Vector<PathFinding::PathResult*>& routes,
        const std::string& date) {
        Vector<PathFinding::PathResult*> available;
        for (int i = 0; i < routes.size(); i++) {
            PathFinding::PathResult* route = routes[i];
            if (isRouteAvailable(route, date)) {
                available.push_back(route);
            }
        }
        return available;
    }
};

// Static member definition
Vector<BookedRoute> BookingSystem::bookedRoutes;

#endif