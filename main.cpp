// project.cpp
// Full integrated project with pathFinding.h usage (uses pathFinding.h as you requested)

#include <iostream>
#include "linkedList.h"
#include <SFML/Graphics.hpp>
#include "Graph.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <string>

using namespace std;


int main() {
    Graph graph;
    graph.addPorts("PortCharges.txt");
    graph.addRoutes("Routes.txt");

    if (graph.size == 0) {
        cerr << "Error: No ports loaded.\n";
        return 1;
    }

    const unsigned int winW = 1600;
    const unsigned int winH = 900;

    sf::Texture mapTexture;
    if (!mapTexture.loadFromFile("world_map.png")) {
        cerr << "ERROR: world_map.png missing!\n";
    }

    sf::Sprite mapSprite(mapTexture);
    sf::Vector2u texSize = mapTexture.getSize();
    mapSprite.setScale(float(winW) / texSize.x, float(winH) / texSize.y);

    sf::RenderWindow window(sf::VideoMode(winW, winH), "OceanRoute Nav Optimizer (SFML)");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("Arial.ttf")) {
        cerr << "ERROR: Font missing! (Arial.ttf)\n";
        return 1;
    }

    vector<sf::Vector2f> positions = {
        {1150.f, 330.f},   // Chittagong
        {1050.f, 300.f},  // Karachi
        {730.f, 170.f},  // Dublin
        {790.f, 200.f},   // Marseille
        {925.f, 330.f},  // Jeddah
        {1120.f, 420.f},   // Colombo
        {1400.f, 630.f},  // Sydney
        {1350.f, 650.f},  // Melbourne
        {490.f, 200.f},   // Montreal
        {970.f, 310.f},  // Doha
        {1275.f, 430.f},  // Manila
        {880.f, 280.f},   // Alexandria
        {920.f, 220.f},   // Istanbul
        {1200.f, 450.f},  // Singapore
        {1300.f, 250.f},  // Busan
        {1000.f, 320.f},  // AbuDhabi
        {420.f, 130.f},   // Antwerp
        {850.f, 230.f},   // Athens
        {820.f, 620.f},   // CapeTown
        {810.f, 130.f},   // Copenhagen
        {1000.f, 360.f},  // Dubai
        {880.f, 620.f},   // Durban
        {790.f, 250.f},   // Genoa
        {860.f, 160.f},  // Hamburg
        {1250.f, 150.f},  // Helsinki
        {1275.f, 350.f},  // HongKong
        {1250.f, 500.f},  // Jakarta
        {730.f, 250.f},   // Lisbon
        {400.f, 300.f},   // London
        {250.f, 300.f},   // LosAngeles
        {1070.f, 350.f},  // Mumbai
        {400.f, 250.f},   // NewYork
        {550.f, 600.f},   // Osaka
        {950.f, 80.f},   // Oslo
        {925.f, 500.f},  // PortLouis
        {700.f, 350.f},   // Rotterdam
        {1275.f, 300.f},  // Shanghai
        {800.f, 450.f},   // Stockholm
        {1350.f, 200.f},  // Tokyo
        {1500.f, 550.f}   // Vancouver
    };

    vector<sf::CircleShape> nodes(graph.size);
    vector<sf::Text> labels(graph.size);
    float nodeRadius = 6.f; 

    // --------------------------
    // Initialize nodes and labels
    // --------------------------
    for (int i = 0; i < graph.size; i++) {
        // Node
        nodes[i].setRadius(nodeRadius);
        nodes[i].setOrigin(nodeRadius, nodeRadius);
        if (i < (int)positions.size()) nodes[i].setPosition(positions[i]);
        nodes[i].setFillColor(sf::Color::Cyan);
        nodes[i].setOutlineColor(sf::Color::Black);
        nodes[i].setOutlineThickness(1.f);

        // Label
        labels[i].setFont(font);
        labels[i].setString(graph.vertices[i].port.name);
        labels[i].setCharacterSize(14); 
        labels[i].setFillColor(sf::Color::White); 
        labels[i].setOutlineColor(sf::Color::Black);
        labels[i].setOutlineThickness(1.f);

        auto b = labels[i].getLocalBounds();
        labels[i].setOrigin(b.width / 2, b.height + 5); 
        if (i < (int)positions.size()) labels[i].setPosition(positions[i].x, positions[i].y - nodeRadius - 5);
    }

    const float panelWidth = 350;
    float panelX = -panelWidth;
    bool panelOpen = false;
    int currentMenu = 0; // 0: Main Menu, 1: Menu 1 Sub, 2: Menu 2 Sub, 3: Menu 3 Sub.

    sf::RectangleShape panel(sf::Vector2f(panelWidth, winH));
    panel.setFillColor(sf::Color(40,40,40, 240)); 
    panel.setPosition(panelX, 0);

    // --- STATIC TOGGLE BUTTON (To open the panel) ---
    sf::RectangleShape toggleBtn(sf::Vector2f(70, 40));
    sf::Text toggleBtnTxt("MENU", font, 18);
    toggleBtn.setFillColor(sf::Color(0, 100, 0)); 
    toggleBtnTxt.setFillColor(sf::Color::White);
    toggleBtn.setPosition(10, 0); // Fixed position
    toggleBtnTxt.setPosition(18, 10);
    // --- END STATIC TOGGLE BUTTON ---


    // Close Button (For Main Menu, inside the panel)
    sf::RectangleShape closeBtn(sf::Vector2f(40,40));
    sf::Text closeBtnTxt("X", font, 24);
    closeBtn.setFillColor(sf::Color(150, 0, 0)); 
    closeBtnTxt.setFillColor(sf::Color::White);
    closeBtn.setPosition(10, 10);
    closeBtnTxt.setPosition(20,15);

    // Back Button (For Submenus, inside the panel)
    sf::RectangleShape backBtn(sf::Vector2f(50,40));
    sf::Text backBtnTxt("<-", font, 24);
    backBtn.setFillColor(sf::Color(0, 100, 150)); 
    backBtnTxt.setFillColor(sf::Color::White);
    backBtn.setPosition(10, 10);
    backBtnTxt.setPosition(18,15);
    
    // --- Main Menu Items ---
    sf::Vector2f menuSize(320.f, 60.f);
    float menuYStart = 100.f;
    float spacing = 20.f;

    sf::RectangleShape menuRect1(menuSize);
    sf::RectangleShape menuRect2(menuSize);
    sf::RectangleShape menuRect3(menuSize);
    
    sf::Color menuColor(70, 70, 70);
    menuRect1.setFillColor(menuColor);
    menuRect2.setFillColor(menuColor);
    menuRect3.setFillColor(menuColor);
    
    // Position menu items with vertical spacing
    menuRect1.setPosition(20, menuYStart);
    menuRect2.setPosition(20, menuYStart + menuSize.y + spacing);
    menuRect3.setPosition(20, menuYStart + 2 * (menuSize.y + spacing));

    sf::Text menu1("1. Shortest/Cheapest Route Finder", font, 18);
    sf::Text menu2("2. Ship Route Booking & Connections", font, 18);
    sf::Text menu3("3. Custom Ship Preferences & Filters", font, 18);
    
    sf::Color menuTxtColor(sf::Color::White);
    menu1.setFillColor(menuTxtColor);
    menu2.setFillColor(menuTxtColor);
    menu3.setFillColor(menuTxtColor);

    // Text centering within the button rectangles
    menu1.setPosition(25, menuRect1.getPosition().y + 10);
    menu2.setPosition(25, menuRect2.getPosition().y + 10);
    menu3.setPosition(25, menuRect3.getPosition().y + 10);

    
    // --- Submenu Items ---
    
    // Submenu 1 (Route Finder)
    sf::RectangleShape submenuRect1(menuSize);
    sf::RectangleShape submenuRect2(menuSize);
    sf::Text sub1_A("A. Find Shortest TIME Route", font, 18);
    sf::Text sub1_B("B. Find Cheapest COST Route", font, 18);
    submenuRect1.setFillColor(menuColor);
    submenuRect2.setFillColor(menuColor);
    // submenuRect1.setPosition(20)
    
    // Submenu 2 (Booking)
    sf::Text sub2_A("A. Select Route and Date", font, 18);
    sf::Text sub2_B("B. View Connecting Routes", font, 18);
    
    // Submenu 3 (Preferences)
    sf::Text sub3_A("A. Filter by Shipping Company", font, 18);
    sf::Text sub3_B("B. Avoid Specific Port", font, 18);

    sf::Color subTxtColor(sf::Color::White);
    sub1_A.setFillColor(subTxtColor);
    sub1_B.setFillColor(subTxtColor);
    sub2_A.setFillColor(subTxtColor);
    sub2_B.setFillColor(subTxtColor);
    sub3_A.setFillColor(subTxtColor);
    sub3_B.setFillColor(subTxtColor);

    float subYStart = 160.f;


    bool selectingOrigin = false;
    bool selectingDest = false;
    int selectedOriginIndex = -1;
    int selectedDestIndex = -1;

    // Scroll state for port list inside dropdown
    int listOffset = 0;
    const int maxVisibleItems = 8;
    const float portItemHeight = 30.f;
    const float portListStartY = 260.f; // relative to panel

    // Helper rectangles for Origin/Dest fields
    sf::RectangleShape fieldOrigin(sf::Vector2f(300.f, 36.f));
    sf::RectangleShape fieldDest(sf::Vector2f(300.f, 36.f));
    fieldOrigin.setFillColor(sf::Color(55,55,55));
    fieldDest.setFillColor(sf::Color(55,55,55));
    fieldOrigin.setOutlineColor(sf::Color::White);
    fieldDest.setOutlineColor(sf::Color::White);
    fieldOrigin.setOutlineThickness(1.f);
    fieldDest.setOutlineThickness(1.f);

    sf::Text originLabel("Origin: (click to choose)", font, 16);
    sf::Text destLabel("Destination: (click to choose)", font, 16);
    originLabel.setFillColor(sf::Color::White);
    destLabel.setFillColor(sf::Color::White);

    // Scroll hint / up-down rectangles
    sf::RectangleShape upBtn(sf::Vector2f(30, 24));
    sf::RectangleShape downBtn(sf::Vector2f(30, 24));
    upBtn.setFillColor(sf::Color(100,100,100));
    downBtn.setFillColor(sf::Color(100,100,100));
    sf::Text upTxt("^", font, 16), downTxt("v", font, 16);
    upTxt.setFillColor(sf::Color::White); downTxt.setFillColor(sf::Color::White);


    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) window.close();

            // Toggle panel using 'P' key
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::P) {
                if (!selectingOrigin && !selectingDest) panelOpen = !panelOpen;
            }

            // Scroll for port list
            if (e.type == sf::Event::MouseWheelScrolled) {
                if (panelOpen && (selectingOrigin || selectingDest)) {
                    if (e.mouseWheelScroll.delta > 0) listOffset--;
                    else listOffset++;
                    if (listOffset < 0) listOffset = 0;
                    int available = graph.size - ((selectingOrigin && selectedDestIndex!=-1) || (selectingDest && selectedOriginIndex!=-1) ? 1 : 0);
                    if (listOffset > std::max(0, available - maxVisibleItems)) listOffset = std::max(0, available - maxVisibleItems);
                }
            }

            if (e.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mouseGlobal(sf::Mouse::getPosition(window));

                // STATIC TOGGLE BUTTON
                if (!panelOpen && toggleBtn.getGlobalBounds().contains(mouseGlobal)) {
                    panelOpen = true;
                    continue;
                }

                if (panelOpen) {
                    sf::Vector2f panelMouse(mouseGlobal.x - panelX, mouseGlobal.y);

                    // Origin/Destination dropdown selection
                    if (selectingOrigin || selectingDest) {
                        int visibleCount = 0;
                        int skipCount = 0;
                        for (int realIndex = 0; realIndex < graph.size; realIndex++) {
                            if (selectingOrigin && selectedDestIndex == realIndex) continue;
                            if (selectingDest && selectedOriginIndex == realIndex) continue;
                            if (skipCount < listOffset) { skipCount++; continue; }
                            if (visibleCount >= maxVisibleItems) break;

                            float y = portListStartY + visibleCount * (portItemHeight + 6);
                            sf::FloatRect itemRect(panelX + 40, y, 270.f, portItemHeight);

                            if (itemRect.contains(mouseGlobal)) {
                                if (selectingOrigin) {
                                    selectedOriginIndex = realIndex;
                                    if (selectedDestIndex == selectedOriginIndex) selectedDestIndex = -1;
                                    selectingOrigin = false;
                                } else {
                                    selectedDestIndex = realIndex;
                                    if (selectedOriginIndex == selectedDestIndex) selectedOriginIndex = -1;
                                    selectingDest = false;
                                }
                                listOffset = 0;
                                break;
                            }
                            visibleCount++;
                        }

                        float listTop = portListStartY;
                        float listBottom = portListStartY + maxVisibleItems * (portItemHeight + 6);
                        if (!(panelMouse.x >= 40 && panelMouse.x <= 40 + 270.f &&
                            panelMouse.y >= listTop && panelMouse.y <= listBottom)) {
                            selectingOrigin = selectingDest = false;
                            listOffset = 0;
                        }
                        continue; // consume event
                    }

                    // Close button
                    if (currentMenu == 0 && closeBtn.getGlobalBounds().contains(mouseGlobal)) {
                        panelOpen = false;
                        continue;
                    }

                    // Back button
                    if (currentMenu != 0 && backBtn.getGlobalBounds().contains(mouseGlobal)) {
                        currentMenu = 0;
                        selectingOrigin = selectingDest = false;
                        listOffset = 0;
                        continue;
                    }

                    // Main menu selection
                    if (currentMenu == 0) {
                        if (menuRect1.getGlobalBounds().contains(mouseGlobal)) { currentMenu = 1; continue; }
                        if (menuRect2.getGlobalBounds().contains(mouseGlobal)) { currentMenu = 2; continue; }
                        if (menuRect3.getGlobalBounds().contains(mouseGlobal)) { currentMenu = 3; continue; }
                    } 
                    // Submenu handling
                    else if (currentMenu == 1) {
                        if (fieldOrigin.getGlobalBounds().contains(mouseGlobal)) { selectingOrigin = true; selectingDest = false; continue; }
                        if (fieldDest.getGlobalBounds().contains(mouseGlobal)) { selectingDest = true; selectingOrigin = false; continue; }
                        
                    }
                }
            }
        }

        // Animate panel
        if (panelOpen && panelX < 0) panelX += 20;
        if (!panelOpen && panelX > -panelWidth) panelX -= 20;
        if (panelX > 0) panelX = 0;
        if (panelX < -panelWidth) panelX = -panelWidth;
        
        // Update positions based on panelX
        panel.setPosition(panelX, 0);
        closeBtn.setPosition(panelX + 10, 10);
        closeBtnTxt.setPosition(panelX + 20, 15);
        backBtn.setPosition(panelX + 10, 10);
        backBtnTxt.setPosition(panelX + 18, 15);
        
        // Update Main Menu positions
        menuRect1.setPosition(panelX + 20, menuYStart);
        menuRect2.setPosition(panelX + 20, menuYStart + menuSize.y + spacing);
        menuRect3.setPosition(panelX + 20, menuYStart + 2 * (menuSize.y + spacing));
        menu1.setPosition(panelX + 25, menuRect1.getPosition().y + 10);
        menu2.setPosition(panelX + 25, menuRect2.getPosition().y + 10);
        menu3.setPosition(panelX + 25, menuRect3.getPosition().y + 10);
        
        // Update Sub Menu positions
        submenuRect1.setPosition(panelX + 20, subYStart + 0 * (menuSize.y + spacing) + 150);
        submenuRect2.setPosition(panelX + 20, subYStart + 1 * (menuSize.y + spacing) + 150);
        sub1_A.setPosition(submenuRect1.getPosition().x + 5 , submenuRect1.getPosition().y + 10) ;
        sub1_B.setPosition(submenuRect2.getPosition().x + 5 , submenuRect2.getPosition().y + 10);
        
        
        sub2_A.setPosition(panelX + 40, subYStart + 0 * (menuSize.y + spacing)+ 150);
        sub2_B.setPosition(panelX + 40, subYStart + 1 * (menuSize.y + spacing)+ 150);
        
        sub3_A.setPosition(panelX + 40, subYStart + 0 * (menuSize.y + spacing)+ 150);
        sub3_B.setPosition(panelX + 40, subYStart + 1 * (menuSize.y + spacing)+ 150);

        // Update Origin/Dest field positions
        fieldOrigin.setPosition(panelX + 20, subYStart + 10);
        fieldDest.setPosition(panelX + 20, subYStart + 60);
        originLabel.setPosition(panelX + 26, subYStart + 14);
        destLabel.setPosition(panelX + 26, subYStart + 64);

        // Update scroll buttons positions
        upBtn.setPosition(panelX + 310, portListStartY - 6);
        downBtn.setPosition(panelX + 310, portListStartY + maxVisibleItems * (portItemHeight + 6) - 6);
        upTxt.setPosition(upBtn.getPosition().x + 8, upBtn.getPosition().y + 2);
        downTxt.setPosition(downBtn.getPosition().x + 8, downBtn.getPosition().y + 2);

        window.clear();
        window.draw(mapSprite);

        // Draw nodes and labels (hide leftmost region under panel when open)
        for (int i = 0; i < graph.size; i++) {
            if (!panelOpen || nodes[i].getPosition().x > panelWidth) {
                // If path includes this node, highlight it
                
                nodes[i].setFillColor(sf::Color::Cyan);
                nodes[i].setRadius(nodeRadius);
                nodes[i].setOrigin(nodeRadius, nodeRadius);

                window.draw(nodes[i]);
                window.draw(labels[i]);
            }
        }


        // Draw Panel and Menu Items
        window.draw(panel);
        
        if (currentMenu == 0) { // MAIN MENU
            window.draw(closeBtn);
            window.draw(closeBtnTxt);
            window.draw(menuRect1);
            window.draw(menuRect2);
            window.draw(menuRect3);
            window.draw(menu1);
            window.draw(menu2);
            window.draw(menu3);
        } else { // SUBMENU VIEW
            window.draw(backBtn);
            window.draw(backBtnTxt);
            
            // Draw Submenu items based on currentMenu state
            if (currentMenu == 1) { // Route Finder
                sf::Text header("Route Finder: Select Metric", font, 20);
                header.setFillColor(sf::Color::White);
                header.setPosition(panelX + 15, subYStart - 40);
                window.draw(header);
                
                window.draw(submenuRect1);
                window.draw(submenuRect2);
                window.draw(sub1_A);
                window.draw(sub1_B);

                // Draw Origin/Destination fields
                window.draw(fieldOrigin);
                window.draw(fieldDest);

                // Draw current selections (if any)
                if (selectedOriginIndex >= 0) {
                    sf::Text t(graph.vertices[selectedOriginIndex].port.name, font, 16);
                    t.setFillColor(sf::Color::White);
                    t.setPosition(panelX + 26, subYStart + 14);
                    window.draw(t);
                } else {
                    window.draw(originLabel);
                }

                if (selectedDestIndex >= 0) {
                    sf::Text t2(graph.vertices[selectedDestIndex].port.name, font, 16);
                    t2.setFillColor(sf::Color::White);
                    t2.setPosition(panelX + 26, subYStart + 64);
                    window.draw(t2);
                } else {
                    window.draw(destLabel);
                }

                // If selectingOrigin or selectingDest draw the dropdown list
                if (selectingOrigin || selectingDest) {
                    // Background for list
                    sf::RectangleShape listBg(sf::Vector2f(300.f, maxVisibleItems * (portItemHeight + 6)));
                    listBg.setFillColor(sf::Color(60,60,60));
                    listBg.setPosition(panelX + 20, portListStartY);
                    window.draw(listBg);

                    // Up/down buttons
                    window.draw(upBtn);
                    window.draw(downBtn);
                    window.draw(upTxt);
                    window.draw(downTxt);

                    // draw visible items
                    int visibleCount = 0;
                    int skipCount = 0;
                    for (int realIndex = 0; realIndex < graph.size; realIndex++) {
                        // Exclude the opposite selection from the list
                        if (selectingOrigin && selectedDestIndex == realIndex) continue;
                        if (selectingDest && selectedOriginIndex == realIndex) continue;

                        // We only start drawing from listOffset
                        if (skipCount < listOffset) { skipCount++; continue; }
                        if (visibleCount >= maxVisibleItems) break;

                        float y = portListStartY + visibleCount * (portItemHeight + 6);
                        sf::RectangleShape itemRect(sf::Vector2f(270.f, portItemHeight));
                        itemRect.setPosition(panelX + 40, y);
                        // Highlight the currently selected one (if matches)
                        if (realIndex == selectedOriginIndex || realIndex == selectedDestIndex) {
                            itemRect.setFillColor(sf::Color(90, 160, 90));
                        } else {
                            itemRect.setFillColor(sf::Color(90,90,90));
                        }
                        itemRect.setOutlineColor(sf::Color::White);
                        itemRect.setOutlineThickness(1.f);
                        window.draw(itemRect);

                        sf::Text nameTxt(graph.vertices[realIndex].port.name, font, 15);
                        nameTxt.setFillColor(sf::Color::White);
                        nameTxt.setPosition(panelX + 46, y + 4);
                        window.draw(nameTxt);

                        visibleCount++;
                    }
                }

            } else if (currentMenu == 2) { // Booking
                sf::Text header("Booking: Select Ports and Date", font, 20);
                header.setFillColor(sf::Color::White);
                header.setPosition(panelX + 15, subYStart - 40);
                window.draw(header);

                window.draw(sub2_A);
                window.draw(sub2_B);

                // Draw Origin/Destination fields (same behavior)
                window.draw(fieldOrigin);
                window.draw(fieldDest);

                if (selectedOriginIndex >= 0) {
                    sf::Text t(graph.vertices[selectedOriginIndex].port.name, font, 16);
                    t.setFillColor(sf::Color::White);
                    t.setPosition(panelX + 26, subYStart + 14);
                    window.draw(t);
                } else {
                    window.draw(originLabel);
                }

                if (selectedDestIndex >= 0) {
                    sf::Text t2(graph.vertices[selectedDestIndex].port.name, font, 16);
                    t2.setFillColor(sf::Color::White);
                    t2.setPosition(panelX + 26, subYStart + 64);
                    window.draw(t2);
                } else {
                    window.draw(destLabel);
                }

                if (selectingOrigin || selectingDest) {
                    sf::RectangleShape listBg(sf::Vector2f(300.f, maxVisibleItems * (portItemHeight + 6)));
                    listBg.setFillColor(sf::Color(60,60,60));
                    listBg.setPosition(panelX + 20, portListStartY);
                    window.draw(listBg);

                    window.draw(upBtn);
                    window.draw(downBtn);
                    window.draw(upTxt);
                    window.draw(downTxt);

                    int visibleCount = 0;
                    int skipCount = 0;
                    for (int realIndex = 0; realIndex < graph.size; realIndex++) {
                        if (selectingOrigin && selectedDestIndex == realIndex) continue;
                        if (selectingDest && selectedOriginIndex == realIndex) continue;

                        if (skipCount < listOffset) { skipCount++; continue; }
                        if (visibleCount >= maxVisibleItems) break;

                        float y = portListStartY + visibleCount * (portItemHeight + 6);
                        sf::RectangleShape itemRect(sf::Vector2f(270.f, portItemHeight));
                        itemRect.setPosition(panelX + 40, y);
                        itemRect.setFillColor(sf::Color(90,90,90));
                        itemRect.setOutlineColor(sf::Color::White);
                        itemRect.setOutlineThickness(1.f);
                        window.draw(itemRect);

                        sf::Text nameTxt(graph.vertices[realIndex].port.name, font, 15);
                        nameTxt.setFillColor(sf::Color::White);
                        nameTxt.setPosition(panelX + 46, y + 4);
                        window.draw(nameTxt);

                        visibleCount++;
                    }
                }

            } else if (currentMenu == 3) { // Preferences
                sf::Text header("Preferences: Apply Filters", font, 20);
                header.setFillColor(sf::Color::White);
                header.setPosition(panelX + 15, subYStart - 40);
                window.draw(header);

                window.draw(sub3_A);
                window.draw(sub3_B);

                // Draw Origin/Destination fields (same behavior)
                window.draw(fieldOrigin);
                window.draw(fieldDest);

                if (selectedOriginIndex >= 0) {
                    sf::Text t(graph.vertices[selectedOriginIndex].port.name, font, 16);
                    t.setFillColor(sf::Color::White);
                    t.setPosition(panelX + 26, subYStart + 14);
                    window.draw(t);
                } else {
                    window.draw(originLabel);
                }

                if (selectedDestIndex >= 0) {
                    sf::Text t2(graph.vertices[selectedDestIndex].port.name, font, 16);
                    t2.setFillColor(sf::Color::White);
                    t2.setPosition(panelX + 26, subYStart + 64);
                    window.draw(t2);
                } else {
                    window.draw(destLabel);
                }

                if (selectingOrigin || selectingDest) {
                    sf::RectangleShape listBg(sf::Vector2f(300.f, maxVisibleItems * (portItemHeight + 6)));
                    listBg.setFillColor(sf::Color(60,60,60));
                    listBg.setPosition(panelX + 20, portListStartY);
                    window.draw(listBg);

                    window.draw(upBtn);
                    window.draw(downBtn);
                    window.draw(upTxt);
                    window.draw(downTxt);

                    int visibleCount = 0;
                    int skipCount = 0;
                    for (int realIndex = 0; realIndex < graph.size; realIndex++) {
                        if (selectingOrigin && selectedDestIndex == realIndex) continue;
                        if (selectingDest && selectedOriginIndex == realIndex) continue;

                        if (skipCount < listOffset) { skipCount++; continue; }
                        if (visibleCount >= maxVisibleItems) break;

                        float y = portListStartY + visibleCount * (portItemHeight + 6);
                        sf::RectangleShape itemRect(sf::Vector2f(270.f, portItemHeight));
                        itemRect.setPosition(panelX + 40, y);
                        itemRect.setFillColor(sf::Color(90,90,90));
                        itemRect.setOutlineColor(sf::Color::White);
                        itemRect.setOutlineThickness(1.f);
                        window.draw(itemRect);

                        sf::Text nameTxt(graph.vertices[realIndex].port.name, font, 15);
                        nameTxt.setFillColor(sf::Color::White);
                        nameTxt.setPosition(panelX + 46, y + 4);
                        window.draw(nameTxt);

                        visibleCount++;
                    }
                }
            }
        }
        
        // --- DRAW THE STATIC TOGGLE BUTTON IF THE PANEL IS CLOSED ---
        if (!panelOpen) {
            window.draw(toggleBtn);
            window.draw(toggleBtnTxt);
        }
        // --- END DRAW STATIC TOGGLE BUTTON ---
        
        window.display();
    }

    return 0;
}
