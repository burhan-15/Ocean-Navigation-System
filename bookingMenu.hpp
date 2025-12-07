#ifndef BOOKINGMENU_HPP
#define BOOKINGMENU_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include "Graph.hpp"
#include "pathFinding.h"
#include "bookingSystem.hpp"
#include "uiHelpers.hpp"
#include "vector.h"

struct BookingMenu {
    // Origin and Destination selection
    sf::RectangleShape fieldOrigin;
    sf::RectangleShape fieldDest;
    sf::Text originLabel;
    sf::Text destLabel;
    bool selectingOrigin;
    bool selectingDest;
    int selectedOriginIndex;
    int selectedDestIndex;
    int originDestListOffset;
    
    // Date selection
    sf::RectangleShape dateField;
    sf::Text dateLabel;
    sf::Text dateDisplay;
    bool selectingDate;
    std::string departureDate;
    Vector<std::string> availableDates;
    int dateListOffset;
    
    // Option buttons
    sf::RectangleShape directPathsBtn;
    sf::RectangleShape connectedPathsBtn;
    sf::Text directPathsTxt;
    sf::Text connectedPathsTxt;
    
    // Book button
    sf::RectangleShape bookBtn;
    sf::Text bookBtnTxt;
    
    // Show Subgraph button
    sf::RectangleShape showSubgraphBtn;
    sf::Text showSubgraphBtnTxt;
    bool showSubgraph;
    
    // Navigation buttons
    sf::RectangleShape prevBtn;
    sf::RectangleShape nextBtn;
    sf::Text prevBtnTxt;
    sf::Text nextBtnTxt;
    sf::Text routeCounterTxt;
    
    // Available routes
    Vector<PathFinding::PathResult*> availableRoutes;
    int currentRouteIndex;
    bool showingDirectPaths;
    
    // Results display
    sf::RectangleShape resultsArea;
    sf::Text resultHeader;
    sf::Text resultBody;
    
    sf::Color btnNormal;
    sf::Color btnHover;
    sf::Color selectedColor;

    BookingMenu(const Graph& /*graph*/, const sf::Font& font)
        : fieldOrigin(sf::Vector2f(340.f, 40.f)),
          fieldDest(sf::Vector2f(340.f, 40.f)),
          originLabel(),
          destLabel(),
          selectingOrigin(false),
          selectingDest(false),
          selectedOriginIndex(-1),
          selectedDestIndex(-1),
          originDestListOffset(0),
          dateField(sf::Vector2f(340.f, 40.f)),
          dateLabel(),
          dateDisplay(),
          selectingDate(false),
          departureDate(),
          availableDates(),
          dateListOffset(0),
          directPathsBtn(sf::Vector2f(160.f, 50.f)),
          connectedPathsBtn(sf::Vector2f(170.f, 50.f)),
          directPathsTxt(),
          connectedPathsTxt(),
          bookBtn(sf::Vector2f(340.f, 50.f)),
          bookBtnTxt(),
          showSubgraphBtn(sf::Vector2f(340.f, 50.f)),
          showSubgraphBtnTxt(),
          showSubgraph(false),
          prevBtn(sf::Vector2f(50.f, 40.f)),
          nextBtn(sf::Vector2f(50.f, 40.f)),
          prevBtnTxt(),
          nextBtnTxt(),
          routeCounterTxt(),
          availableRoutes(),
          currentRouteIndex(-1),
          showingDirectPaths(false),
          resultsArea(sf::Vector2f(360.f, 200.f)),
          resultHeader(),
          resultBody(),
          btnNormal(70, 80, 100),
          btnHover(90, 100, 125),
          selectedColor(0, 150, 200) {
        
        // Setup origin/destination fields
        fieldOrigin.setFillColor(sf::Color(30, 40, 50));
        fieldOrigin.setOutlineColor(sf::Color(120, 120, 120));
        fieldOrigin.setOutlineThickness(1.f);
        
        fieldDest.setFillColor(sf::Color(30, 40, 50));
        fieldDest.setOutlineColor(sf::Color(120, 120, 120));
        fieldDest.setOutlineThickness(1.f);
        
        originLabel.setFont(font);
        originLabel.setString("Select Origin...");
        originLabel.setCharacterSize(15);
        originLabel.setFillColor(sf::Color(150, 150, 150));
        
        destLabel.setFont(font);
        destLabel.setString("Select Destination...");
        destLabel.setCharacterSize(15);
        destLabel.setFillColor(sf::Color(150, 150, 150));
        
        // Setup date field
        dateField.setFillColor(sf::Color(30, 40, 50));
        dateField.setOutlineColor(sf::Color(120, 120, 120));
        dateField.setOutlineThickness(1.f);
        
        dateDisplay.setFont(font);
        dateDisplay.setString("Select Date...");
        dateDisplay.setCharacterSize(15);
        dateDisplay.setFillColor(sf::Color(150, 150, 150));
        
        // Setup option buttons
        directPathsBtn.setFillColor(btnNormal);
        connectedPathsBtn.setFillColor(btnNormal);
        
        directPathsTxt.setFont(font);
        directPathsTxt.setString("Direct Paths");
        directPathsTxt.setCharacterSize(15);
        directPathsTxt.setFillColor(sf::Color::White);
        
        connectedPathsTxt.setFont(font);
        connectedPathsTxt.setString("Connected Paths");
        connectedPathsTxt.setCharacterSize(15);
        connectedPathsTxt.setFillColor(sf::Color::White);
        
        // Setup book button
        bookBtn.setFillColor(btnNormal);
        bookBtnTxt.setFont(font);
        bookBtnTxt.setString("Book Selected Route");
        bookBtnTxt.setCharacterSize(15);
        bookBtnTxt.setFillColor(sf::Color::White);
        
        // Setup show subgraph button
        showSubgraphBtn.setFillColor(btnNormal);
        showSubgraphBtnTxt.setFont(font);
        showSubgraphBtnTxt.setString("Show Subgraph");
        showSubgraphBtnTxt.setCharacterSize(15);
        showSubgraphBtnTxt.setFillColor(sf::Color::White);
        
        // Setup navigation buttons
        prevBtn.setFillColor(btnNormal);
        nextBtn.setFillColor(btnNormal);
        prevBtnTxt.setFont(font);
        prevBtnTxt.setString("<");
        prevBtnTxt.setCharacterSize(20);
        prevBtnTxt.setFillColor(sf::Color::White);
        nextBtnTxt.setFont(font);
        nextBtnTxt.setString(">");
        nextBtnTxt.setCharacterSize(20);
        nextBtnTxt.setFillColor(sf::Color::White);
        
        routeCounterTxt.setFont(font);
        routeCounterTxt.setCharacterSize(18);
        routeCounterTxt.setFillColor(sf::Color::White);
        
        // Setup results area
        resultsArea.setFillColor(sf::Color(0, 0, 0, 80));
        resultsArea.setOutlineColor(sf::Color(0, 255, 200));
        resultsArea.setOutlineThickness(1.f);
        
        resultHeader.setFont(font);
        resultHeader.setString("AVAILABLE ROUTES");
        resultHeader.setCharacterSize(14);
        resultHeader.setFillColor(sf::Color(0, 255, 200));
        
        resultBody.setFont(font);
        resultBody.setString("Select origin, destination,\nand departure date");
        resultBody.setCharacterSize(14);
        resultBody.setFillColor(sf::Color::White);
        resultBody.setLineSpacing(1.3f);
    }

    void updatePositions(float panelX, float winH) {
        float startY = 120.f;
        fieldOrigin.setPosition(panelX + 30, startY);
        originLabel.setPosition(panelX + 40, startY + 10);
        fieldDest.setPosition(panelX + 30, startY + 50);
        destLabel.setPosition(panelX + 40, startY + 60);
        dateField.setPosition(panelX + 30, startY + 100);
        dateDisplay.setPosition(panelX + 40, startY + 110);
        
        directPathsBtn.setPosition(panelX + 30, startY + 150);
        directPathsTxt.setPosition(panelX + 50, startY + 165);
        connectedPathsBtn.setPosition(panelX + 200, startY + 150);
        connectedPathsTxt.setPosition(panelX + 210, startY + 165);
        
        bookBtn.setPosition(panelX + 30, startY + 210);
        bookBtnTxt.setPosition(panelX + 70, startY + 225);
        
        showSubgraphBtn.setPosition(panelX + 30, startY + 270);
        showSubgraphBtnTxt.setPosition(panelX + 70, startY + 285);
        
        // Navigation buttons
        prevBtn.setPosition(panelX + 30, winH - 230);
        nextBtn.setPosition(panelX + 320, winH - 230);
        prevBtnTxt.setPosition(panelX + 45, winH - 225);
        nextBtnTxt.setPosition(panelX + 335, winH - 225);
        routeCounterTxt.setPosition(panelX + 100, winH - 225);
        
        resultsArea.setPosition(panelX + 20, winH - 180);
        resultHeader.setPosition(panelX + 35, winH - 180 + 15);
        resultBody.setPosition(panelX + 35, winH - 180 + 50);
    }

    void handleMouseWheel(float delta, const Graph& graph) {
        if (selectingOrigin || selectingDest) {
            int availableItems = 0;
            for (int i = 0; i < graph.size; i++) {
                if ((selectingOrigin && i == selectedDestIndex) || 
                    (selectingDest && i == selectedOriginIndex)) continue;
                availableItems++;
            }
            int maxOffset = (availableItems > 6) ? (availableItems - 6) : 0;
            
            if (delta > 0) originDestListOffset--;
            else originDestListOffset++;
            if (originDestListOffset < 0) originDestListOffset = 0;
            if (originDestListOffset > maxOffset) originDestListOffset = maxOffset;
        }
        else if (selectingDate) {
            int maxOffset = (availableDates.size() > 6) ? (availableDates.size() - 6) : 0;
            if (delta > 0) dateListOffset--;
            else dateListOffset++;
            if (dateListOffset < 0) dateListOffset = 0;
            if (dateListOffset > maxOffset) dateListOffset = maxOffset;
        }
    }

    void updateAvailableDates(Graph& graph) {
        availableDates.clear();
        // Use vector to track unique dates (no built-in structures)
        Vector<std::string> dateList;
        
        // If origin and destination are selected, show dates for both direct and connected routes
        if (selectedOriginIndex != -1 && selectedDestIndex != -1) {
            // Check direct routes
            for (int i = 0; i < graph.size; i++) {
                if (i != selectedOriginIndex) continue;
                LinkedList<Route>::Node* node = graph.vertices[i].routes.head;
                while (node != nullptr) {
                    Route& route = node->data;
                    int destIdx = graph.findPort(route.dest.name);
                    if (destIdx == selectedDestIndex) {
                        // Check if date already exists
                        bool exists = false;
                        for (const std::string& d : dateList) {
                            if (d == route.date) {
                                exists = true;
                                break;
                            }
                        }
                        if (!exists) {
                            dateList.push_back(route.date);
                        }
                    }
                    node = node->next;
                }
            }
            
            // For connected paths, we only need dates from routes starting at origin
            // Any connected path will start with one of these routes, so collect all dates
            // from all routes starting at the origin (we already have direct routes above)
            // This is much more efficient than exploring all paths
            LinkedList<Route>::Node* originRouteNode = graph.vertices[selectedOriginIndex].routes.head;
            while (originRouteNode != nullptr) {
                Route& route = originRouteNode->data;
                // Check if date already exists
                bool exists = false;
                for (const std::string& d : dateList) {
                    if (d == route.date) {
                        exists = true;
                        break;
                    }
                }
                if (!exists) {
                    dateList.push_back(route.date);
                }
                originRouteNode = originRouteNode->next;
            }
        } else {
            // If only origin is selected, show dates from that origin
            if (selectedOriginIndex != -1) {
                LinkedList<Route>::Node* node = graph.vertices[selectedOriginIndex].routes.head;
                while (node != nullptr) {
                    // Check if date already exists
                    bool exists = false;
                    for (const std::string& d : dateList) {
                        if (d == node->data.date) {
                            exists = true;
                            break;
                        }
                    }
                    if (!exists) {
                        dateList.push_back(node->data.date);
                    }
                    node = node->next;
                }
            } else {
                // Show all dates from all routes
                for (int i = 0; i < graph.size; i++) {
                    LinkedList<Route>::Node* node = graph.vertices[i].routes.head;
                    while (node != nullptr) {
                        // Check if date already exists
                        bool exists = false;
                        for (const std::string& d : dateList) {
                            if (d == node->data.date) {
                                exists = true;
                                break;
                            }
                        }
                        if (!exists) {
                            dateList.push_back(node->data.date);
                        }
                        node = node->next;
                    }
                }
            }
        }
        
        availableDates = dateList;
        // Simple bubble sort (no built-in sort)
        for (int i = 0; i < availableDates.size(); i++) {
            for (int j = i + 1; j < availableDates.size(); j++) {
                if (availableDates[i] > availableDates[j]) {
                    std::string temp = availableDates[i];
                    availableDates[i] = availableDates[j];
                    availableDates[j] = temp;
                }
            }
        }
    }

    bool handleClick(Graph& graph,
                    const sf::Vector2f& mouseGlobal,
                    float panelX,
                    float winH,
                    const sf::Font& /*font*/,
                    PathFinding::PathResult*& currentPathResult,
                    std::string& resultTextString,
                    bool& panelShouldClose) {
        // Handle origin/destination selection
        if (selectingOrigin || selectingDest) {
            return handleOriginDestSelectionClick(graph, mouseGlobal, panelX);
        }
        // Handle date selection
        if (selectingDate) {
            return handleDateSelectionClick(mouseGlobal, panelX);
        }
        // Handle field clicks
        else if (fieldOrigin.getGlobalBounds().contains(mouseGlobal)) {
            selectingOrigin = true;
            selectingDest = selectingDate = false;
            return true;
        }
        else if (fieldDest.getGlobalBounds().contains(mouseGlobal)) {
            selectingDest = true;
            selectingOrigin = selectingDate = false;
            return true;
        }
        else if (dateField.getGlobalBounds().contains(mouseGlobal)) {
            updateAvailableDates(graph);
            selectingDate = true;
            selectingOrigin = selectingDest = false;
            return true;
        }
        // Handle option buttons
        else if (directPathsBtn.getGlobalBounds().contains(mouseGlobal)) {
            if (selectedOriginIndex != -1 && selectedDestIndex != -1 && !departureDate.empty()) {
                findDirectPaths(graph, currentPathResult);
                updateCurrentPathResult(currentPathResult);
            }
            return true;
        }
        else if (connectedPathsBtn.getGlobalBounds().contains(mouseGlobal)) {
            if (selectedOriginIndex != -1 && selectedDestIndex != -1 && !departureDate.empty()) {
                findConnectedPaths(graph, currentPathResult);
                updateCurrentPathResult(currentPathResult);
            }
            return true;
        }
        // Handle book button
        else if (bookBtn.getGlobalBounds().contains(mouseGlobal)) {
            if (availableRoutes.size() > 0 && currentRouteIndex >= 0) {
                bookRoute(graph, currentPathResult, resultTextString);
                return true;
            }
        }
        // Handle show subgraph button
        else if (showSubgraphBtn.getGlobalBounds().contains(mouseGlobal)) {
            if (availableRoutes.size() > 0 && currentRouteIndex >= 0 && currentPathResult && currentPathResult->found) {
                showSubgraph = true;
                panelShouldClose = true;
                return true;
            }
        }
        // Handle navigation buttons
        if (availableRoutes.size() > 0) {
            float navY = winH - 230;
            sf::FloatRect prevRect(panelX + 30, navY, 50, 40);
            sf::FloatRect nextRect(panelX + 320, navY, 50, 40);
            
            if (prevRect.contains(mouseGlobal)) {
                navigatePrevious(currentPathResult, resultTextString);
                return true;
            }
            else if (nextRect.contains(mouseGlobal)) {
                navigateNext(currentPathResult, resultTextString);
                return true;
            }
        }
        
        return false;
    }

    void findDirectPaths(Graph& graph, PathFinding::PathResult*& currentPathResult) {
        // Clean up old routes (but don't delete if currentPathResult points to them)
        for (auto* route : availableRoutes) {
            if (route != currentPathResult) {
                delete route;
            }
        }
        availableRoutes.clear();
        // Reset currentPathResult if it was in availableRoutes
        currentPathResult = nullptr;
        
        showingDirectPaths = true;
        
        // Find direct routes (single route segment)
        for (int i = 0; i < graph.size; i++) {
            if (i != selectedOriginIndex) continue;
            
            LinkedList<Route>::Node* node = graph.vertices[i].routes.head;
            while (node != nullptr) {
                Route& route = node->data;
                int destIdx = graph.findPort(route.dest.name);
                
                if (destIdx == selectedDestIndex && route.date == departureDate) {
                    // Create a path result for this direct route
                    PathFinding::PathResult* result = new PathFinding::PathResult();
                    result->found = true;
                    result->path.insertEnd(selectedOriginIndex);
                    result->path.insertEnd(selectedDestIndex);
                    result->routes.insertEnd(route);
                    result->totalCost = route.cost;
                    
                    // Calculate time
                    long long dep = TimeUtils::toAbsoluteMinutes(route.date, route.deptTime);
                    long long arr = TimeUtils::toAbsoluteMinutes(route.date, route.arrTime);
                    if (arr < dep) arr += 24 * 60;
                    result->totalTime = (int)(arr - dep);
                    
                    // Check if available (not booked)
                    if (BookingSystem::isRouteAvailable(result, departureDate)) {
                        availableRoutes.push_back(result);
                    } else {
                        delete result;
                    }
                }
                node = node->next;
            }
        }
        
        // Note: Routes are already filtered for availability when added above
        // No need to filter again as we already check isRouteAvailable before adding
        
        currentRouteIndex = -1;
        if (availableRoutes.size() > 0) {
            currentRouteIndex = 0;
        }
    }

    void findConnectedPaths(Graph& graph, PathFinding::PathResult*& currentPathResult) {
        // Clean up old routes (but don't delete if currentPathResult points to them)
        for (int i = 0; i < availableRoutes.size(); i++) {
            if (availableRoutes[i] != currentPathResult) {
                delete availableRoutes[i];
            }
        }
        availableRoutes.clear();
        // Reset currentPathResult if it was in availableRoutes
        currentPathResult = nullptr;
        
        showingDirectPaths = false;
        
        // Use BFS to find multiple connected paths
        struct PathState {
            int current;
            Vector<int> path;
            Vector<Route> routes;
            int cost;
            long long arrivalTime;
            
            PathState() : current(-1), cost(0), arrivalTime(0) {}
        };
        
        const int MAX_PATHS = 15;
        const int MAX_DEPTH = 8;
        
        Vector<PathState> queue;
        PathState initial;
        initial.current = selectedOriginIndex;
        initial.path.push_back(selectedOriginIndex);
        initial.arrivalTime = 0;
        queue.push_back(initial);
        
        Vector<PathFinding::PathResult*> allPaths;
        
        while (queue.size() > 0 && allPaths.size() < MAX_PATHS) {
            PathState state = queue[0];
            queue.erase(0);
            
            if (state.current == selectedDestIndex && state.path.size() > 2) {
                // Connected paths must have more than 2 nodes (at least origin -> intermediate -> destination)
                // This excludes direct paths which have exactly 2 nodes
                // Check if first route matches departure date
                if (state.routes.size() > 1 && state.routes[0].date == departureDate) {
                    // Create a new PathResult for this path
                    PathFinding::PathResult* result = new PathFinding::PathResult();
                    result->found = true;
                    result->totalCost = state.cost;
                    result->totalTime = 0;
                    
                    // PathResult constructor already initializes empty LinkedLists
                    // Copy path nodes
                    for (int i = 0; i < state.path.size(); i++) {
                        result->path.insertEnd(state.path[i]);
                    }
                    
                    // Copy route nodes
                    for (int i = 0; i < state.routes.size(); i++) {
                        result->routes.insertEnd(state.routes[i]);
                    }
                    
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
                }
                continue;
            }
            
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
                
                // Check if already visited in this path
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
                
                // Check if first route matches departure date
                if (state.path.size() == 1 && route.date != departureDate) {
                    routeNode = routeNode->next;
                    continue;
                }
                
                // Check timing constraints
                long long depAbs = TimeUtils::toAbsoluteMinutes(route.date, route.deptTime);
                long long arrAbs = TimeUtils::toAbsoluteMinutes(route.date, route.arrTime);
                if (arrAbs < depAbs) arrAbs += 24 * 60;
                
                bool timeValid = true;
                if (state.current != selectedOriginIndex) {
                    if (state.arrivalTime > depAbs - 60) {
                        timeValid = false;
                    }
                }
                
                if (timeValid) {
                    // Create a new state (copy constructor)
                    PathState newState;
                    newState.current = destIndex;
                    newState.path = state.path;  // Copy the vector
                    newState.path.push_back(destIndex);
                    newState.routes = state.routes;  // Copy the vector
                    newState.routes.push_back(route);
                    newState.cost = state.cost + route.cost;
                    
                    // Add layover fee if waiting > 12 hours
                    if (state.current != selectedOriginIndex) {
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
        
        // Filter by availability (not booked) and avoid duplicates
        Vector<std::string> seenPathKeys;  // Track paths we've seen (no built-in structures)
        for (int i = 0; i < allPaths.size(); i++) {
            PathFinding::PathResult* path = allPaths[i];
            if (!BookingSystem::isRouteAvailable(path, departureDate)) {
                delete path;
                continue;
            }
            
            // Create a unique identifier for this path to avoid duplicates
            std::string pathKey = "";
            for (int i = 0; i < path->path.getSize(); i++) {
                pathKey += std::to_string(path->path.get(i)) + ",";
            }
            pathKey += "|";
            for (int i = 0; i < path->routes.getSize(); i++) {
                Route r = path->routes.get(i);
                pathKey += r.startPoint.name + "->" + r.dest.name + ":" + r.date + ":" + r.deptTime + ",";
            }
            
            // Check if we've seen this path before (linear search)
            bool isDuplicate = false;
            for (const std::string& key : seenPathKeys) {
                if (key == pathKey) {
                    isDuplicate = true;
                    break;
                }
            }
            
            // Only add if we haven't seen this exact path before
            if (!isDuplicate) {
                seenPathKeys.push_back(pathKey);
                availableRoutes.push_back(path);
            } else {
                delete path;  // Duplicate path, delete it
            }
        }
        
        currentRouteIndex = -1;
        if (availableRoutes.size() > 0) {
            currentRouteIndex = 0;
        }
    }
    
    void updateCurrentPathResult(PathFinding::PathResult*& currentPathResult) {
        if (availableRoutes.size() > 0 && currentRouteIndex >= 0 && 
            currentRouteIndex < availableRoutes.size()) {
            // Just set to the current route - don't delete anything
            // currentPathResult should already be nullptr from findDirectPaths/findConnectedPaths
            currentPathResult = availableRoutes[currentRouteIndex];
        }
    }

    void bookRoute(Graph& /*graph*/, PathFinding::PathResult*& currentPathResult, std::string& resultTextString) {
        if (currentRouteIndex < 0 || currentRouteIndex >= availableRoutes.size()) {
            return;
        }
        
        PathFinding::PathResult* selectedRoute = availableRoutes[currentRouteIndex];
        
        // Add to booking system (this creates a deep copy, so selectedRoute is still valid)
        BookingSystem::addBooking(selectedOriginIndex, selectedDestIndex, 
                                  departureDate, selectedRoute);
        
        // Check if currentPathResult points to the route we're about to remove
        bool wasCurrentPath = (currentPathResult == selectedRoute);
        
        // Remove from available routes vector
        // Note: We don't delete selectedRoute here because:
        // 1. It's been deep-copied to BookingSystem
        // 2. It might still be referenced by currentPathResult
        // 3. It will be properly deleted when availableRoutes is cleared in cleanup()
        availableRoutes.erase(currentRouteIndex);
        
        // Update current index
        if (currentRouteIndex >= availableRoutes.size()) {
            currentRouteIndex = availableRoutes.size() - 1;
        }
        if (currentRouteIndex < 0 && availableRoutes.size() > 0) {
            currentRouteIndex = 0;
        }
        
        // Update current path result
        if (wasCurrentPath) {
            // The route we booked was the current one, so update to next available
            if (availableRoutes.size() > 0 && currentRouteIndex >= 0 && 
                currentRouteIndex < availableRoutes.size()) {
                currentPathResult = availableRoutes[currentRouteIndex];
            } else if (availableRoutes.size() > 0) {
                currentRouteIndex = 0;
                currentPathResult = availableRoutes[0];
            } else {
                // No more routes available
                currentPathResult = nullptr;
            }
        } else {
            // The booked route was not the current one, so currentPathResult is still valid
            // But we need to make sure it's still in availableRoutes
            if (currentPathResult) {
                bool stillInAvailable = false;
                for (int i = 0; i < availableRoutes.size(); i++) {
                    if (availableRoutes[i] == currentPathResult) {
                        stillInAvailable = true;
                        break;
                    }
                }
                if (!stillInAvailable) {
                    // This shouldn't happen, but if it does, set to first available
                    if (availableRoutes.size() > 0) {
                        currentRouteIndex = 0;
                        currentPathResult = availableRoutes[0];
                    } else {
                        currentPathResult = nullptr;
                    }
                }
            }
        }
        
        // Update result text
        resultTextString = "Route Booked!";
    }

    void navigatePrevious(PathFinding::PathResult*& currentPathResult,
                         std::string& resultTextString) {
        if (availableRoutes.size() == 0 || currentRouteIndex <= 0) return;
        
        currentRouteIndex--;
        // currentPathResult should always be one of the routes in availableRoutes
        // so we don't need to delete it, just update the pointer
        currentPathResult = availableRoutes[currentRouteIndex];
        resultTextString = showingDirectPaths ? "Direct Route" : "Connected Route";
    }

    void navigateNext(PathFinding::PathResult*& currentPathResult,
                     std::string& resultTextString) {
        if (availableRoutes.size() == 0 || 
            currentRouteIndex >= availableRoutes.size() - 1) return;
        
        currentRouteIndex++;
        // currentPathResult should always be one of the routes in availableRoutes
        // so we don't need to delete it, just update the pointer
        currentPathResult = availableRoutes[currentRouteIndex];
        resultTextString = showingDirectPaths ? "Direct Route" : "Connected Route";
    }

    void reset() {
        selectingOrigin = selectingDest = selectingDate = false;
        originDestListOffset = 0;
        dateListOffset = 0;
        currentRouteIndex = -1;
        showingDirectPaths = false;
    }

    void cleanup() {
        for (int i = 0; i < availableRoutes.size(); i++) {
            delete availableRoutes[i];
        }
        availableRoutes.clear();
    }

private:
    bool handleDateSelectionClick(const sf::Vector2f& mouseGlobal, float panelX) {
        int const visibleItems = 6;
        float itemH = 30.f;
        float startY = dateField.getPosition().y + 45;
        
        bool clickedItem = false;
        int visibleIndex = 0;
        int skip = 0;
        
        for (int i = 0; i < availableDates.size(); i++) {
            if (skip < dateListOffset) { skip++; continue; }
            if (visibleIndex >= visibleItems) break;
            
            sf::FloatRect itemRect(panelX + 30, startY + visibleIndex * (itemH + 2), 340, itemH);
            if (itemRect.contains(mouseGlobal)) {
                departureDate = availableDates[i];
                selectingDate = false;
                clickedItem = true;
                break;
            }
            visibleIndex++;
        }
        
        if (!clickedItem) {
            selectingDate = false;
        }
        return true;
    }
    
    void drawDateDropdown(sf::RenderWindow& window,
                         const sf::Font& font,
                         const sf::Vector2f& mouseGlobal,
                         float panelX) {
        sf::RectangleShape dropBg(sf::Vector2f(340, 200));
        float yPosition = dateField.getPosition().y + 45;
        dropBg.setPosition(panelX + 30, yPosition);
        dropBg.setFillColor(sf::Color(40, 50, 60));
        dropBg.setOutlineColor(sf::Color::Cyan);
        dropBg.setOutlineThickness(1);
        window.draw(dropBg);
        
        int visibleIndex = 0;
        int skip = 0;
        for (int i = 0; i < availableDates.size(); i++) {
            if (skip < dateListOffset) { skip++; continue; }
            if (visibleIndex >= 6) break;
            
            sf::Text itemTxt(availableDates[i], font, 14);
            itemTxt.setPosition(panelX + 40, yPosition + visibleIndex * 32 + 5);
            itemTxt.setFillColor(sf::Color::White);
            
            sf::FloatRect itemRect(panelX + 30, yPosition + visibleIndex * 32, 340, 32);
            if (itemRect.contains(mouseGlobal)) {
                sf::RectangleShape highlight(sf::Vector2f(338, 32));
                highlight.setPosition(panelX + 31, yPosition + visibleIndex * 32);
                highlight.setFillColor(sf::Color(0, 100, 150));
                window.draw(highlight);
            }
            
            window.draw(itemTxt);
            visibleIndex++;
        }
    }
    
    bool handleOriginDestSelectionClick(const Graph& graph,
                                       const sf::Vector2f& mouseGlobal,
                                       float panelX) {
        int const visibleItems = 6;
        float itemH = 30.f;
        float startY = (selectingOrigin) ? fieldOrigin.getPosition().y + 45 : fieldDest.getPosition().y + 45;
        
        bool clickedItem = false;
        int visibleIndex = 0;
        int skip = 0;
        
        for (int i = 0; i < graph.size; i++) {
            if (selectingOrigin && i == selectedDestIndex) continue;
            if (selectingDest && i == selectedOriginIndex) continue;
            
            if (skip < originDestListOffset) { skip++; continue; }
            if (visibleIndex >= visibleItems) break;
            
            sf::FloatRect itemRect(panelX + 30, startY + visibleIndex * (itemH + 2), 340, itemH);
            if (itemRect.contains(mouseGlobal)) {
                if (selectingOrigin) {
                    selectedOriginIndex = i;
                } else {
                    selectedDestIndex = i;
                }
                selectingOrigin = selectingDest = false;
                clickedItem = true;
                break;
            }
            visibleIndex++;
        }
        
        if (!clickedItem) {
            selectingOrigin = selectingDest = false;
        }
        return true;
    }

public:
    void draw(sf::RenderWindow& window,
             const Graph& graph,
             const sf::Font& font,
             const sf::Vector2f& mouseGlobal,
             float panelX,
             float /*winH*/,
             PathFinding::PathResult* currentPathResult,
             const std::string& /*resultTextString*/) {
        // Draw fields
        window.draw(fieldOrigin);
        window.draw(fieldDest);
        window.draw(dateField);
        
        // Update and draw origin label
        if (selectedOriginIndex != -1) {
            originLabel.setString(graph.vertices[selectedOriginIndex].port.name);
            originLabel.setFillColor(sf::Color::White);
        } else {
            originLabel.setString("Select Origin...");
            originLabel.setFillColor(sf::Color(150, 150, 150));
        }
        window.draw(originLabel);
        
        // Update and draw destination label
        if (selectedDestIndex != -1) {
            destLabel.setString(graph.vertices[selectedDestIndex].port.name);
            destLabel.setFillColor(sf::Color::White);
        } else {
            destLabel.setString("Select Destination...");
            destLabel.setFillColor(sf::Color(150, 150, 150));
        }
        window.draw(destLabel);
        
        // Update and draw date display
        if (departureDate.empty()) {
            dateDisplay.setString("Select Date...");
            dateDisplay.setFillColor(sf::Color(150, 150, 150));
        } else {
            dateDisplay.setString(departureDate);
            dateDisplay.setFillColor(sf::Color::White);
        }
        window.draw(dateDisplay);
        
        // Draw selection indicator
        if (selectingDate) {
            dateField.setOutlineColor(sf::Color(0, 255, 200));
            dateField.setOutlineThickness(2.f);
        } else {
            dateField.setOutlineColor(sf::Color(120, 120, 120));
            dateField.setOutlineThickness(1.f);
        }
        
        // Draw option buttons
        bool canSearch = (selectedOriginIndex != -1 && selectedDestIndex != -1 && !departureDate.empty());
        directPathsBtn.setFillColor((canSearch && isHovering(directPathsBtn, mouseGlobal)) ? 
                                    btnHover : (canSearch ? btnNormal : sf::Color(30, 30, 30)));
        connectedPathsBtn.setFillColor((canSearch && isHovering(connectedPathsBtn, mouseGlobal)) ? 
                                      btnHover : (canSearch ? btnNormal : sf::Color(30, 30, 30)));
        window.draw(directPathsBtn);
        window.draw(directPathsTxt);
        window.draw(connectedPathsBtn);
        window.draw(connectedPathsTxt);
        
        // Draw book button
        bool canBook = (availableRoutes.size() > 0 && currentRouteIndex >= 0);
        bookBtn.setFillColor((canBook && isHovering(bookBtn, mouseGlobal)) ? 
                            btnHover : (canBook ? btnNormal : sf::Color(30, 30, 30)));
        window.draw(bookBtn);
        window.draw(bookBtnTxt);
        
        // Draw show subgraph button
        bool canShowSubgraph = (availableRoutes.size() > 0 && currentRouteIndex >= 0 && currentPathResult && currentPathResult->found);
        showSubgraphBtn.setFillColor((canShowSubgraph && isHovering(showSubgraphBtn, mouseGlobal)) ? 
                                    btnHover : (canShowSubgraph ? btnNormal : sf::Color(30, 30, 30)));
        window.draw(showSubgraphBtn);
        window.draw(showSubgraphBtnTxt);
        
        // Draw navigation buttons if routes are available
        if (availableRoutes.size() > 0) {
            prevBtn.setFillColor((currentRouteIndex > 0 && isHovering(prevBtn, mouseGlobal)) ? 
                                btnHover : (currentRouteIndex > 0 ? btnNormal : sf::Color(30, 30, 30)));
            nextBtn.setFillColor((currentRouteIndex < availableRoutes.size() - 1 && 
                                 isHovering(nextBtn, mouseGlobal)) ? 
                                btnHover : (currentRouteIndex < availableRoutes.size() - 1 ? 
                                btnNormal : sf::Color(30, 30, 30)));
            
            window.draw(prevBtn);
            window.draw(prevBtnTxt);
            window.draw(nextBtn);
            window.draw(nextBtnTxt);
            
            // Draw route counter
            std::stringstream ss;
            ss << (currentRouteIndex + 1) << " / " << availableRoutes.size();
            routeCounterTxt.setString(ss.str());
            window.draw(routeCounterTxt);
        }
        
        // Draw dropdowns
        if (selectingOrigin || selectingDest) {
            drawOriginDestDropdown(window, graph, font, mouseGlobal, panelX);
        } else if (selectingDate) {
            drawDateDropdown(window, font, mouseGlobal, panelX);
        } else {
            // Draw results
            window.draw(resultsArea);
            window.draw(resultHeader);
            
            std::stringstream ss;
            if (availableRoutes.size() > 0 && currentRouteIndex >= 0) {
                PathFinding::PathResult* result = availableRoutes[currentRouteIndex];
                ss << (showingDirectPaths ? "Direct" : "Connected") << " Route ";
                ss << (currentRouteIndex + 1) << " of " << availableRoutes.size() << "\n";
                ss << "Cost: $" << result->totalCost << "\n";
                ss << "Stops: " << result->path.getSize() - 2 << "\n";
                ss << "Date: " << departureDate << "\n";
                ss << "Path:\n";
                
                std::string pathStr = "";
                int lineLength = 0;
                const int maxLineLength = 35;
                
                for (int i = 0; i < result->path.getSize(); i++) {
                    std::string portName = graph.vertices[result->path.get(i)].port.name;
                    
                    if (i > 0) {
                        pathStr += " -> ";
                        lineLength += 4;
                    }
                    
                    if (lineLength + portName.length() > maxLineLength && i > 0) {
                        pathStr += "\n";
                        lineLength = 0;
                    }
                    
                    pathStr += portName;
                    lineLength += portName.length();
                }
                ss << pathStr;
            } else if (availableRoutes.size() == 0 && selectedOriginIndex != -1 && 
                      selectedDestIndex != -1 && !departureDate.empty()) {
                ss << "No available routes found\nfor this date.\nRoutes may be booked.";
            } else {
                ss << "Select origin, destination,\nand departure date";
            }
            resultBody.setString(ss.str());
            window.draw(resultBody);
        }
    }

private:
    void drawOriginDestDropdown(sf::RenderWindow& window,
                               const Graph& graph,
                               const sf::Font& font,
                               const sf::Vector2f& mouseGlobal,
                               float panelX) {
        sf::RectangleShape dropBg(sf::Vector2f(340, 200));
        float yPosition = (selectingOrigin) ? fieldOrigin.getPosition().y + 45 : fieldDest.getPosition().y + 45;
        dropBg.setPosition(panelX + 30, yPosition);
        dropBg.setFillColor(sf::Color(40, 50, 60));
        dropBg.setOutlineColor(sf::Color::Cyan);
        dropBg.setOutlineThickness(1);
        window.draw(dropBg);
        
        int visibleIndex = 0;
        int skip = 0;
        for (int i = 0; i < graph.size; i++) {
            if (selectingOrigin && i == selectedDestIndex) continue;
            if (selectingDest && i == selectedOriginIndex) continue;
            
            if (skip < originDestListOffset) { skip++; continue; }
            if (visibleIndex >= 6) break;
            
            sf::Text itemTxt(graph.vertices[i].port.name, font, 14);
            itemTxt.setPosition(panelX + 40, yPosition + visibleIndex * 32 + 5);
            itemTxt.setFillColor(sf::Color::White);
            
            sf::FloatRect itemRect(panelX + 30, yPosition + visibleIndex * 32, 340, 32);
            if (itemRect.contains(mouseGlobal)) {
                sf::RectangleShape highlight(sf::Vector2f(338, 32));
                highlight.setPosition(panelX + 31, yPosition + visibleIndex * 32);
                highlight.setFillColor(sf::Color(0, 100, 150));
                window.draw(highlight);
            }
            
            window.draw(itemTxt);
            visibleIndex++;
        }
    }
};

#endif

