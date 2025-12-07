#include <iostream>
#include "linkedList.h"
#include <SFML/Graphics.hpp>
#include "Graph.hpp"
#include <fstream>
#include <sstream>
#include <cmath>
#include <string>
#include <iomanip>

// Include pathfinding logic
#include "pathFinding.h"

// Include UI components
#include "uiHelpers.hpp"
#include "visualRenderer.hpp"
#include "mainMenu.hpp"
#include "routeFindingMenu.hpp"
#include "preferencesMenu.hpp"
#include "bookingMenu.hpp"
#include "portInitializer.hpp"
#include "uiPanel.hpp"
#include "boatSimulationMenu.hpp"
#include "multiLegJourneyMenu.hpp"

using namespace std;

int main() {
    // --- GRAPH SETUP ---
    Graph graph;
    graph.addPorts("PortCharges.txt");
    graph.addRoutes("Routes.txt");

    if (graph.size == 0) {
        cerr << "Error: No ports loaded.\n";
        return 1;
    }

    // --- WINDOW SETUP ---
    const unsigned int winW = 1600;
    const unsigned int winH = 900;

    sf::RenderWindow window(sf::VideoMode(winW, winH), "OceanRoute Nav Optimizer (Professional)");
    window.setFramerateLimit(60);

    // --- TEXTURES ---
    sf::Texture mapTexture;
    if (!mapTexture.loadFromFile("world_map.png")) {
        cerr << "ERROR: world_map.png missing! Creating fallback.\n";
        mapTexture.create(winW, winH);
    }
    sf::Sprite mapSprite(mapTexture);
    sf::Vector2u texSize = mapTexture.getSize();
    mapSprite.setScale(float(winW) / texSize.x, float(winH) / texSize.y);

    // Load Port Texture
    sf::Texture portTexture;
    if (!portTexture.loadFromFile("Port.png")) {
        cerr << "ERROR: Port.png missing! Ensure the file is in the project folder.\n";
        portTexture.create(20, 20);
    }

    // --- FONTS ---
    sf::Font font;
    if (!font.loadFromFile("CinzelDecorative-Regular.ttf")) {
        cerr << "ERROR: Font missing! (Arial.ttf)\n";
        return 1;
    }

    // --- PATHFINDING STATE ---
    PathFinding::PathResult* currentPathResult = nullptr;
    string resultTextString = "";

    // --- PORT POSITIONS ---
    Vector<sf::Vector2f> positions = PortInitializer::getDefaultPositions();

    // --- VISUAL ELEMENTS: NODES (SPRITES) ---
    Vector<sf::Sprite> portSprites;
    Vector<sf::Text> labels;
    float baseScale = 0.15f;
    
    PortInitializer::initializePorts(graph, portTexture, font, positions, 
                                    portSprites, labels, baseScale);

    // --- UI PANEL SETUP ---
    const float panelWidth = 400.f;
    int currentMenu = 0;
    UIPanel uiPanel(font, winH, panelWidth);

    // --- MENU COMPONENTS ---
    MainMenu mainMenu(font);
    RouteFindingMenu routeMenu(font);
    PreferencesMenu preferencesMenu(graph, font);
    BookingMenu bookingMenu(graph, font);
    BoatSimulationMenu boatSimMenu(font);
    MultiLegJourneyMenu multiLegMenu(font);
    
    // Clock for delta time
    sf::Clock clock;

    // --- MAIN LOOP ---
    while (window.isOpen()) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mouseGlobal = window.mapPixelToCoords(mousePos);

        // Calculate delta time
        float deltaTime = clock.restart().asSeconds();
        
        // UI Positioning
        uiPanel.updatePositions();
        mainMenu.updatePositions(uiPanel.panelX);
        routeMenu.updatePositions(uiPanel.panelX, winH);
        preferencesMenu.updatePositions(uiPanel.panelX, winH);
        bookingMenu.updatePositions(uiPanel.panelX, winH);
        boatSimMenu.updatePositions(uiPanel.panelX, winH);
        multiLegMenu.updatePositions(uiPanel.panelX, winH, winW);

        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) window.close();

            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::P) {
                if (!routeMenu.selectingOrigin && !routeMenu.selectingDest &&
                    !preferencesMenu.selectingOrigin && !preferencesMenu.selectingDest &&
                    !preferencesMenu.selectingPorts && !preferencesMenu.selectingCompanies &&
                    !bookingMenu.selectingOrigin && !bookingMenu.selectingDest && !bookingMenu.selectingDate &&
                    !multiLegMenu.selectingOrigin && !multiLegMenu.selectingDest) {
                    uiPanel.panelOpen = !uiPanel.panelOpen;
                }
            }
            

            if (e.type == sf::Event::MouseWheelScrolled) {
                if (uiPanel.panelOpen) {
                    if (routeMenu.selectingOrigin || routeMenu.selectingDest) {
                        routeMenu.handleMouseWheel(e.mouseWheelScroll.delta, graph);
                    }
                    else if (preferencesMenu.selectingOrigin || preferencesMenu.selectingDest) {
                        preferencesMenu.handleMouseWheel(e.mouseWheelScroll.delta, true, false, graph);
                    }
                    else if (preferencesMenu.selectingPorts) {
                        preferencesMenu.handleMouseWheel(e.mouseWheelScroll.delta, false, true, graph);
                    }
                    else if (preferencesMenu.selectingCompanies) {
                        preferencesMenu.handleMouseWheel(e.mouseWheelScroll.delta, false, false, graph);
                    }
                    else if (bookingMenu.selectingOrigin || bookingMenu.selectingDest || bookingMenu.selectingDate || bookingMenu.showingRouteDetails) {
                        bookingMenu.handleMouseWheel(e.mouseWheelScroll.delta, graph);
                    }
                    else if (multiLegMenu.selectingOrigin || multiLegMenu.selectingDest) {
                        multiLegMenu.handleMouseWheel(e.mouseWheelScroll.delta, graph);
                    }
                }
            }

            if (e.type == sf::Event::MouseButtonPressed) {
                if (!uiPanel.panelOpen) {
                    uiPanel.handleToggleClick(mouseGlobal);
                } 
                else {
                    // Inside Panel Logic
                    if (uiPanel.handleCloseClick(mouseGlobal, currentMenu)) {
                        // Panel closed
                    }
                    else if (uiPanel.handleBackClick(mouseGlobal, currentMenu)) {
                        currentMenu = 0;
                        routeMenu.reset();
                        preferencesMenu.reset();
                        bookingMenu.reset();
                        boatSimMenu.reset();
                        multiLegMenu.reset();
                        // Check if currentPathResult is in filteredRoutes or availableRoutes before deleting
                        bool isInFiltered = false;
                        for (int i = 0; i < preferencesMenu.filteredRoutes.size(); i++) {
                            if (preferencesMenu.filteredRoutes[i] == currentPathResult) {
                                isInFiltered = true;
                                break;
                            }
                        }
                        if (!isInFiltered) {
                            for (int i = 0; i < bookingMenu.availableRoutes.size(); i++) {
                                if (bookingMenu.availableRoutes[i] == currentPathResult) {
                                    isInFiltered = true;
                                    break;
                                }
                            }
                        }
                        if (currentPathResult && !isInFiltered) {
                            delete currentPathResult;
                        }
                        currentPathResult = nullptr;
                    }
                    else if (currentMenu == 0) {
                        int menuChoice = mainMenu.handleClick(mouseGlobal);
                        if (menuChoice > 0) {
                            currentMenu = menuChoice;
                            // Initialize menus when opened
                            if (menuChoice == 4) {
                                boatSimMenu.initializeRoute();
                            } else if (menuChoice == 5) {
                                multiLegMenu.initialize();
                            }
                        }
                    }
                    else if (currentMenu == 1) {
                        routeMenu.handleClick(graph, mouseGlobal, uiPanel.panelX, 
                                            currentPathResult, resultTextString);
                    }
                    else if (currentMenu == 2) {
                        preferencesMenu.handleClick(graph, mouseGlobal, uiPanel.panelX, winH, font,
                                                   currentPathResult, resultTextString);
                    }
                    else if (currentMenu == 3) {
                        bool panelShouldClose = false;
                        bookingMenu.handleClick(graph, mouseGlobal, uiPanel.panelX, winH, font,
                                              currentPathResult, resultTextString, panelShouldClose);
                        if (panelShouldClose) {
                            uiPanel.panelOpen = false;
                        }
                    }
                    else if (currentMenu == 4) {
                        boatSimMenu.handleClick(graph, mouseGlobal, uiPanel.panelX, winH, positions);
                    }
                    else if (currentMenu == 5) {
                        bool panelShouldClose = false;
                        multiLegMenu.handleClick(graph, mouseGlobal, uiPanel.panelX, winH, panelShouldClose);
                        if (panelShouldClose) {
                            uiPanel.panelOpen = false;
                        }
                    }
                }
            }
            
            // Check if multi-leg journey just completed and open panel
            if (multiLegMenu.showResult && !uiPanel.panelOpen) {
                uiPanel.panelOpen = true;
            }
            
            // Handle port clicks on map when tracking multi-leg journey
            if (!uiPanel.panelOpen && multiLegMenu.isTracking) {
                if (e.type == sf::Event::MouseButtonPressed) {
                    // First check if clicking on modal
                    if (multiLegMenu.showModal) {
                        if (multiLegMenu.handleModalClick(graph, mouseGlobal)) {
                            // Modal click handled
                            continue;
                        }
                    }
                    
                    // If modal not shown, check if clicking on a port
                    if (!multiLegMenu.showModal) {
                        for (int i = 0; i < graph.size; i++) {
                            if (isHovering(portSprites[i], mouseGlobal)) {
                                multiLegMenu.handleMapPortClick(i, graph);
                                break;
                            }
                        }
                    }
                }
            }
        }

        // --- ANIMATION ---
        uiPanel.updateAnimation();
        boatSimMenu.update(deltaTime, positions);
        multiLegMenu.update(deltaTime);

        // --- RENDER ---
        VisualRenderer::drawMap(window, mapSprite);
        
        // Draw path for boat simulation if active
        PathFinding::PathResult* boatSimPath = nullptr;
        bool useDottedLines = false;
        
        if (currentMenu == 4 && boatSimMenu.currentRouteIndex >= 0 && 
            boatSimMenu.currentRouteIndex < BookingSystem::bookedRoutes.size()) {
            BookedRoute& route = BookingSystem::bookedRoutes[boatSimMenu.currentRouteIndex];
            if (route.path && route.path->found) {
                boatSimPath = route.path;
                useDottedLines = boatSimMenu.isAnimating; // Use dotted lines during simulation
                VisualRenderer::drawPath(window, route.path, positions, useDottedLines);
            }
        } 
        // Draw path for multi-leg journey if active or completed (show path until user leaves menu)
        else if (currentMenu == 5 && multiLegMenu.currentPathResult) {
            VisualRenderer::drawPath(window, multiLegMenu.currentPathResult, positions, false);
        }
        else {
            VisualRenderer::drawPath(window, currentPathResult, positions, false);
        }
        
        // Pass boat simulation path or multi-leg journey path for port highlighting
        PathFinding::PathResult* highlightPath = nullptr;
        if (currentMenu == 4 && boatSimPath) {
            highlightPath = boatSimPath;
        } else if (currentMenu == 5 && multiLegMenu.currentPathResult) {
            // Highlight all ports in the path (whether tracking or completed)
            highlightPath = multiLegMenu.currentPathResult;
        }
        
        // Check if subgraph mode should be disabled when panel opens
        if (currentMenu == 3 && uiPanel.panelOpen && bookingMenu.showSubgraph) {
            bookingMenu.showSubgraph = false;
        }
        
        VisualRenderer::drawPorts(window, graph, portSprites, labels, positions, 
                                 currentPathResult, mouseGlobal, baseScale, 
                                 uiPanel.panelOpen, uiPanel.panelWidth,
                                 preferencesMenu.selectedPorts,
                                 highlightPath,
                                 (currentMenu == 3 && bookingMenu.showSubgraph));
        
        // Draw boat if simulating
        if (currentMenu == 4 && boatSimMenu.isAnimating) {
            window.draw(boatSimMenu.boatSprite);
        }

        // Draw Panel
        uiPanel.draw(window, currentMenu);
        window.draw(mainMenu.menuHeading);
        
        if (uiPanel.panelOpen) {
            if (currentMenu == 0) {
                mainMenu.draw(window, mouseGlobal);
            } 
            else {
                if (currentMenu == 1) {
                    routeMenu.draw(window, graph, font, mouseGlobal, uiPanel.panelX, 
                                 currentPathResult, resultTextString);
                }
                else if (currentMenu == 2) {
                    preferencesMenu.draw(window, graph, font, mouseGlobal, uiPanel.panelX, winH);
                }
                else if (currentMenu == 3) {
                    bookingMenu.draw(window, graph, font, mouseGlobal, uiPanel.panelX, winH,
                                   currentPathResult, resultTextString);
                }
                else if (currentMenu == 4) {
                    boatSimMenu.draw(window, graph, font, mouseGlobal, uiPanel.panelX, winH, positions);
                }
                else if (currentMenu == 5) {
                    multiLegMenu.draw(window, graph, font, mouseGlobal, uiPanel.panelX, winH, winW);
                }
            }
        }
        
        // Draw multi-leg menu modals even when panel is closed
        if (multiLegMenu.showModal || multiLegMenu.showResult) {
            multiLegMenu.draw(window, graph, font, mouseGlobal, uiPanel.panelX, winH, winW);
        }
        
        // Draw alert even when panel is closed
        if (multiLegMenu.showAlert) {
            multiLegMenu.draw(window, graph, font, mouseGlobal, uiPanel.panelX, winH, winW);
        }
        
        window.display();
    }
    
    // Cleanup
    // Check if currentPathResult is in filteredRoutes or availableRoutes to avoid double deletion
    bool isInFilteredRoutes = false;
    for (auto* route : preferencesMenu.filteredRoutes) {
        if (route == currentPathResult) {
            isInFilteredRoutes = true;
            break;
        }
    }
    if (!isInFilteredRoutes) {
        for (auto* route : bookingMenu.availableRoutes) {
            if (route == currentPathResult) {
                isInFilteredRoutes = true;
                break;
            }
        }
    }
    
    // Set currentPathResult to nullptr before cleanup
    // The menus' cleanup() will delete all routes in their vectors
    if (isInFilteredRoutes) {
        currentPathResult = nullptr;  // Don't delete - it's owned by a menu
    } else if (currentPathResult) {
        // Only delete if it's not in any menu's vector
        delete currentPathResult;
        currentPathResult = nullptr;
    }
    
    // Cleanup menus (this will delete filteredRoutes and availableRoutes)
    preferencesMenu.cleanup();
    bookingMenu.cleanup();
    
    return 0;
}
