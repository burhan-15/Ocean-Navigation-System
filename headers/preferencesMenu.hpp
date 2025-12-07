#ifndef PREFERENCESMENU_HPP
#define PREFERENCESMENU_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include "Graph.hpp"
#include "routeFilter.hpp"
#include "uiHelpers.hpp"
#include "vector.h"

struct PreferencesMenu {
    // 1. Origin and Destination selection
    sf::RectangleShape fieldOrigin;
    sf::RectangleShape fieldDest;
    sf::Text originLabel;
    sf::Text destLabel;
    bool selectingOrigin;
    bool selectingDest;
    int selectedOriginIndex;
    int selectedDestIndex;
    int originDestListOffset;
    
    // 2. Preferred ports (for layovers)
    sf::RectangleShape portField;
    sf::Text portLabel;
    bool selectingPorts;
    Vector<int> selectedPorts;
    int portListOffset;
    
    // 3. Preferred companies (for route segments)
    sf::RectangleShape companyField;
    sf::Text companyLabel;
    bool selectingCompanies;
    Vector<std::string> selectedCompanies;
    Vector<std::string> allCompanies;
    int companyListOffset;
    
    // 4. Show Routes button
    sf::RectangleShape showRoutesBtn;
    sf::Text showRoutesTxt;
    
    // 5. Navigation buttons
    sf::RectangleShape prevBtn;
    sf::RectangleShape nextBtn;
    sf::Text prevBtnTxt;
    sf::Text nextBtnTxt;
    sf::Text routeCounterTxt;
    
    // 6. Filtered routes
    Vector<PathFinding::PathResult*> filteredRoutes;
    int currentRouteIndex;
    
    // 7. Results display
    sf::RectangleShape resultsArea;
    sf::Text resultHeader;
    sf::Text resultBody;
    
    // Tags for selected ports and companies
    Vector<sf::RectangleShape> portTags;
    Vector<sf::Text> portTagTexts;
    Vector<sf::RectangleShape> companyTags;
    Vector<sf::Text> companyTagTexts;
    
    sf::Color btnNormal;
    sf::Color btnHover;
    sf::Color selectedColor;
    sf::Color tagColor;

    // --- CONSTRUCTOR: FIXED INITIALIZATION ORDER ---
    PreferencesMenu(const Graph& graph, const sf::Font& font)
        : fieldOrigin(sf::Vector2f(340.f, 40.f)),
          fieldDest(sf::Vector2f(340.f, 40.f)),
          selectingOrigin(false),
          selectingDest(false),
          selectedOriginIndex(-1),
          selectedDestIndex(-1),
          originDestListOffset(0),
          portField(sf::Vector2f(340.f, 40.f)),
          selectingPorts(false),
          portListOffset(0),
          companyField(sf::Vector2f(340.f, 40.f)),
          selectingCompanies(false),
          companyListOffset(0),
          showRoutesBtn(sf::Vector2f(340.f, 50.f)),
          prevBtn(sf::Vector2f(50.f, 40.f)),
          nextBtn(sf::Vector2f(50.f, 40.f)),
          currentRouteIndex(-1),
          resultsArea(sf::Vector2f(360.f, 250.f)),
          btnNormal(70, 80, 100),
          btnHover(90, 100, 125),
          selectedColor(0, 150, 200),
          tagColor(50, 100, 150) {
        
        // Get all companies
        allCompanies = RouteFilter::getAllCompanies(graph);
        
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
        
        // Setup port field (for layovers)
        portField.setFillColor(sf::Color(30, 40, 50));
        portField.setOutlineColor(sf::Color(120, 120, 120));
        portField.setOutlineThickness(1.f);
        
        portLabel.setFont(font);
        portLabel.setString("Select Preferred Layover Ports...");
        portLabel.setCharacterSize(15);
        portLabel.setFillColor(sf::Color(150, 150, 150));
        
        // Setup company field
        companyField.setFillColor(sf::Color(30, 40, 50));
        companyField.setOutlineColor(sf::Color(120, 120, 120));
        companyField.setOutlineThickness(1.f);
        
        companyLabel.setFont(font);
        companyLabel.setString("Select Preferred Companies...");
        companyLabel.setCharacterSize(15);
        companyLabel.setFillColor(sf::Color(150, 150, 150));
        
        // Setup show routes button
        showRoutesBtn.setFillColor(btnNormal);
        showRoutesTxt.setFont(font);
        showRoutesTxt.setString("Show Routes");
        showRoutesTxt.setCharacterSize(16);
        showRoutesTxt.setFillColor(sf::Color::White);
        
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
        resultHeader.setString("FILTERED ROUTES");
        resultHeader.setCharacterSize(14);
        resultHeader.setFillColor(sf::Color(0, 255, 200));
        
        resultBody.setFont(font);
        resultBody.setString("Select origin, destination,\nand preferences");
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
        portField.setPosition(panelX + 30, startY + 100);
        portLabel.setPosition(panelX + 40, startY + 110);
        companyField.setPosition(panelX + 30, startY + 150);
        companyLabel.setPosition(panelX + 40, startY + 160);
        showRoutesBtn.setPosition(panelX + 30, startY + 200);
        showRoutesTxt.setPosition(panelX + 150, startY + 215);
        
        // Navigation buttons (position will be adjusted in draw based on tags)
        prevBtn.setPosition(panelX + 30, winH - 230);
        nextBtn.setPosition(panelX + 320, winH - 230);
        prevBtnTxt.setPosition(panelX + 45, winH - 225);
        nextBtnTxt.setPosition(panelX + 335, winH - 225);
        routeCounterTxt.setPosition(panelX + 100, winH - 225);
        
        // Results area position (will be adjusted dynamically based on tags)
        resultsArea.setPosition(panelX + 20, winH - 180);
        resultHeader.setPosition(panelX + 35, winH - 180 + 15);
        resultBody.setPosition(panelX + 35, winH - 180 + 50);
    }

    void handleMouseWheel(float delta, bool isOriginDest, bool isPortSelection, const Graph& graph) {
        if (isOriginDest) {
            int availableItems = 0;
            for (int i = 0; i < graph.size; i++) {
                if ((selectingOrigin && i == selectedDestIndex) || 
                    (selectingDest && i == selectedOriginIndex)) continue;
                availableItems++;
            }
            int maxOffset = std::max(0, availableItems - 6);
            
            if (delta > 0) originDestListOffset--;
            else originDestListOffset++;
            if (originDestListOffset < 0) originDestListOffset = 0;
            if (originDestListOffset > maxOffset) originDestListOffset = maxOffset;
        } else if (isPortSelection) {
            int availableItems = 0;
            for (int i = 0; i < graph.size; i++) {
                if (i == selectedOriginIndex || i == selectedDestIndex) continue;
                availableItems++;
            }
            int maxOffset = std::max(0, availableItems - 6);
            
            if (delta > 0) portListOffset--;
            else portListOffset++;
            if (portListOffset < 0) portListOffset = 0;
            if (portListOffset > maxOffset) portListOffset = maxOffset;
        } else {
            int maxOffset = std::max(0, (int)allCompanies.size() - 6);
            
            if (delta > 0) companyListOffset--;
            else companyListOffset++;
            if (companyListOffset < 0) companyListOffset = 0;
            if (companyListOffset > maxOffset) companyListOffset = maxOffset;
        }
    }

    // --- FIX: ADDED FONT PARAMETER FOR ACCURATE HITBOXES ---
    bool handleClick(Graph& graph,
                    const sf::Vector2f& mouseGlobal,
                    float panelX,
                    float winH,
                    const sf::Font& font, // Required for calculating layout
                    PathFinding::PathResult*& currentPathResult,
                    std::string& resultTextString) {
        // Handle origin/destination selection
        if (selectingOrigin || selectingDest) {
            return handleOriginDestSelectionClick(graph, mouseGlobal, panelX);
        }
        else if (selectingPorts) {
            return handlePortSelectionClick(graph, mouseGlobal, panelX);
        }
        else if (selectingCompanies) {
            return handleCompanySelectionClick(mouseGlobal, panelX);
        }
        else if (fieldOrigin.getGlobalBounds().contains(mouseGlobal)) {
            selectingOrigin = true;
            selectingDest = selectingPorts = selectingCompanies = false;
            return true;
        }
        else if (fieldDest.getGlobalBounds().contains(mouseGlobal)) {
            selectingDest = true;
            selectingOrigin = selectingPorts = selectingCompanies = false;
            return true;
        }
        else if (portField.getGlobalBounds().contains(mouseGlobal)) {
            selectingPorts = true;
            selectingOrigin = selectingDest = selectingCompanies = false;
            return true;
        }
        else if (companyField.getGlobalBounds().contains(mouseGlobal)) {
            selectingCompanies = true;
            selectingOrigin = selectingDest = selectingPorts = false;
            return true;
        }
        
        // --- FIX: CALCULATE EXACT BUTTON POSITION ---
        if (filteredRoutes.size() > 0) {
            float navY = calculateLayoutNavY(graph, font, panelX, winH);
            
            // Define hitboxes exactly where draw() places them
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
        
        // Handle show routes button
        if (showRoutesBtn.getGlobalBounds().contains(mouseGlobal)) {
            if (selectedOriginIndex != -1 && selectedDestIndex != -1) {
                applyFilters(graph, currentPathResult, resultTextString);
            }
            return true;
        }
        
        return false;
    }

    void applyFilters(Graph& graph, 
                     PathFinding::PathResult*& currentPathResult,
                     std::string& resultTextString) {
        bool currentIsInFiltered = false;
        for (int i = 0; i < filteredRoutes.size(); i++) {
            if (filteredRoutes[i] == currentPathResult) {
                currentIsInFiltered = true;
                break;
            }
        }
        
        for (int i = 0; i < filteredRoutes.size(); i++) {
            if (filteredRoutes[i] != currentPathResult) {
                delete filteredRoutes[i];
            }
        }
        filteredRoutes.clear();
        
        if (currentPathResult && currentIsInFiltered) {
            delete currentPathResult;
            currentPathResult = nullptr;
        }
        
        filteredRoutes = RouteFilter::findFilteredRoutes(
            graph, selectedOriginIndex, selectedDestIndex, 
            selectedPorts, selectedCompanies);
        
        currentRouteIndex = -1;
        if (filteredRoutes.size() > 0) {
            currentRouteIndex = 0;
            if (currentPathResult) {
                bool isInNewFiltered = false;
                for (int i = 0; i < filteredRoutes.size(); i++) {
                    if (filteredRoutes[i] == currentPathResult) {
                        isInNewFiltered = true;
                        break;
                    }
                }
                if (!isInNewFiltered) {
                    delete currentPathResult;
                }
            }
            currentPathResult = filteredRoutes[0];
            resultTextString = "Filtered Route";
        } else {
            if (currentPathResult) {
                delete currentPathResult;
                currentPathResult = nullptr;
            }
        }
    }

    void navigatePrevious(PathFinding::PathResult*& currentPathResult,
                         std::string& resultTextString) {
        if (filteredRoutes.size() == 0 || currentRouteIndex <= 0) return;
        
        currentRouteIndex--;
        bool isInFiltered = false;
        for (int i = 0; i < filteredRoutes.size(); i++) {
            if (filteredRoutes[i] == currentPathResult) {
                isInFiltered = true;
                break;
            }
        }
        if (currentPathResult && !isInFiltered) {
            delete currentPathResult;
        }
        currentPathResult = filteredRoutes[currentRouteIndex];
        resultTextString = "Filtered Route";
    }

    void navigateNext(PathFinding::PathResult*& currentPathResult,
                     std::string& resultTextString) {
        if (filteredRoutes.size() == 0 || 
            currentRouteIndex >= filteredRoutes.size() - 1) return;
        
        currentRouteIndex++;
        bool isInFiltered = false;
        for (int i = 0; i < filteredRoutes.size(); i++) {
            if (filteredRoutes[i] == currentPathResult) {
                isInFiltered = true;
                break;
            }
        }
        if (currentPathResult && !isInFiltered) {
            delete currentPathResult;
        }
        currentPathResult = filteredRoutes[currentRouteIndex];
        resultTextString = "Filtered Route";
    }

    void reset() {
        selectingOrigin = selectingDest = selectingPorts = selectingCompanies = false;
        originDestListOffset = portListOffset = companyListOffset = 0;
        currentRouteIndex = -1;
    }

    void cleanup() {
        for (int i = 0; i < filteredRoutes.size(); i++) {
            delete filteredRoutes[i];
        }
        filteredRoutes.clear();
    }

private:
    // --- FIX: HELPER FOR CALCULATING EXACT BUTTON Y POSITION ---
    float calculateLayoutNavY(const Graph& graph, const sf::Font& font, float panelX, float winH) {
        float startY = showRoutesBtn.getPosition().y + showRoutesBtn.getSize().y + 10;
        float tagX = panelX + 30;
        float tagY = startY;
        float maxWidth = 340.f;
        float tagHeight = 25.f;
        float tagSpacing = 5.f;
        float maxY = startY;

        // Simulate Port Tags
        for (int portIdx : selectedPorts) {
            if (portIdx < 0 || portIdx >= graph.size) continue;
            std::string name = graph.vertices[portIdx].port.name;
            sf::Text tempText(name, font, 12);
            float width = tempText.getLocalBounds().width + 20;

            if (tagX + width > panelX + 30 + maxWidth) {
                tagX = panelX + 30;
                tagY += tagHeight + tagSpacing;
            }
            maxY = std::max(maxY, tagY + tagHeight);
            tagX += width + tagSpacing;
        }

        // Simulate Company Tags
        if (selectedCompanies.size() > 0) {
            if (selectedPorts.size() > 0) {
                tagY = maxY + 10; // Gap between groups
                tagX = panelX + 30;
            }
            
            for (int i = 0; i < selectedCompanies.size(); i++) {
                const std::string& company = selectedCompanies[i];
                sf::Text tempText(company, font, 12);
                float width = tempText.getLocalBounds().width + 20;

                if (tagX + width > panelX + 30 + maxWidth) {
                    tagX = panelX + 30;
                    tagY += tagHeight + tagSpacing;
                }
                maxY = std::max(maxY, tagY + tagHeight);
                tagX += width + tagSpacing;
            }
        }
        
        // Match the logic in draw(): resultsY = std::min(winH - 180, tagsEndY + 100);
        float resultsY = std::min(winH - 180, maxY + 100);
        return resultsY - 50; // Navigation buttons are 50px above results
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

    bool handlePortSelectionClick(const Graph& graph,
                                  const sf::Vector2f& mouseGlobal,
                                  float panelX) {
        int const visibleItems = 6;
        float itemH = 30.f;
        float startY = portField.getPosition().y + 45;
        
        bool clickedItem = false;
        int visibleIndex = 0;
        int skip = 0;
        
        for (int i = 0; i < graph.size; i++) {
            if (i == selectedOriginIndex || i == selectedDestIndex) continue;
            
            if (skip < portListOffset) { skip++; continue; }
            if (visibleIndex >= visibleItems) break;
            
            sf::FloatRect itemRect(panelX + 30, startY + visibleIndex * (itemH + 2), 340, itemH);
            if (itemRect.contains(mouseGlobal)) {
                // Find index of port in selectedPorts (linear search)
                int foundIndex = -1;
                for (int j = 0; j < selectedPorts.size(); j++) {
                    if (selectedPorts[j] == i) {
                        foundIndex = j;
                        break;
                    }
                }
                if (foundIndex != -1) {
                    selectedPorts.erase(foundIndex);
                } else {
                    selectedPorts.push_back(i);
                }
                clickedItem = true;
                break;
            }
            visibleIndex++;
        }
        
        if (!clickedItem) {
            selectingPorts = false;
        }
        return true;
    }

    bool handleCompanySelectionClick(const sf::Vector2f& mouseGlobal,
                                    float panelX) {
        int const visibleItems = 6;
        float itemH = 30.f;
        float startY = companyField.getPosition().y + 45;
        
        bool clickedItem = false;
        int visibleIndex = 0;
        int skip = 0;
        
        for (int i = 0; i < allCompanies.size(); i++) {
            if (skip < companyListOffset) { skip++; continue; }
            if (visibleIndex >= visibleItems) break;
            
            sf::FloatRect itemRect(panelX + 30, startY + visibleIndex * (itemH + 2), 340, itemH);
            if (itemRect.contains(mouseGlobal)) {
                // Find index of company in selectedCompanies (linear search)
                int foundIndex = -1;
                for (int j = 0; j < selectedCompanies.size(); j++) {
                    if (selectedCompanies[j] == allCompanies[i]) {
                        foundIndex = j;
                        break;
                    }
                }
                if (foundIndex != -1) {
                    selectedCompanies.erase(foundIndex);
                } else {
                    selectedCompanies.push_back(allCompanies[i]);
                }
                clickedItem = true;
                break;
            }
            visibleIndex++;
        }
        
        if (!clickedItem) {
            selectingCompanies = false;
        }
        return true;
    }

public:
    void draw(sf::RenderWindow& window,
             const Graph& graph,
             const sf::Font& font,
             const sf::Vector2f& mouseGlobal,
             float panelX,
             float winH) {
        window.draw(fieldOrigin);
        window.draw(fieldDest);
        window.draw(portField);
        window.draw(companyField);
        
        if (selectedOriginIndex != -1) {
            originLabel.setString(graph.vertices[selectedOriginIndex].port.name);
            originLabel.setFillColor(sf::Color::White);
        } else {
            originLabel.setString("Select Origin...");
            originLabel.setFillColor(sf::Color(150, 150, 150));
        }
        window.draw(originLabel);
        
        if (selectedDestIndex != -1) {
            destLabel.setString(graph.vertices[selectedDestIndex].port.name);
            destLabel.setFillColor(sf::Color::White);
        } else {
            destLabel.setString("Select Destination...");
            destLabel.setFillColor(sf::Color(150, 150, 150));
        }
        window.draw(destLabel);
        
        if (!selectedPorts.empty()) {
            std::stringstream ss;
            ss << selectedPorts.size() << " layover port(s)";
            portLabel.setString(ss.str());
            portLabel.setFillColor(selectedColor);
        } else {
            portLabel.setString("Select Preferred Layover Ports...");
            portLabel.setFillColor(sf::Color(150, 150, 150));
        }
        window.draw(portLabel);
        
        if (!selectedCompanies.empty()) {
            std::stringstream ss;
            ss << selectedCompanies.size() << " company(ies)";
            companyLabel.setString(ss.str());
            companyLabel.setFillColor(selectedColor);
        } else {
            companyLabel.setString("Select Preferred Companies...");
            companyLabel.setFillColor(sf::Color(150, 150, 150));
        }
        window.draw(companyLabel);
        
        bool canShowRoutes = (selectedOriginIndex != -1 && selectedDestIndex != -1);
        showRoutesBtn.setFillColor((canShowRoutes && isHovering(showRoutesBtn, mouseGlobal)) ? 
                                   btnHover : (canShowRoutes ? btnNormal : sf::Color(30, 30, 30)));
        window.draw(showRoutesBtn);
        window.draw(showRoutesTxt);
        
        // Draw tags and calculate positions
        float tagsEndY = drawTags(window, graph, font, panelX);
        
        // Adjust results area position based on tags
        float resultsY = std::min(winH - 180, tagsEndY + 100);
        resultsArea.setPosition(panelX + 20, resultsY);
        resultHeader.setPosition(panelX + 35, resultsY + 15);
        resultBody.setPosition(panelX + 35, resultsY + 50);
        
        // Adjust navigation buttons position
        float navY = resultsY - 50;
        prevBtn.setPosition(panelX + 30, navY);
        nextBtn.setPosition(panelX + 320, navY);
        prevBtnTxt.setPosition(panelX + 45, navY + 5);
        nextBtnTxt.setPosition(panelX + 335, navY + 5);
        routeCounterTxt.setPosition(panelX + 100, navY + 5);
        
        if (!filteredRoutes.empty()) {
            prevBtn.setFillColor((currentRouteIndex > 0 && isHovering(prevBtn, mouseGlobal)) ? 
                                btnHover : (currentRouteIndex > 0 ? btnNormal : sf::Color(30, 30, 30)));
            nextBtn.setFillColor((currentRouteIndex < (int)filteredRoutes.size() - 1 && 
                                 isHovering(nextBtn, mouseGlobal)) ? 
                                btnHover : (currentRouteIndex < (int)filteredRoutes.size() - 1 ? 
                                btnNormal : sf::Color(30, 30, 30)));
            
            window.draw(prevBtn);
            window.draw(prevBtnTxt);
            window.draw(nextBtn);
            window.draw(nextBtnTxt);
            
            std::stringstream ss;
            ss << (currentRouteIndex + 1) << " / " << filteredRoutes.size();
            routeCounterTxt.setString(ss.str());
            window.draw(routeCounterTxt);
        }
        
        if (selectingOrigin || selectingDest) {
            drawOriginDestDropdown(window, graph, font, mouseGlobal, panelX);
        } else if (selectingPorts) {
            drawPortDropdown(window, graph, font, mouseGlobal, panelX);
        } else if (selectingCompanies) {
            drawCompanyDropdown(window, font, mouseGlobal, panelX);
        } else {
            window.draw(resultsArea);
            window.draw(resultHeader);
            
            std::stringstream ss;
            if (!filteredRoutes.empty() && currentRouteIndex >= 0) {
                PathFinding::PathResult* result = filteredRoutes[currentRouteIndex];
                ss << "Route " << (currentRouteIndex + 1) << " of " << filteredRoutes.size() << "\n";
                ss << "Cost: $" << result->totalCost << "\n";
                ss << "Stops: " << result->path.getSize() - 2 << "\n";
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
            } else if (filteredRoutes.empty() && selectedOriginIndex != -1 && selectedDestIndex != -1) {
                ss << "No routes found matching\nyour preferences.";
            } else {
                ss << "Select origin, destination,\nand preferences";
            }
            resultBody.setString(ss.str());
            resultBody.setCharacterSize(14);
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

    void drawPortDropdown(sf::RenderWindow& window,
                         const Graph& graph,
                         const sf::Font& font,
                         const sf::Vector2f& mouseGlobal,
                         float panelX) {
        sf::RectangleShape dropBg(sf::Vector2f(340, 200));
        float yPosition = portField.getPosition().y + 45;
        dropBg.setPosition(panelX + 30, yPosition);
        dropBg.setFillColor(sf::Color(40, 50, 60));
        dropBg.setOutlineColor(sf::Color::Cyan);
        dropBg.setOutlineThickness(1);
        window.draw(dropBg);
        
        int visibleIndex = 0;
        int skip = 0;
        for (int i = 0; i < graph.size; i++) {
            if (i == selectedOriginIndex || i == selectedDestIndex) continue;
            
            if (skip < portListOffset) { skip++; continue; }
            if (visibleIndex >= 6) break;
            
            // Linear search for selected port
            bool isSelected = false;
            for (int j = 0; j < selectedPorts.size(); j++) {
                if (selectedPorts[j] == i) {
                    isSelected = true;
                    break;
                }
            }
            
            sf::Text itemTxt(graph.vertices[i].port.name, font, 14);
            itemTxt.setPosition(panelX + 40, yPosition + visibleIndex * 32 + 5);
            if (isSelected) {
                itemTxt.setFillColor(selectedColor);
                itemTxt.setStyle(sf::Text::Bold);
            } else {
                itemTxt.setFillColor(sf::Color::White);
            }
            
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

    void drawCompanyDropdown(sf::RenderWindow& window,
                            const sf::Font& font,
                            const sf::Vector2f& mouseGlobal,
                            float panelX) {
        sf::RectangleShape dropBg(sf::Vector2f(340, 200));
        float yPosition = companyField.getPosition().y + 45;
        dropBg.setPosition(panelX + 30, yPosition);
        dropBg.setFillColor(sf::Color(40, 50, 60));
        dropBg.setOutlineColor(sf::Color::Cyan);
        dropBg.setOutlineThickness(1);
        window.draw(dropBg);
        
        int visibleIndex = 0;
        int skip = 0;
        for (int i = 0; i < allCompanies.size(); i++) {
            if (skip < companyListOffset) { skip++; continue; }
            if (visibleIndex >= 6) break;
            
            // Linear search for selected company
            bool isSelected = false;
            for (int j = 0; j < selectedCompanies.size(); j++) {
                if (selectedCompanies[j] == allCompanies[i]) {
                    isSelected = true;
                    break;
                }
            }
            
            sf::Text itemTxt(allCompanies[i], font, 14);
            itemTxt.setPosition(panelX + 40, yPosition + visibleIndex * 32 + 5);
            if (isSelected) {
                itemTxt.setFillColor(selectedColor);
                itemTxt.setStyle(sf::Text::Bold);
            } else {
                itemTxt.setFillColor(sf::Color::White);
            }
            
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

    float drawTags(sf::RenderWindow& window,
                 const Graph& graph,
                 const sf::Font& font,
                 float panelX) {
        portTags.clear();
        portTagTexts.clear();
        
        float tagStartY = showRoutesBtn.getPosition().y + showRoutesBtn.getSize().y + 10;
        float tagX = panelX + 30;
        float tagY = tagStartY;
        float tagSpacing = 5.f;
        float maxWidth = 340.f;
        
        for (int portIdx : selectedPorts) {
            if (portIdx < 0 || portIdx >= graph.size) continue;
            
            std::string portName = graph.vertices[portIdx].port.name;
            sf::Text tagText(portName, font, 12);
            tagText.setFillColor(sf::Color::White);
            
            sf::FloatRect textBounds = tagText.getLocalBounds();
            float tagWidth = textBounds.width + 20;
            float tagHeight = 25;
            
            if (tagX + tagWidth > panelX + 30 + maxWidth) {
                tagX = panelX + 30;
                tagY += tagHeight + tagSpacing;
            }
            
            sf::RectangleShape tag(sf::Vector2f(tagWidth, tagHeight));
            tag.setPosition(tagX, tagY);
            tag.setFillColor(tagColor);
            tag.setOutlineColor(selectedColor);
            tag.setOutlineThickness(1.f);
            
            tagText.setPosition(tagX + 10, tagY + 5);
            
            portTags.push_back(tag);
            portTagTexts.push_back(tagText);
            
            tagX += tagWidth + tagSpacing;
        }
        
        companyTags.clear();
        companyTagTexts.clear();
        
        if (selectedPorts.size() > 0) {
            // Updated spacing logic to match simulate
            float maxPortY = tagStartY;
            for(int i = 0; i < portTags.size(); i++) {
                float y = portTags[i].getPosition().y + portTags[i].getSize().y;
                if (y > maxPortY) maxPortY = y;
            }
            tagY = maxPortY + 10;
        }
        
        tagX = panelX + 30;
        
        for (int i = 0; i < selectedCompanies.size(); i++) {
            const std::string& company = selectedCompanies[i];
            sf::Text tagText(company, font, 12);
            tagText.setFillColor(sf::Color::White);
            
            sf::FloatRect textBounds = tagText.getLocalBounds();
            float tagWidth = textBounds.width + 20;
            float tagHeight = 25;
            
            if (tagX + tagWidth > panelX + 30 + maxWidth) {
                tagX = panelX + 30;
                tagY += tagHeight + tagSpacing;
            }
            
            sf::RectangleShape tag(sf::Vector2f(tagWidth, tagHeight));
            tag.setPosition(tagX, tagY);
            tag.setFillColor(tagColor);
            tag.setOutlineColor(selectedColor);
            tag.setOutlineThickness(1.f);
            
            tagText.setPosition(tagX + 10, tagY + 5);
            
            companyTags.push_back(tag);
            companyTagTexts.push_back(tagText);
            
            tagX += tagWidth + tagSpacing;
        }
        
        for (int i = 0; i < portTags.size(); i++) {
            window.draw(portTags[i]);
            window.draw(portTagTexts[i]);
        }
        
        for (int i = 0; i < companyTags.size(); i++) {
            window.draw(companyTags[i]);
            window.draw(companyTagTexts[i]);
        }
        
        float maxY = tagStartY;
        for (int i = 0; i < portTags.size(); i++) {
            float y = portTags[i].getPosition().y + portTags[i].getSize().y;
            if (y > maxY) maxY = y;
        }
        for (int i = 0; i < companyTags.size(); i++) {
            float y = companyTags[i].getPosition().y + companyTags[i].getSize().y;
            if (y > maxY) maxY = y;
        }
        return maxY;
    }
};

#endif