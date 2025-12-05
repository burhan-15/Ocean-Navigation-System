#include <iostream>
#include "linkedList.h"
#include <SFML/Graphics.hpp>
#include "Graph.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <string>
#include <iomanip>

// Include pathfinding logic
#include "pathFinding.h"

using namespace std;

// [VISUAL UPGRADE] Helper function to draw thick lines
void drawThickLine(sf::RenderWindow& window, sf::Vector2f point1, sf::Vector2f point2, float thickness, sf::Color color) {
    sf::Vector2f direction = point2 - point1;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159f;

    sf::RectangleShape line(sf::Vector2f(length, thickness));
    line.setPosition(point1);
    line.setRotation(angle);
    line.setFillColor(color);
    window.draw(line);
}

// [VISUAL UPGRADE] Check if mouse is hovering over shapes
bool isHovering(const sf::Shape& shape, const sf::Vector2f& mousePos) {
    return shape.getGlobalBounds().contains(mousePos);
}

bool isHovering(const sf::RectangleShape& rect, const sf::Vector2f& mousePos) {
    return rect.getGlobalBounds().contains(mousePos);
}

// NEW: Overload for Sprites
bool isHovering(const sf::Sprite& sprite, const sf::Vector2f& mousePos) {
    return sprite.getGlobalBounds().contains(mousePos);
}

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

    // NEW: Load Port Texture
    sf::Texture portTexture;
    if (!portTexture.loadFromFile("Port.png")) {
        cerr << "ERROR: Port.png missing! Ensure the file is in the project folder.\n";
        // Create a small fallback texture if missing so the game doesn't crash
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
    vector<sf::Vector2f> positions = {
        {1150.f, 330.f}, {1050.f, 300.f}, {730.f, 170.f}, {790.f, 200.f}, {925.f, 350.f},
        {1120.f, 420.f}, {1400.f, 630.f}, {1350.f, 650.f}, {490.f, 200.f}, {970.f, 310.f},
        {1275.f, 430.f}, {880.f, 280.f}, {920.f, 220.f}, {1200.f, 450.f}, {1300.f, 250.f},
        {1350.f, 50.f}, {420.f, 130.f}, {850.f, 230.f}, {820.f, 620.f}, {810.f, 130.f},
        {1000.f, 360.f}, {880.f, 620.f}, {790.f, 250.f}, {860.f, 160.f}, {1250.f, 150.f},
        {1275.f, 350.f}, {1250.f, 500.f}, {730.f, 250.f}, {450.f, 350.f}, {250.f, 300.f},
        {1070.f, 350.f}, {400.f, 250.f}, {550.f, 600.f}, {950.f, 80.f}, {925.f, 500.f},
        {700.f, 350.f}, {1275.f, 300.f}, {800.f, 450.f}, {1350.f, 200.f}, {1500.f, 550.f}
    };

    // --- VISUAL ELEMENTS: NODES (SPRITES) ---
    // Changed from sf::CircleShape to sf::Sprite
    vector<sf::Sprite> portSprites(graph.size);
    vector<sf::Text> labels(graph.size);

    // Define a base scale for your sprite. 
    // If your image is very big (e.g. 500px), set this to 0.1f. If small, set to 1.0f.
    float baseScale = 0.15f; 

    for (int i = 0; i < graph.size; i++) {
        // Setup Sprite
        portSprites[i].setTexture(portTexture);
        
        // Center the sprite origin
        sf::Vector2u pSize = portTexture.getSize();
        portSprites[i].setOrigin(pSize.x / 2.0f, pSize.y / 2.0f);
        
        if (i < (int)positions.size()) portSprites[i].setPosition(positions[i]);
        
        portSprites[i].setScale(baseScale, baseScale);
        portSprites[i].setColor(sf::Color(255, 255, 255)); // Default white (no tint)

        // Setup Labels
        labels[i].setFont(font);
        labels[i].setString(graph.vertices[i].port.name);
        labels[i].setCharacterSize(12);
        labels[i].setFillColor(sf::Color::White);
        labels[i].setOutlineColor(sf::Color::Black);
        labels[i].setOutlineThickness(2.f); 

        auto b = labels[i].getLocalBounds();
        labels[i].setOrigin(b.width / 2, b.height + 8);
        // Position text slightly above the sprite
        if (i < (int)positions.size()) labels[i].setPosition(positions[i].x, positions[i].y - (pSize.y * baseScale)/2 - 5);
    }

    // --- UI PANEL SETUP ---
    const float panelWidth = 400.f; 
    float panelX = -panelWidth;
    bool panelOpen = false;
    int currentMenu = 0;

    sf::RectangleShape panel(sf::Vector2f(panelWidth, winH));
    panel.setFillColor(sf::Color(20, 30, 45, 245)); 
    panel.setOutlineThickness(-2.f); 
    panel.setOutlineColor(sf::Color(0, 150, 200)); 

    // --- BUTTONS ---
    sf::RectangleShape toggleBtn(sf::Vector2f(80, 40));
    toggleBtn.setFillColor(sf::Color(0, 150, 200));
    toggleBtn.setPosition(10, 10);
    
    sf::Text toggleBtnTxt("MENU", font, 16);
    toggleBtnTxt.setFillColor(sf::Color::White);
    toggleBtnTxt.setPosition(25, 20);

    // Close Button
    sf::Text closeBtnTxt("X", font, 22);
    closeBtnTxt.setFillColor(sf::Color(200, 50, 50));
    sf::RectangleShape closeBtnHitbox(sf::Vector2f(40,40)); 
    closeBtnHitbox.setFillColor(sf::Color::Transparent);

    // Back Button
    sf::Text backBtnTxt("< BACK", font, 18);
    backBtnTxt.setFillColor(sf::Color(100, 200, 255));
    sf::RectangleShape backBtnHitbox(sf::Vector2f(80,40)); 
    backBtnHitbox.setFillColor(sf::Color::Transparent);

    // --- MENU STYLING ---
    sf::Vector2f menuSize(360.f, 50.f);
    sf::Color btnNormal(50, 60, 75);
    sf::Color btnHover(70, 80, 100);

    sf::RectangleShape menuRect1(menuSize), menuRect2(menuSize), menuRect3(menuSize);
    menuRect1.setFillColor(btnNormal); menuRect2.setFillColor(btnNormal); menuRect3.setFillColor(btnNormal);

    sf::Text menuHeading("Ocean Navigation System",font,32);
    sf::Text menu1("1. Route Finder (Cost/Time)", font, 16);
    sf::Text menu2("2. Booking & Connections", font, 16);
    sf::Text menu3("3. Preferences & Filters", font, 16);
    menu1.setFillColor(sf::Color::White); menu2.setFillColor(sf::Color::White); menu3.setFillColor(sf::Color::White);

    // Submenu Buttons
    sf::RectangleShape subBtn1(menuSize), subBtn2(menuSize);
    subBtn1.setFillColor(btnNormal); subBtn2.setFillColor(btnNormal);
    
    sf::Text sub1_A("Find Shortest TIME Route", font, 16);
    sf::Text sub1_B("Find Cheapest COST Route", font, 16);
    sf::Text sub2_A("Select Route and Date", font, 16);
    sf::Text sub2_B("View Connecting Routes", font, 16);
    sf::Text sub3_A("Filter by Shipping Company", font, 16);
    sf::Text sub3_B("Avoid Specific Port", font, 16);
    sub1_A.setFillColor(sf::Color::White); sub1_B.setFillColor(sf::Color::White);
    sub2_A.setFillColor(sf::Color::White); sub2_B.setFillColor(sf::Color::White);
    sub3_A.setFillColor(sf::Color::White); sub3_B.setFillColor(sf::Color::White);

    // --- INPUT FIELDS ---
    bool selectingOrigin = false, selectingDest = false;
    int selectedOriginIndex = -1, selectedDestIndex = -1;
    int listOffset = 0;
    
    sf::RectangleShape fieldOrigin(sf::Vector2f(340.f, 40.f));
    sf::RectangleShape fieldDest(sf::Vector2f(340.f, 40.f));
    fieldOrigin.setFillColor(sf::Color(30, 40, 50)); fieldOrigin.setOutlineColor(sf::Color(120,120,120)); fieldOrigin.setOutlineThickness(1.f);
    fieldDest.setFillColor(sf::Color(30, 40, 50)); fieldDest.setOutlineColor(sf::Color(120,120,120)); fieldDest.setOutlineThickness(1.f);

    sf::Text originLabel("Select Origin...", font, 15); originLabel.setFillColor(sf::Color(150,150,150));
    sf::Text destLabel("Select Destination...", font, 15); destLabel.setFillColor(sf::Color(150,150,150));

    // --- RESULTS PANEL ---
    sf::RectangleShape resultsArea(sf::Vector2f(panelWidth - 40, 200));
    resultsArea.setFillColor(sf::Color(0, 0, 0, 80)); 
    resultsArea.setOutlineColor(sf::Color(0, 255, 200)); 
    resultsArea.setOutlineThickness(1.f);

    sf::Text resultHeader("ROUTE RESULTS", font, 14);
    resultHeader.setFillColor(sf::Color(0, 255, 200));
    sf::Text resultBody("Select origin and destination\nto calculate path.", font, 16);
    resultBody.setFillColor(sf::Color::White);
    resultBody.setLineSpacing(1.4f);

    // --- MAIN LOOP ---
    while (window.isOpen()) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mouseGlobal = window.mapPixelToCoords(mousePos);

        // UI Positioning
        panel.setPosition(panelX, 0);
        closeBtnHitbox.setPosition(panelX + panelWidth - 50, 10);
        closeBtnTxt.setPosition(panelX + panelWidth - 40, 15);
        backBtnHitbox.setPosition(panelX + 10, 10);
        backBtnTxt.setPosition(panelX + 20, 15);
        
        menuRect1.setPosition(panelX + 20, 120);
        menuRect2.setPosition(panelX + 20, 120 + 65);
        menuRect3.setPosition(panelX + 20, 120 + 130);
        menuHeading.setPosition(panelX + 15, 50);
        menu1.setPosition(panelX + 40, 135);
        menu2.setPosition(panelX + 40, 200);
        menu3.setPosition(panelX + 40, 265);

        float subY = 120.f;
        fieldOrigin.setPosition(panelX + 30, subY);
        originLabel.setPosition(panelX + 40, subY + 10);
        fieldDest.setPosition(panelX + 30, subY + 50);
        destLabel.setPosition(panelX + 40, subY + 60);
        subBtn1.setPosition(panelX + 20, subY + 110);
        subBtn2.setPosition(panelX + 20, subY + 175);
        sub1_A.setPosition(panelX + 50, subY + 125);
        sub1_B.setPosition(panelX + 50, subY + 190);
        resultsArea.setPosition(panelX + 20, winH - 220);
        resultHeader.setPosition(panelX + 35, winH - 220 + 15);
        resultBody.setPosition(panelX + 35, winH - 220 + 50);

        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) window.close();

            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::P) {
                if (!selectingOrigin && !selectingDest) panelOpen = !panelOpen;
            }

            if (e.type == sf::Event::MouseWheelScrolled) {
                if (panelOpen && (selectingOrigin || selectingDest)) {
                    if (e.mouseWheelScroll.delta > 0) listOffset--;
                    else listOffset++;
                    if (listOffset < 0) listOffset = 0;
                }
            }

            if (e.type == sf::Event::MouseButtonPressed) {
                if (!panelOpen) {
                    if (toggleBtn.getGlobalBounds().contains(mouseGlobal)) {
                        panelOpen = true;
                    }
                } 
                else {
                    // Inside Panel Logic
                    if (currentMenu == 0 && closeBtnHitbox.getGlobalBounds().contains(mouseGlobal)) {
                        panelOpen = false;
                    }
                    else if (currentMenu != 0 && backBtnHitbox.getGlobalBounds().contains(mouseGlobal)) {
                        currentMenu = 0;
                        selectingOrigin = selectingDest = false;
                        if(currentPathResult) { delete currentPathResult; currentPathResult = nullptr; }
                    }
                    else if (currentMenu == 0) {
                        if (menuRect1.getGlobalBounds().contains(mouseGlobal)) currentMenu = 1;
                        else if (menuRect2.getGlobalBounds().contains(mouseGlobal)) currentMenu = 2;
                        else if (menuRect3.getGlobalBounds().contains(mouseGlobal)) currentMenu = 3;
                    }
                    else if (currentMenu == 1) {
                        if (selectingOrigin || selectingDest) {
                            int const visibleItems = 8;
                            float itemH = 30.f;
                            float startY  = (selectingOrigin) ? fieldOrigin.getPosition().y +45 : fieldDest.getPosition().y +45;

                            bool clickedItem = false;
                            int visibleIndex = 0;
                            int skip = 0;

                            for(int i=0; i<graph.size; i++) {
                                if(selectingOrigin && i == selectedDestIndex) continue;
                                if(selectingDest && i == selectedOriginIndex) continue;
                                
                                if(skip < listOffset) { skip++; continue; }
                                if(visibleIndex >= visibleItems) break;

                                sf::FloatRect itemRect(panelX + 30, startY + visibleIndex*(itemH+2), 340, itemH);
                                if(itemRect.contains(mouseGlobal)) {
                                    if(selectingOrigin) selectedOriginIndex = i;
                                    else selectedDestIndex = i;
                                    
                                    selectingOrigin = selectingDest = false;
                                    if(currentPathResult) { delete currentPathResult; currentPathResult = nullptr; }
                                    clickedItem = true;
                                    break;
                                }
                                visibleIndex++;
                            }
                            if(!clickedItem) { selectingOrigin = selectingDest = false; }
                        }
                        else if (fieldOrigin.getGlobalBounds().contains(mouseGlobal)) { selectingOrigin = true; selectingDest = false; }
                        else if (fieldDest.getGlobalBounds().contains(mouseGlobal)) { selectingDest = true; selectingOrigin = false; }
                        else if (selectedOriginIndex != -1 && selectedDestIndex != -1) {
                            if (subBtn1.getGlobalBounds().contains(mouseGlobal)) {
                                if(currentPathResult) delete currentPathResult;
                                currentPathResult = PathFinding::findShortestTimePath(graph, selectedOriginIndex, selectedDestIndex);
                                resultTextString = "Optimization: FASTEST";
                                
                            }
                            else if (subBtn2.getGlobalBounds().contains(mouseGlobal)) {
                                if(currentPathResult) delete currentPathResult;
                                currentPathResult = PathFinding::findCheapestPath(graph, selectedOriginIndex, selectedDestIndex);
                                resultTextString = "Optimization: CHEAPEST";
                            }
                        }
                    }
                }
            }
        }

        // --- ANIMATION ---
        if (panelOpen && panelX < 0) panelX += 25;
        if (!panelOpen && panelX > -panelWidth) panelX -= 25;
        if (panelX > 0) panelX = 0;
        if (panelX < -panelWidth) panelX = -panelWidth;

        // --- RENDER ---
        window.clear(sf::Color(10, 10, 10)); 
        window.draw(mapSprite);

        // 1. Draw Path Lines
        if (currentPathResult && currentPathResult->found) {
            for (int i = 0; i < currentPathResult->path.getSize() - 1; i++) {
                int u = currentPathResult->path.get(i);
                int v = currentPathResult->path.get(i+1);
                
                if (u >= 0 && u < (int)positions.size() && v >= 0 && v < (int)positions.size()) {
                    drawThickLine(window, positions[u], positions[v], 6.f, sf::Color(0, 0, 0, 150));
                    drawThickLine(window, positions[u], positions[v], 3.f, sf::Color(255, 0, 255));
                }
            }
        }

        // 2. Draw Nodes (SPRITES)
        for (int i = 0; i < graph.size; i++) {
            bool underPanel = panelOpen && positions[i].x < panelWidth;
            if (!underPanel) {
                bool inPath = false;
                bool isEndNode = false;
                if(currentPathResult && currentPathResult->found) {
                    for(int k=0; k<currentPathResult->path.getSize(); k++) 
                        if(currentPathResult->path.get(k) == i){
                            if(k == 0 || k == currentPathResult->path.getSize()-1) isEndNode = true;
                            else inPath = true;
                        }
                }

                float currentScale = baseScale;
                sf::Color tintColor = sf::Color::White; // Normal color (White = original image colors)

                if (inPath) {
                    tintColor = sf::Color(255, 100, 100); // Red tint for path
                    currentScale = baseScale * 1.3f;      // Make slightly bigger
                }
                else if(isEndNode){
                    tintColor = sf::Color::Green; // Red tint for path
                    currentScale = baseScale * 1.3f;
                }
                else {
                    tintColor = sf::Color(200, 255, 255); 
                }

                if (isHovering(portSprites[i], mouseGlobal)) {
                    currentScale *= 1.4f; // Pop effect on hover
                    labels[i].setFillColor(sf::Color::Yellow);
                    labels[i].setStyle(sf::Text::Bold);
                    tintColor = sf::Color(255, 255, 0); // Yellow tint on hover
                } else {
                    labels[i].setFillColor(sf::Color::White);
                    labels[i].setStyle(sf::Text::Regular);
                }

                portSprites[i].setScale(currentScale, currentScale);
                portSprites[i].setColor(tintColor);
                
                window.draw(portSprites[i]);
            }
        }

        for(int i = 0;i<graph.size;i++){
            bool underPanel = panelOpen && positions[i].x < panelWidth;
            if(!underPanel){
                window.draw(labels[i]);
            }
        }

        // 3. Draw Panel
        window.draw(panel);
        window.draw(menuHeading);
        if (!panelOpen) {
            window.draw(toggleBtn);
            window.draw(toggleBtnTxt);
        } 
        else {
            if (currentMenu == 0) {
                window.draw(closeBtnHitbox);
                window.draw(closeBtnTxt); 
                
                menuRect1.setFillColor(isHovering(menuRect1, mouseGlobal) ? btnHover : btnNormal);
                menuRect2.setFillColor(isHovering(menuRect2, mouseGlobal) ? btnHover : btnNormal);
                menuRect3.setFillColor(isHovering(menuRect3, mouseGlobal) ? btnHover : btnNormal);
                window.draw(menuRect1); window.draw(menu1);
                window.draw(menuRect2); window.draw(menu2);
                window.draw(menuRect3); window.draw(menu3);
            } 
            else {
                window.draw(backBtnTxt);
                
                if (currentMenu == 1) {
                    window.draw(fieldOrigin);
                    window.draw(fieldDest);
                    
                    if(selectedOriginIndex != -1) {
                        originLabel.setString(graph.vertices[selectedOriginIndex].port.name);
                        originLabel.setFillColor(sf::Color::White);
                    } else { originLabel.setString("Select Origin..."); originLabel.setFillColor(sf::Color(150,150,150)); }
                    
                    if(selectedDestIndex != -1) {
                        destLabel.setString(graph.vertices[selectedDestIndex].port.name);
                        destLabel.setFillColor(sf::Color::White);
                    } else { destLabel.setString("Select Destination..."); destLabel.setFillColor(sf::Color(150,150,150)); }

                    window.draw(originLabel);
                    window.draw(destLabel);

                    subBtn1.setFillColor(isHovering(subBtn1, mouseGlobal) ? btnHover : btnNormal);
                    subBtn2.setFillColor(isHovering(subBtn2, mouseGlobal) ? btnHover : btnNormal);
                    
                    window.draw(subBtn1); window.draw(sub1_A);
                    window.draw(subBtn2); window.draw(sub1_B);

                    if (selectingOrigin || selectingDest) {
                        sf::RectangleShape dropBg(sf::Vector2f(340, 250));
                        int yPosition = (selectingOrigin) ? fieldOrigin.getPosition().y +45 : fieldDest.getPosition().y +45;
                        dropBg.setPosition(panelX + 30, yPosition); 
                        dropBg.setFillColor(sf::Color(40, 50, 60));
                        dropBg.setOutlineColor(sf::Color::Cyan); dropBg.setOutlineThickness(1);
                        window.draw(dropBg);

                        int visibleIndex = 0;
                        int skip = 0;
                        for(int i=0; i<graph.size; i++) {
                             if(selectingOrigin && i == selectedDestIndex) continue;
                             if(selectingDest && i == selectedOriginIndex) continue;
                             
                             if(skip < listOffset) { skip++; continue; }
                             if(visibleIndex >= 8) break;

                             sf::Text itemTxt(graph.vertices[i].port.name, font, 14);
                             itemTxt.setPosition(panelX + 40, yPosition + visibleIndex*32 + 5);
                             
                             sf::FloatRect itemRect(panelX+30, yPosition + visibleIndex*32, 340, 32);
                             if(itemRect.contains(mouseGlobal)) {
                                 sf::RectangleShape highlight(sf::Vector2f(338, 32));
                                 highlight.setPosition(panelX+31, yPosition + visibleIndex*32);
                                 highlight.setFillColor(sf::Color(0, 100, 150));
                                 window.draw(highlight);
                             }
                             
                             window.draw(itemTxt);
                             visibleIndex++;
                        }
                    } 
                    else {
                        window.draw(resultsArea);
                        window.draw(resultHeader);
                        
                        stringstream ss;
                        if(currentPathResult && currentPathResult->found) {
                            ss << resultTextString << "\n\n";
                            ss << "Total Cost: $" << currentPathResult->totalCost << "\n";
                            int totalMins = currentPathResult->totalTime;
                            int days = totalMins / (24 * 60);
                            int hours = (totalMins % (24 * 60)) / 60;
                            int mins = totalMins % 60;
                            ss << "Total Time: " << days << "d " << hours << "h " << mins << "m\n";
                            ss << "Stops: " << currentPathResult->path.getSize() - 1;
                        } 
                        else if (currentPathResult && !currentPathResult->found) {
                             ss << "Route impossible.\nTry different ports.";
                        }
                        else {
                            ss << "Select Origin and \nDestination above\nto see details.";
                        }
                        resultBody.setString(ss.str());
                        window.draw(resultBody);
                    }
                }
            }
        }
        window.display();
    }
    return 0;
}