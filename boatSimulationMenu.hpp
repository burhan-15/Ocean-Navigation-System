#ifndef BOATSIMULATIONMENU_HPP
#define BOATSIMULATIONMENU_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include "Graph.hpp"
#include "bookingSystem.hpp"
#include "uiHelpers.hpp"
#include "vector.h"
#include "queue.h"

// Structure to manage layover information
struct LayoverInfo {
    int portIndex;
    float timer;
    bool isActive;
    
    LayoverInfo() : portIndex(-1), timer(0.0f), isActive(false) {}
    LayoverInfo(int idx) : portIndex(idx), timer(0.0f), isActive(true) {}
};

struct BoatSimulationMenu {
    // Navigation buttons
    sf::RectangleShape prevBtn;
    sf::RectangleShape nextBtn;
    sf::RectangleShape runSimBtn;
    sf::Text prevText;
    sf::Text nextText;
    sf::Text runSimText;
    sf::Text routeInfoText;
    sf::Text statusText;
    
    // Current route index
    int currentRouteIndex;
    
    // Boat animation state
    sf::Sprite boatSprite;
    sf::Texture boatTexture;
    bool isAnimating;
    int currentSegment;  // Current segment in the path (0 to path.size()-2)
    float segmentProgress;  // Progress along current segment (0.0 to 1.0)
    
    // Layover management using data structure
    LayoverInfo currentLayover;  // Current active layover
    Queue<LayoverInfo> layoverQueue;  // Queue to manage upcoming layovers
    
    // Animation speed
    float boatSpeed;  // Units per second
    
    sf::Color btnNormal;
    sf::Color btnHover;
    sf::Color btnDisabled;
    
    BoatSimulationMenu(const sf::Font& font) 
        : prevBtn(sf::Vector2f(110.f, 35.f)),
          nextBtn(sf::Vector2f(110.f, 35.f)),
          runSimBtn(sf::Vector2f(130.f, 35.f)),
          currentRouteIndex(-1),
          isAnimating(false),
          currentSegment(0),
          segmentProgress(0.0f),
          boatSpeed(100.0f),  // pixels per second
          btnNormal(50, 60, 75),
          btnHover(70, 80, 100),
          btnDisabled(30, 30, 30) {
        
        prevBtn.setFillColor(btnNormal);
        nextBtn.setFillColor(btnNormal);
        runSimBtn.setFillColor(btnNormal);
        
        prevText.setFont(font);
        prevText.setString("Previous");
        prevText.setCharacterSize(13);
        prevText.setFillColor(sf::Color::White);
        
        nextText.setFont(font);
        nextText.setString("Next");
        nextText.setCharacterSize(13);
        nextText.setFillColor(sf::Color::White);
        
        runSimText.setFont(font);
        runSimText.setString("Run Simulation");
        runSimText.setCharacterSize(13);
        runSimText.setFillColor(sf::Color::White);
        
        routeInfoText.setFont(font);
        routeInfoText.setCharacterSize(18);
        routeInfoText.setFillColor(sf::Color::White);
        
        statusText.setFont(font);
        statusText.setCharacterSize(12);
        statusText.setFillColor(sf::Color::Yellow);
        
        // Load boat texture
        if (!boatTexture.loadFromFile("ship.png")) {
            // Create a simple boat shape if texture not found
            boatTexture.create(30, 30);
            sf::Image img;
            img.create(30, 30, sf::Color(100, 150, 200));
            boatTexture.update(img);
        }
        boatSprite.setTexture(boatTexture);
        sf::Vector2u texSize = boatTexture.getSize();
        boatSprite.setOrigin(texSize.x / 2.0f, texSize.y / 2.0f);
        boatSprite.setScale(0.2f, 0.2f);
    }
    
    void updatePositions(float panelX, float winH) {
        // Buttons at bottom
        prevBtn.setPosition(panelX + 20, winH - 50);
        nextBtn.setPosition(panelX + 140, winH - 50);
        runSimBtn.setPosition(panelX + 260, winH - 50);
        
        // Button text
        prevText.setPosition(panelX + 45, winH - 42);
        nextText.setPosition(panelX + 165, winH - 42);
        runSimText.setPosition(panelX + 275, winH - 42);
        
        // Route info at top (with more space)
        routeInfoText.setPosition(panelX + 20, 120);
        statusText.setPosition(panelX + 20, winH - 90);
    }
    
    void reset() {
        // Don't reset currentRouteIndex - keep it so route is shown
        isAnimating = false;
        currentSegment = 0;
        segmentProgress = 0.0f;
        currentLayover = LayoverInfo();  // Reset layover info
        // Clear layover queue
        while (!layoverQueue.isEmpty()) {
            layoverQueue.dequeue();
        }
    }
    
    void initializeRoute() {
        // Initialize to first route when menu is opened
        if (currentRouteIndex < 0 && BookingSystem::bookedRoutes.size() > 0) {
            currentRouteIndex = 0;
        }
    }
    
    void startSimulation(const Vector<sf::Vector2f>& positions) {
        if (currentRouteIndex >= 0 && currentRouteIndex < BookingSystem::bookedRoutes.size()) {
            BookedRoute& route = BookingSystem::bookedRoutes[currentRouteIndex];
            if (route.path && route.path->found && route.path->path.getSize() > 0) {
                isAnimating = true;
                currentSegment = 0;
                segmentProgress = 0.0f;
                currentLayover = LayoverInfo();  // Reset current layover
                
                // Clear and populate layover queue with all layover ports
                while (!layoverQueue.isEmpty()) {
                    layoverQueue.dequeue();
                }
                
                // Pre-populate layover queue with all intermediate ports (layovers)
                for (int i = 1; i < route.path->path.getSize() - 1; i++) {
                    int layoverPortIdx = route.path->path.get(i);
                    layoverQueue.enqueue(LayoverInfo(layoverPortIdx));
                }
                
                // Set boat at origin port
                int originIdx = route.path->path.get(0);
                if (originIdx >= 0 && originIdx < positions.size()) {
                    boatSprite.setPosition(positions[originIdx]);
                }
            }
        }
    }
    
    void stopSimulation() {
        isAnimating = false;
    }
    
    void update(float deltaTime, const Vector<sf::Vector2f>& positions) {
        if (!isAnimating || currentRouteIndex < 0 || 
            currentRouteIndex >= BookingSystem::bookedRoutes.size()) {
            return;
        }
        
        BookedRoute& route = BookingSystem::bookedRoutes[currentRouteIndex];
        if (!route.path || !route.path->found) {
            isAnimating = false;
            return;
        }
        
        // If at layover, wait (using data structure)
        if (currentLayover.isActive) {
            currentLayover.timer += deltaTime;
            if (currentLayover.timer >= 1.5f) {  // 1.5 second layover
                // Layover complete, move to next segment
                currentLayover = LayoverInfo();  // Reset current layover
                // Move to next segment
                currentSegment++;
                segmentProgress = 0.0f;
            }
            return;
        }
        
        // Check if we've reached the end
        if (currentSegment >= route.path->path.getSize() - 1) {
            isAnimating = false;
            return;
        }
        
        // Get current and next port indices
        int currentPortIdx = route.path->path.get(currentSegment);
        int nextPortIdx = route.path->path.get(currentSegment + 1);
        
        // Get positions
        if (currentPortIdx < 0 || currentPortIdx >= positions.size() ||
            nextPortIdx < 0 || nextPortIdx >= positions.size()) {
            isAnimating = false;
            return;
        }
        
        sf::Vector2f startPos = positions[currentPortIdx];
        sf::Vector2f endPos = positions[nextPortIdx];
        
        // Calculate distance
        float dx = endPos.x - startPos.x;
        float dy = endPos.y - startPos.y;
        float distance = std::sqrt(dx * dx + dy * dy);
        
        // Update progress
        float progressIncrement = (boatSpeed * deltaTime) / distance;
        segmentProgress += progressIncrement;
        
        // Check if we've reached the end of this segment
        if (segmentProgress >= 1.0f) {
            segmentProgress = 1.0f;
            
            // Check if this is a layover (not the last segment)
            if (currentSegment < route.path->path.getSize() - 2) {
                // This is a layover port - get from queue
                if (!layoverQueue.isEmpty()) {
                    currentLayover = layoverQueue.dequeue();
                    currentLayover.timer = 0.0f;  // Reset timer
                } else {
                    // Fallback: create layover info directly
                    currentLayover = LayoverInfo(nextPortIdx);
                }
            } else {
                // Reached final destination
                isAnimating = false;
            }
        }
        
        // Update boat position
        float t = segmentProgress;
        sf::Vector2f boatPos = startPos + sf::Vector2f(dx * t, dy * t);
        boatSprite.setPosition(boatPos);
        
        // Rotate boat to face direction of travel
        if (distance > 0.1f) {
            float angle = std::atan2(dy, dx) * 180.0f / 3.14159f;
            boatSprite.setRotation(angle);
        }
    }
    
    void handleClick(const Graph& /*graph*/, const sf::Vector2f& mouseGlobal, 
                    float panelX, float /*winH*/, const Vector<sf::Vector2f>& positions) {
        if (BookingSystem::bookedRoutes.size() == 0) {
            return;
        }
        
        // Initialize to first route if not set
        if (currentRouteIndex < 0) {
            currentRouteIndex = 0;
        }
        
        // Previous button
        if (isHovering(prevBtn, mouseGlobal) && currentRouteIndex > 0) {
            stopSimulation();
            currentRouteIndex--;
        }
        
        // Next button
        if (isHovering(nextBtn, mouseGlobal) && 
            currentRouteIndex < BookingSystem::bookedRoutes.size() - 1) {
            stopSimulation();
            currentRouteIndex++;
        }
        
        // Run Simulation button
        if (isHovering(runSimBtn, mouseGlobal) && currentRouteIndex >= 0 && !isAnimating) {
            startSimulation(positions);
        }
    }
    
    void draw(sf::RenderWindow& window, const Graph& graph, const sf::Font& /*font*/,
             const sf::Vector2f& mouseGlobal, float /*panelX*/, float /*winH*/,
             const Vector<sf::Vector2f>& /*positions*/) {
        
        // Update button colors
        bool canGoPrev = (currentRouteIndex > 0);
        bool canGoNext = (currentRouteIndex < BookingSystem::bookedRoutes.size() - 1);
        bool canRunSim = (currentRouteIndex >= 0 && !isAnimating);
        
        prevBtn.setFillColor(canGoPrev ? 
            (isHovering(prevBtn, mouseGlobal) ? btnHover : btnNormal) : btnDisabled);
        nextBtn.setFillColor(canGoNext ? 
            (isHovering(nextBtn, mouseGlobal) ? btnHover : btnNormal) : btnDisabled);
        runSimBtn.setFillColor(canRunSim ? 
            (isHovering(runSimBtn, mouseGlobal) ? btnHover : btnNormal) : btnDisabled);
        
        // Draw buttons
        window.draw(prevBtn);
        window.draw(prevText);
        window.draw(nextBtn);
        window.draw(nextText);
        window.draw(runSimBtn);
        window.draw(runSimText);
        
        // Draw route info
        if (BookingSystem::bookedRoutes.size() == 0) {
            routeInfoText.setString("No booked routes available");
            routeInfoText.setFillColor(sf::Color(150, 150, 150));
        } else {
            std::stringstream ss;
            if (currentRouteIndex >= 0 && currentRouteIndex < BookingSystem::bookedRoutes.size()) {
                BookedRoute& route = BookingSystem::bookedRoutes[currentRouteIndex];
                ss << "Route " << (currentRouteIndex + 1) << " of " 
                   << BookingSystem::bookedRoutes.size() << "\n";
                ss << "From: " << graph.vertices[route.originIndex].port.name << "\n";
                ss << "To: " << graph.vertices[route.destinationIndex].port.name << "\n";
                ss << "Date: " << route.departureDate << "\n";
                if (route.path && route.path->found) {
                    ss << "Cost: $" << route.path->totalCost << "\n";
                    ss << "Time: " << route.path->totalTime << " min";
                }
            } else {
                ss << "Select a route to simulate";
            }
            routeInfoText.setString(ss.str());
            routeInfoText.setFillColor(sf::Color::White);
        }
        window.draw(routeInfoText);
        
        // Draw status
        std::stringstream statusSs;
        if (isAnimating) {
            if (currentLayover.isActive) {
                statusSs << "Layover at " << graph.vertices[currentLayover.portIndex].port.name 
                         << " (" << (int)(1.5f - currentLayover.timer) << "s)";
            } else {
                statusSs << "Simulating...";
            }
        } else if (currentRouteIndex >= 0) {
            statusSs << "Ready - Click 'Run Simulation' to start";
        } else {
            statusSs << "No route selected";
        }
        statusText.setString(statusSs.str());
        window.draw(statusText);
        
        // Draw boat if animating
        if (isAnimating && currentRouteIndex >= 0 && 
            currentRouteIndex < BookingSystem::bookedRoutes.size()) {
            window.draw(boatSprite);
        }
    }
};

#endif

