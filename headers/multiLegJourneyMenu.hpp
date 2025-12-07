#ifndef MULTILEGJOURNEYMENU_HPP
#define MULTILEGJOURNEYMENU_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include "Graph.hpp"
#include "pathFinding.h"
#include "uiHelpers.hpp"
#include "vector.h"
#include "queue.h"
#include "linkedList.h"
#include "timeUtils.h"

struct MultiLegJourneyMenu {
    // Selection fields
    sf::RectangleShape fieldOrigin;
    sf::RectangleShape fieldDest;
    sf::Text originText;
    sf::Text destText;
    
    // Buttons
    sf::RectangleShape trackBtn;
    sf::Text trackBtnText;
    
    // Selection state
    bool selectingOrigin;
    bool selectingDest;
    int selectedOriginIndex;
    int selectedDestIndex;
    int originListOffset;
    int destListOffset;
    
    // Journey tracking state
    bool isTracking;
    LinkedList<int> journeyPath;  // Track the journey path
    LinkedList<Route> journeyRoutes;  // Track the routes taken
    int currentPortIndex;  // Current port waiting for click
    bool showModal;  // Whether to show port selection modal
    Vector<int> modalPorts;  // Ports to show in modal
    
    // Pre-calculated paths from origin to destination
    Vector<PathFinding::PathResult*> allPaths;
    
    // Current path result for drawing
    PathFinding::PathResult* currentPathResult;
    
    // Route information for each accessible port
    struct PortRouteInfo {
        int portIndex;
        Route route;  // Route from current port to this port
        int routeCost;
        int layoverDuration;  // Minutes
        int layoverCost;  // Port charge
        
        PortRouteInfo() : portIndex(-1), routeCost(0), layoverDuration(0), 
                         layoverCost(0) {}
    };
    Vector<PortRouteInfo> modalPortInfo;  // Detailed info for each modal port
    
    // Modal UI
    sf::RectangleShape modalBg;
    sf::Text modalTitle;
    Vector<sf::RectangleShape> modalPortButtons;
    Vector<sf::Text> modalPortTexts;
    sf::RectangleShape modalCloseBtn;
    sf::Text modalCloseText;
    
    // Alert
    bool showAlert;
    sf::Text alertText;
    float alertTimer;
    
    // Result display
    bool showResult;
    sf::Text resultText;
    sf::RectangleShape resultBg;
    sf::RectangleShape resultCloseBtn;
    sf::Text resultCloseText;
    
    // Helper pointers and window dimensions (for modal updates)
    const Graph* graphPtr;
    const sf::Font* fontPtr;
    float winH;
    float winW;
    
    sf::Color btnNormal;
    sf::Color btnHover;
    sf::Color fieldNormal;
    sf::Color fieldSelected;
    
    MultiLegJourneyMenu(const sf::Font& font)
        : fieldOrigin(sf::Vector2f(340.f, 40.f)),
          fieldDest(sf::Vector2f(340.f, 40.f)),
          trackBtn(sf::Vector2f(340.f, 50.f)),
          selectingOrigin(false),
          selectingDest(false),
          selectedOriginIndex(-1),
          selectedDestIndex(-1),
          originListOffset(0),
          destListOffset(0),
          isTracking(false),
          currentPortIndex(-1),
          showModal(false),
          currentPathResult(nullptr),
          graphPtr(nullptr),
          fontPtr(nullptr),
          winH(900.f),
          winW(1600.f),
          showAlert(false),
          alertTimer(0.0f),
          showResult(false),
          resultBg(sf::Vector2f(500.f, 400.f)),
          resultCloseBtn(sf::Vector2f(100.f, 40.f)),
          btnNormal(50, 60, 75),
          btnHover(70, 80, 100),
          fieldNormal(30, 40, 50),
          fieldSelected(50, 70, 90) {
        
        fieldOrigin.setFillColor(fieldNormal);
        fieldDest.setFillColor(fieldNormal);
        trackBtn.setFillColor(btnNormal);
        
        originText.setFont(font);
        originText.setString("Select Origin...");
        originText.setCharacterSize(14);
        originText.setFillColor(sf::Color(150, 150, 150));
        
        destText.setFont(font);
        destText.setString("Select Destination...");
        destText.setCharacterSize(14);
        destText.setFillColor(sf::Color(150, 150, 150));
        
        trackBtnText.setFont(font);
        trackBtnText.setString("Track Multi-leg Journey");
        trackBtnText.setCharacterSize(16);
        trackBtnText.setFillColor(sf::Color::White);
        
        // Modal setup
        modalBg.setFillColor(sf::Color(30, 40, 55, 250));
        modalBg.setOutlineThickness(3.f);
        modalBg.setOutlineColor(sf::Color(100, 150, 200));
        
        modalTitle.setFont(font);
        modalTitle.setCharacterSize(15);
        modalTitle.setFillColor(sf::Color::White);
        modalTitle.setStyle(sf::Text::Bold);
        
        modalCloseBtn.setSize(sf::Vector2f(100.f, 40.f));
        modalCloseBtn.setFillColor(sf::Color(200, 50, 50));
        modalCloseText.setFont(font);
        modalCloseText.setString("Close");
        modalCloseText.setCharacterSize(14);
        modalCloseText.setFillColor(sf::Color::White);
        
        // Alert setup
        alertText.setFont(font);
        alertText.setCharacterSize(16);
        alertText.setFillColor(sf::Color(255, 100, 100));
        alertText.setStyle(sf::Text::Bold);
        
        // Result setup
        resultBg.setFillColor(sf::Color(20, 30, 45, 250));
        resultBg.setOutlineThickness(3.f);
        resultBg.setOutlineColor(sf::Color(0, 200, 100));
        
        resultText.setFont(font);
        resultText.setCharacterSize(14);
        resultText.setFillColor(sf::Color::White);
        
        resultCloseBtn.setFillColor(sf::Color(0, 150, 200));
        resultCloseText.setFont(font);
        resultCloseText.setString("Close");
        resultCloseText.setCharacterSize(14);
        resultCloseText.setFillColor(sf::Color::White);
    }
    
    void updatePositions(float panelX, float winH, float winW = 1600.f) {
        this->winW = winW;
        fieldOrigin.setPosition(panelX + 20, 120);
        fieldDest.setPosition(panelX + 20, 180);
        trackBtn.setPosition(panelX + 20, 250);
        
        originText.setPosition(panelX + 25, 135);
        destText.setPosition(panelX + 25, 195);
        
        trackBtnText.setPosition(panelX + 60, 265);
        
        // Modal positions will be set in updateModal()
        // Alert at top center
        alertText.setPosition(winW / 2 - 200, 50);
        
        // Result centered (will be updated when shown)
        resultBg.setPosition(winW / 2 - 250, winH / 2 - 200);
        resultText.setPosition(winW / 2 - 240, winH / 2 - 180);
        resultCloseBtn.setSize(sf::Vector2f(100.f, 40.f));
        resultCloseBtn.setPosition(winW / 2 - 50, winH / 2 + 150);
        resultCloseText.setPosition(winW / 2 - 30, winH / 2 + 160);
    }
    
    void reset() {
        selectingOrigin = false;
        selectingDest = false;
        selectedOriginIndex = -1;
        selectedDestIndex = -1;
        isTracking = false;
        journeyPath.clear();
        journeyRoutes.clear();
        currentPortIndex = -1;
        showModal = false;
        modalPorts.clear();
        showAlert = false;
        alertTimer = 0.0f;
        showResult = false;
        
        // Clean up pre-calculated paths
        for (int i = 0; i < allPaths.size(); i++) {
            delete allPaths[i];
        }
        allPaths.clear();
        
        // Clean up current path result (only when leaving menu)
        if (currentPathResult) {
            delete currentPathResult;
            currentPathResult = nullptr;
        }
        
        // Reset text fields
        originText.setString("Click to select");
        originText.setFillColor(sf::Color(150, 150, 150));
        destText.setString("Click to select");
        destText.setFillColor(sf::Color(150, 150, 150));
    }
    
    void initialize() {
        // Initialize menu when opened - but don't clear the path if journey was completed
        // Only reset selection state, not the completed journey
        // This allows the path to remain visible until user leaves the menu
        if (!showResult) {
            // Only reset if no completed journey to show
            selectingOrigin = false;
            selectingDest = false;
            showModal = false;
            showAlert = false;
            alertTimer = 0.0f;
        }
    }
    
    // Calculate all paths from origin to destination using BFS
    void calculateAllPaths(Graph& graph, int origin, int destination) {
        // Clean up old paths
        for (int i = 0; i < allPaths.size(); i++) {
            delete allPaths[i];
        }
        allPaths.clear();
        
        const int MAX_PATHS = 30;
        const int MAX_DEPTH = 10;
        
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
        initial.current = origin;
        initial.path.push_back(origin);
        initial.arrivalTime = 0;
        queue.push_back(initial);
        
        while (queue.size() > 0 && allPaths.size() < MAX_PATHS) {
            PathState state = queue[0];
            queue.erase(0);
            
            // Check if we reached destination
            if (state.current == destination && state.path.size() > 1) {
                PathFinding::PathResult* result = new PathFinding::PathResult();
                result->found = true;
                
                for (int i = 0; i < state.path.size(); i++) {
                    result->path.insertEnd(state.path[i]);
                }
                
                for (int i = 0; i < state.routes.size(); i++) {
                    result->routes.insertEnd(state.routes[i]);
                }
                
                result->totalCost = state.cost;
                
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
                
                // Check timing constraints
                long long depAbs = TimeUtils::toAbsoluteMinutes(route.date, route.deptTime);
                long long arrAbs = TimeUtils::toAbsoluteMinutes(route.date, route.arrTime);
                if (arrAbs < depAbs) arrAbs += 24 * 60;
                
                bool timeValid = true;
                if (state.current != origin) {
                    if (state.arrivalTime > depAbs - 60) {
                        timeValid = false;
                    }
                }
                
                if (timeValid) {
                    PathState newState;
                    newState.current = destIndex;
                    newState.path = state.path;
                    newState.path.push_back(destIndex);
                    newState.routes = state.routes;
                    newState.routes.push_back(route);
                    newState.cost = state.cost + route.cost;
                    
                    // Add layover fee if waiting > 12 hours
                    if (state.current != origin) {
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
    }
    
    // Find all ports that appear as next step in pre-calculated paths
    Vector<PortRouteInfo> findNextPortsInPaths(const Graph& graph, int currentPort) {
        Vector<PortRouteInfo> nextPorts;
        
        // Track which ports we've seen and their route info
        Vector<int> seenPorts;
        Vector<Route> portRoutes;  // Route to reach each port
        
        // Go through all pre-calculated paths
        for (int i = 0; i < allPaths.size(); i++) {
            PathFinding::PathResult* path = allPaths[i];
            if (!path || !path->found) continue;
            
            // Find current port in this path
            int currentPos = -1;
            for (int j = 0; j < path->path.getSize(); j++) {
                if (path->path.get(j) == currentPort) {
                    currentPos = j;
                    break;
                }
            }
            
            // If current port found and not at the end
            if (currentPos >= 0 && currentPos < path->path.getSize() - 1) {
                int nextPort = path->path.get(currentPos + 1);
                
                // Check if we've already added this port
                bool alreadyAdded = false;
                for (int k = 0; k < seenPorts.size(); k++) {
                    if (seenPorts[k] == nextPort) {
                        alreadyAdded = true;
                        break;
                    }
                }
                
                // Get the route to this next port
                Route routeToNext;
                if (currentPos < path->routes.getSize()) {
                    routeToNext = path->routes.get(currentPos);
                }
                
                if (!alreadyAdded) {
                    PortRouteInfo info;
                    info.portIndex = nextPort;
                    info.route = routeToNext;
                    info.routeCost = routeToNext.cost;
                    info.layoverCost = graph.vertices[nextPort].port.portCharge;
                    
                    // Calculate layover duration
                    // If from origin, no layover. Otherwise calculate based on previous route
                    if (currentPort == selectedOriginIndex) {
                        // First leg from origin - no layover
                        info.layoverDuration = 0;
                        info.layoverCost = 0;
                    } else if (currentPos > 0 && currentPos < path->routes.getSize()) {
                        // Connecting port - calculate layover at current port
                        Route prevRoute = path->routes.get(currentPos - 1);
                        int arrivalMinutes = TimeUtils::timeToMinutes(prevRoute.arrTime);
                        int departureMinutes = TimeUtils::timeToMinutes(routeToNext.deptTime);
                        if (departureMinutes < arrivalMinutes) {
                            departureMinutes += 24 * 60;
                        }
                        info.layoverDuration = departureMinutes - arrivalMinutes;
                        if (info.layoverDuration <= 0) {
                            info.layoverCost = 0;
                        }
                    } else {
                        info.layoverDuration = 0;
                        info.layoverCost = 0;
                    }
                    
                    nextPorts.push_back(info);
                    seenPorts.push_back(nextPort);
                    portRoutes.push_back(routeToNext);
                }
            }
        }
        
        return nextPorts;
    }
    
    void startTracking(Graph& graph) {
        if (selectedOriginIndex >= 0 && selectedDestIndex >= 0) {
            // Pre-calculate all paths from origin to destination
            calculateAllPaths(graph, selectedOriginIndex, selectedDestIndex);
            
            isTracking = true;
            journeyPath.clear();
            journeyRoutes.clear();
            journeyPath.insertEnd(selectedOriginIndex);
            currentPortIndex = selectedOriginIndex;
            showModal = false;  // Don't show modal automatically, wait for port click
            
            // Initialize path result for drawing
            if (currentPathResult) {
                delete currentPathResult;
            }
            currentPathResult = new PathFinding::PathResult();
            currentPathResult->found = true;
            currentPathResult->path.insertEnd(selectedOriginIndex);
        }
    }
    
    void updateModal() {
        // Clear existing modal buttons
        modalPortButtons.clear();
        modalPortTexts.clear();
        
        if (!graphPtr) return;
        
        // Find next ports that appear in pre-calculated paths
        modalPortInfo = findNextPortsInPaths(*graphPtr, currentPortIndex);
        modalPorts.clear();
        for (int i = 0; i < modalPortInfo.size(); i++) {
            modalPorts.push_back(modalPortInfo[i].portIndex);
        }
        
        // Calculate modal dimensions first
        float buttonHeight = 60.f;  // Taller buttons to show details
        float buttonSpacing = 5.f;
        float modalPadding = 20.f;
        float titleHeight = 40.f;
        float closeBtnHeight = 50.f;
        
        float modalHeight = modalPortInfo.size() * (buttonHeight + buttonSpacing) + titleHeight + closeBtnHeight + modalPadding * 2;
        if (modalHeight > winH - 100) modalHeight = winH - 100;  // Limit height
        
        // Set modal background size and position (centered)
        modalBg.setSize(sf::Vector2f(520.f, modalHeight));
        float modalX = winW / 2 - 260;
        float modalY = winH / 2 - modalHeight / 2;
        modalBg.setPosition(modalX, modalY);
        
        // Position modal title
        modalTitle.setPosition(modalX + 10, modalY + 10);
        
        // Position close button at bottom (centered)
        float closeBtnX = modalX + modalBg.getSize().x / 2 - 50;
        float closeBtnY = modalY + modalHeight - closeBtnHeight - 10;
        modalCloseBtn.setPosition(closeBtnX, closeBtnY);
        modalCloseText.setPosition(closeBtnX + 30, closeBtnY + 10);
        
        // Calculate starting Y for buttons (after title)
        float startY = modalY + titleHeight + modalPadding;
        
        // Create buttons for each accessible port with details
        for (int i = 0; i < modalPortInfo.size(); i++) {
            sf::RectangleShape btn(sf::Vector2f(500.f, buttonHeight));
            btn.setFillColor(btnNormal);
            btn.setPosition(modalX + 10, startY + i * (buttonHeight + buttonSpacing));
            modalPortButtons.push_back(btn);
            
            // Create text with port name and details
            sf::Text text;
            text.setFont(*fontPtr);
            PortRouteInfo& info = modalPortInfo[i];
            
            std::stringstream ss;
            ss << graphPtr->vertices[info.portIndex].port.name;
            if (info.portIndex == selectedDestIndex) {
                ss << " (DESTINATION)";
            }
            ss << "\n";
            ss << "Route Cost: $" << info.routeCost;
            
            if (info.layoverDuration > 0) {
                int hours = info.layoverDuration / 60;
                int minutes = info.layoverDuration % 60;
                ss << " | Layover: " << hours << "h " << minutes << "m";
                if (info.layoverCost > 0) {
                    ss << " (Port Fee: $" << info.layoverCost << ")";
                }
            } else if (currentPortIndex == selectedOriginIndex) {
                ss << " | Direct route";
            }
            
            text.setString(ss.str());
            text.setCharacterSize(12);
            text.setFillColor(sf::Color::White);
            text.setPosition(modalX + 15, startY + i * (buttonHeight + buttonSpacing) + 5);
            text.setLineSpacing(1.3f);
            modalPortTexts.push_back(text);
        }
        
        // Update modal title text
        if (currentPortIndex >= 0 && graphPtr) {
            std::stringstream ss;
            ss << "From: " << graphPtr->vertices[currentPortIndex].port.name 
               << " - Select next port to " << graphPtr->vertices[selectedDestIndex].port.name;
            modalTitle.setString(ss.str());
        }
    }
    
    void handlePortClick(int portIndex, const Graph& graph) {
        if (!isTracking) return;
        
        // Check if this is the expected port
        if (portIndex == currentPortIndex) {
            // User clicked on the current port, show modal with accessible ports
            graphPtr = &graph;
            showModal = true;
            updateModal();
        } else {
            // Wrong port clicked, show alert
            showAlert = true;
            alertTimer = 2.0f;  // Show for 2 seconds
            std::stringstream ss;
            ss << "Please click on: " << graph.vertices[currentPortIndex].port.name;
            alertText.setString(ss.str());
        }
    }
    
    void handleModalPortSelection(int selectedPortIndex, const Graph& graph) {
        if (selectedPortIndex < 0 || selectedPortIndex >= modalPortInfo.size()) return;
        
        PortRouteInfo& info = modalPortInfo[selectedPortIndex];
        int portIdx = info.portIndex;
        
        // Find the actual route from current port to selected port in the graph
        Route actualRoute;
        bool routeFound = false;
        
        // Search for route in graph from current port to selected port
        for (int i = 0; i < graph.vertices[currentPortIndex].routes.getSize(); i++) {
            Route r = graph.vertices[currentPortIndex].routes.get(i);
            if (r.dest.name == graph.vertices[portIdx].port.name) {
                actualRoute = r;
                routeFound = true;
                break;
            }
        }
        
        // If route not found, use the one from modal info as fallback
        if (!routeFound) {
            actualRoute = info.route;
        }
        
        // Add to journey path and routes
        journeyPath.insertEnd(portIdx);
        journeyRoutes.insertEnd(actualRoute);
        
        // Update current path result for drawing
        if (currentPathResult) {
            currentPathResult->path.insertEnd(portIdx);
            currentPathResult->routes.insertEnd(actualRoute);
            currentPathResult->totalCost += info.routeCost;
            if (info.layoverCost > 0) {
                currentPathResult->totalCost += info.layoverCost;
            }
        }
        
        currentPortIndex = portIdx;
        
        // Check if destination reached
        if (portIdx == selectedDestIndex) {
            // Journey complete
            showModal = false;
            showResult = true;
            isTracking = false;  // Stop tracking
            displayResult(graph);
            // Panel will be opened automatically in main.cpp
        } else {
            // Close modal, user needs to click on the connecting port
            showModal = false;
            // Modal will reopen when user clicks on the connecting port
        }
    }
    
    void displayResult(const Graph& graph) {
        std::stringstream ss;
        ss << "Multi-leg Journey Complete!\n\n";
        ss << "Path:\n";
        
        for (int i = 0; i < journeyPath.getSize(); i++) {
            int portIdx = journeyPath.get(i);
            ss << graph.vertices[portIdx].port.name;
            if (i < journeyPath.getSize() - 1) {
                ss << " ->\n";
            }
        }
        
        ss << "\n\nTotal Legs: " << (journeyPath.getSize() - 1);
        
        // Calculate total cost
        int totalCost = 0;
        if (currentPathResult) {
            totalCost = currentPathResult->totalCost;
        }
        ss << "\nTotal Cost: $" << totalCost;
        
        resultText.setString(ss.str());
        resultText.setLineSpacing(1.5f);
    }
    
    void update(float deltaTime) {
        if (showAlert && alertTimer > 0) {
            alertTimer -= deltaTime;
            if (alertTimer <= 0) {
                showAlert = false;
            }
        }
    }
    
    // Handle modal clicks when panel is closed
    bool handleModalClick(const Graph& graph, const sf::Vector2f& mouseGlobal) {
        if (!showModal) return false;
        
        // Handle modal close button
        if (isHovering(modalCloseBtn, mouseGlobal)) {
            showModal = false;
            isTracking = false;
            return true;
        }
        
        // Check modal port buttons
        for (int i = 0; i < modalPortButtons.size(); i++) {
            if (isHovering(modalPortButtons[i], mouseGlobal)) {
                handleModalPortSelection(i, graph);
                return true;
            }
        }
        
        return false;
    }
    
    void handleClick(const Graph& graph, const sf::Vector2f& mouseGlobal, 
                    float panelX, float /*winH*/, bool& panelShouldClose) {
        if (showResult) {
            // Set button position before checking hover (since draw() hasn't been called yet)
            float resultY = 100;
            resultCloseBtn.setPosition(panelX + 120, resultY + 450);
            resultCloseText.setPosition(panelX + 150, resultY + 460);
            
            // Handle result close
            if (isHovering(resultCloseBtn, mouseGlobal)) {
                // Don't reset the path - keep it visible until user leaves menu
                showResult = false;
                // Just clear the result display, but keep path and journey data visible
                panelShouldClose = false;
            }
            return;  // Don't handle other clicks when showing result
        }
        
        if (showModal) {
            // Handle modal clicks
            if (isHovering(modalCloseBtn, mouseGlobal)) {
                showModal = false;
                isTracking = false;
                return;
            }
            
            // Check modal port buttons
            for (int i = 0; i < modalPortButtons.size(); i++) {
                if (isHovering(modalPortButtons[i], mouseGlobal)) {
                    handleModalPortSelection(i, graph);
                    return;
                }
            }
            return;
        }
        
        // Handle port selection from dropdown first (before field clicks)
        if (selectingOrigin || selectingDest) {
            // Check clicks on port list (similar to routeFindingMenu)
            float itemH = 30.f;
            float startY = (selectingOrigin) ? fieldOrigin.getPosition().y + 45 : fieldDest.getPosition().y + 45;
            int visibleItems = 8;
            
            bool clickedItem = false;
            int visibleIndex = 0;
            int skip = 0;
            int listOffset = (selectingOrigin) ? originListOffset : destListOffset;
            
            for (int i = 0; i < graph.size; i++) {
                // Skip already selected port
                if (selectingOrigin && i == selectedDestIndex) continue;
                if (selectingDest && i == selectedOriginIndex) continue;
                
                if (skip < listOffset) { skip++; continue; }
                if (visibleIndex >= visibleItems) break;
                
                sf::FloatRect itemRect(panelX + 20, startY + visibleIndex * (itemH + 2), 340, itemH);
                if (itemRect.contains(mouseGlobal)) {
                    if (selectingOrigin) {
                        selectedOriginIndex = i;
                        originText.setString(graph.vertices[i].port.name);
                        originText.setFillColor(sf::Color::White);
                        selectingOrigin = false;
                    } else {
                        selectedDestIndex = i;
                        destText.setString(graph.vertices[i].port.name);
                        destText.setFillColor(sf::Color::White);
                        selectingDest = false;
                    }
                    clickedItem = true;
                    break;
                }
                visibleIndex++;
            }
            
            // If clicked outside dropdown, close it
            if (!clickedItem) {
                selectingOrigin = false;
                selectingDest = false;
            }
            return;  // Don't process other clicks when dropdown is open
        }
        
        // Handle main menu clicks (only when dropdown is not open)
        if (isHovering(fieldOrigin, mouseGlobal)) {
            selectingOrigin = true;
            selectingDest = false;
        } else if (isHovering(fieldDest, mouseGlobal)) {
            selectingDest = true;
            selectingOrigin = false;
        } else if (isHovering(trackBtn, mouseGlobal)) {
            if (selectedOriginIndex >= 0 && selectedDestIndex >= 0) {
                startTracking(const_cast<Graph&>(graph));
                panelShouldClose = true;  // Close panel to start tracking
            }
        }
    }
    
    void handleMouseWheel(float delta, const Graph& graph) {
        // Calculate available items (excluding already selected port)
        int availableItems = 0;
        if (selectingOrigin) {
            for (int i = 0; i < graph.size; i++) {
                if (i == selectedDestIndex) continue;
                availableItems++;
            }
        } else if (selectingDest) {
            for (int i = 0; i < graph.size; i++) {
                if (i == selectedOriginIndex) continue;
                availableItems++;
            }
        }
        
        int maxOffset = (availableItems > 8) ? (availableItems - 8) : 0;
        
        if (selectingOrigin) {
            originListOffset += (delta > 0) ? -1 : 1;
            if (originListOffset < 0) originListOffset = 0;
            if (originListOffset > maxOffset) originListOffset = maxOffset;
        } else if (selectingDest) {
            destListOffset += (delta > 0) ? -1 : 1;
            if (destListOffset < 0) destListOffset = 0;
            if (destListOffset > maxOffset) destListOffset = maxOffset;
        }
    }
    
    void handleMapPortClick(int portIndex, const Graph& graph) {
        if (isTracking && !showModal) {
            handlePortClick(portIndex, graph);
        }
    }
    
    void draw(sf::RenderWindow& window, const Graph& graph, const sf::Font& font,
             const sf::Vector2f& mouseGlobal, float panelX, float winHParam, float winWParam = 1600.f) {
        
        // Store pointers for modal updates
        graphPtr = &graph;
        fontPtr = &font;
        winH = winHParam;
        winW = winWParam;
        
        if (showModal) {
            // Draw modal
            window.draw(modalBg);
            window.draw(modalTitle);
            
            // Draw port buttons
            for (int i = 0; i < modalPortButtons.size(); i++) {
                bool hover = isHovering(modalPortButtons[i], mouseGlobal);
                modalPortButtons[i].setFillColor(hover ? btnHover : btnNormal);
                window.draw(modalPortButtons[i]);
                window.draw(modalPortTexts[i]);
            }
            
            modalCloseBtn.setFillColor(isHovering(modalCloseBtn, mouseGlobal) ? btnHover : btnNormal);
            window.draw(modalCloseBtn);
            window.draw(modalCloseText);
            return;
        }
        
        // Draw main menu or result
        if (showResult) {
            // Draw result inside panel
            float resultY = 100;
            resultBg.setPosition(panelX + 10, resultY);
            resultBg.setSize(sf::Vector2f(340, 500));
            window.draw(resultBg);
            
            resultText.setPosition(panelX + 20, resultY + 20);
            window.draw(resultText);
            
            resultCloseBtn.setPosition(panelX + 120, resultY + 450);
            resultCloseText.setPosition(panelX + 150, resultY + 460);
            resultCloseBtn.setFillColor(isHovering(resultCloseBtn, mouseGlobal) ? btnHover : btnNormal);
            window.draw(resultCloseBtn);
            window.draw(resultCloseText);
        } else {
            // Draw main menu
            fieldOrigin.setFillColor(selectingOrigin ? fieldSelected : fieldNormal);
            fieldDest.setFillColor(selectingDest ? fieldSelected : fieldNormal);
            
            window.draw(fieldOrigin);
            window.draw(fieldDest);
            window.draw(originText);
            window.draw(destText);
            
            trackBtn.setFillColor(isHovering(trackBtn, mouseGlobal) ? btnHover : btnNormal);
            window.draw(trackBtn);
            window.draw(trackBtnText);
            
            // Draw alert if active
            if (showAlert) {
                window.draw(alertText);
            }
            
            // Draw port selection dropdown (matching routeFindingMenu design)
            if (selectingOrigin || selectingDest) {
                float itemH = 30.f;
                float startY = (selectingOrigin) ? fieldOrigin.getPosition().y + 45 : fieldDest.getPosition().y + 45;
                int visibleItems = 8;
                int listOffset = (selectingOrigin) ? originListOffset : destListOffset;
                
                // Draw dropdown background
                sf::RectangleShape dropdownBg(sf::Vector2f(340, visibleItems * (itemH + 2)));
                dropdownBg.setPosition(panelX + 20, startY);
                dropdownBg.setFillColor(sf::Color(20, 30, 40));
                dropdownBg.setOutlineColor(sf::Color(100, 150, 200));
                dropdownBg.setOutlineThickness(2.f);
                window.draw(dropdownBg);
                
                // Draw port items
                int visibleIndex = 0;
                int skip = 0;
                for (int i = 0; i < graph.size; i++) {
                    // Skip already selected port
                    if (selectingOrigin && i == selectedDestIndex) continue;
                    if (selectingDest && i == selectedOriginIndex) continue;
                    
                    if (skip < listOffset) { skip++; continue; }
                    if (visibleIndex >= visibleItems) break;
                    
                    float itemY = startY + visibleIndex * (itemH + 2);
                    sf::FloatRect itemRect(panelX + 20, itemY, 340, itemH);
                    
                    sf::RectangleShape itemBg(sf::Vector2f(336, itemH));
                    itemBg.setPosition(panelX + 22, itemY + 1);
                    
                    if (itemRect.contains(mouseGlobal)) {
                        itemBg.setFillColor(sf::Color(60, 80, 100));
                    } else {
                        itemBg.setFillColor(sf::Color(30, 40, 50));
                    }
                    window.draw(itemBg);
                    
                    sf::Text portName;
                    portName.setFont(font);
                    portName.setString(graph.vertices[i].port.name);
                    portName.setCharacterSize(14);
                    portName.setFillColor(sf::Color::White);
                    portName.setPosition(panelX + 30, itemY + 8);
                    window.draw(portName);
                    
                    visibleIndex++;
                }
            }
        }
    }
};

#endif

