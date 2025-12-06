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
    if (!font.loadFromFile("Arial.ttf")) {
        cerr << "ERROR: Font missing! (Arial.ttf)\n";
        return 1;
    }

    // --- PATHFINDING STATE ---
    PathFinding::PathResult* currentPathResult = nullptr;
    string resultTextString = "";

    // --- PORT POSITIONS ---
    vector<sf::Vector2f> positions = PortInitializer::getDefaultPositions();

    // --- VISUAL ELEMENTS: NODES (SPRITES) ---
    vector<sf::Sprite> portSprites;
    vector<sf::Text> labels;
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

    // --- MAIN LOOP ---
    while (window.isOpen()) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mouseGlobal = window.mapPixelToCoords(mousePos);

        // UI Positioning
        uiPanel.updatePositions();
        mainMenu.updatePositions(uiPanel.panelX);
        routeMenu.updatePositions(uiPanel.panelX, winH);
        preferencesMenu.updatePositions(uiPanel.panelX, winH);
        bookingMenu.updatePositions(uiPanel.panelX, winH);

        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) window.close();

            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::P) {
                if (!routeMenu.selectingOrigin && !routeMenu.selectingDest &&
                    !preferencesMenu.selectingOrigin && !preferencesMenu.selectingDest &&
                    !preferencesMenu.selectingPorts && !preferencesMenu.selectingCompanies &&
                    !bookingMenu.selectingOrigin && !bookingMenu.selectingDest && !bookingMenu.selectingDate) {
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
                    else if (bookingMenu.selectingOrigin || bookingMenu.selectingDest || bookingMenu.selectingDate) {
                        bookingMenu.handleMouseWheel(e.mouseWheelScroll.delta, graph);
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
                        bookingMenu.handleClick(graph, mouseGlobal, uiPanel.panelX, winH, font,
                                              currentPathResult, resultTextString);
                    }
                }
            }
        }

        // --- ANIMATION ---
        uiPanel.updateAnimation();

        // --- RENDER ---
        VisualRenderer::drawMap(window, mapSprite);
        VisualRenderer::drawPath(window, currentPathResult, positions);
        VisualRenderer::drawPorts(window, graph, portSprites, labels, positions, 
                                 currentPathResult, mouseGlobal, baseScale, 
                                 uiPanel.panelOpen, uiPanel.panelWidth,
                                 preferencesMenu.selectedPorts);

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
            }
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
