#ifndef PREFERENCESMENU_HPP
#define PREFERENCESMENU_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <set>
#include <sstream>
#include <algorithm>
#include "Graph.hpp"
#include "routeFilter.hpp"
#include "uiHelpers.hpp"

struct PreferencesMenu {
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
    
    // Preferred ports (for layovers)
    sf::RectangleShape portField;
    sf::Text portLabel;
    bool selectingPorts;
    std::vector<int> selectedPorts;
    int portListOffset;
    
    // Preferred companies (for route segments)
    sf::RectangleShape companyField;
    sf::Text companyLabel;
    bool selectingCompanies;
    std::vector<std::string> selectedCompanies;
    std::vector<std::string> allCompanies;
    int companyListOffset;
    
    // Show Routes button
    sf::RectangleShape showRoutesBtn;
    sf::Text showRoutesTxt;
    
    // Navigation buttons
    sf::RectangleShape prevBtn;
    sf::RectangleShape nextBtn;
    sf::Text prevBtnTxt;
    sf::Text nextBtnTxt;
    sf::Text routeCounterTxt;
    
    // Filtered routes
    std::vector<PathFinding::PathResult*> filteredRoutes;
    int currentRouteIndex;
    
    // Results display
    sf::RectangleShape resultsArea;
    sf::Text resultHeader;
    sf::Text resultBody;
    
    // Tags for selected ports and companies
    std::vector<sf::RectangleShape> portTags;
    std::vector<sf::Text> portTagTexts;
    std::vector<sf::RectangleShape> companyTags;
    std::vector<sf::Text> companyTagTexts;
    
    sf::Color btnNormal;
    sf::Color btnHover;
    sf::Color selectedColor;
    sf::Color tagColor;

    PreferencesMenu(const Graph& graph, const sf::Font& font)
        : fieldOrigin(sf::Vector2f(340.f, 40.f)),
          fieldDest(sf::Vector2f(340.f, 40.f)),
          portField(sf::Vector2f(340.f, 40.f)),
          companyField(sf::Vector2f(340.f, 40.f)),
          showRoutesBtn(sf::Vector2f(340.f, 50.f)),
          prevBtn(sf::Vector2f(50.f, 40.f)),
          nextBtn(sf::Vector2f(50.f, 40.f)),
          resultsArea(sf::Vector2f(360.f, 250.f)),
          selectingOrigin(false),
          selectingDest(false),
          selectingPorts(false),
          selectingCompanies(false),
          selectedOriginIndex(-1),
          selectedDestIndex(-1),
          originDestListOffset(0),
          portListOffset(0),
          companyListOffset(0),
          currentRouteIndex(-1),
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
            // Calculate max offset for origin/destination
            int availableItems = 0;
            for (int i = 0; i < graph.size; i++) {
                if ((selectingOrigin && i == selectedDestIndex) || 
                    (selectingDest && i == selectedOriginIndex)) continue;
                availableItems++;
            }
            int maxOffset = std::max(0, availableItems - 6); // 6 visible items
            
            if (delta > 0) originDestListOffset--;
            else originDestListOffset++;
            if (originDestListOffset < 0) originDestListOffset = 0;
            if (originDestListOffset > maxOffset) originDestListOffset = maxOffset;
        } else if (isPortSelection) {
            // Calculate max offset for ports (excluding origin/destination)
            int availableItems = 0;
            for (int i = 0; i < graph.size; i++) {
                if (i == selectedOriginIndex || i == selectedDestIndex) continue;
                availableItems++;
            }
            int maxOffset = std::max(0, availableItems - 6); // 6 visible items
            
            if (delta > 0) portListOffset--;
            else portListOffset++;
            if (portListOffset < 0) portListOffset = 0;
            if (portListOffset > maxOffset) portListOffset = maxOffset;
        } else {
            // Calculate max offset for companies
            int maxOffset = std::max(0, (int)allCompanies.size() - 6); // 6 visible items
            
            if (delta > 0) companyListOffset--;
            else companyListOffset++;
            if (companyListOffset < 0) companyListOffset = 0;
            if (companyListOffset > maxOffset) companyListOffset = maxOffset;
        }
    }

    bool handleClick(Graph& graph,
                    const sf::Vector2f& mouseGlobal,
                    float panelX,
                    float winH,
                    const sf::Font& font,
                    PathFinding::PathResult*& currentPathResult,
                    std::string& resultTextString) {
        // Handle origin/destination selection
        if (selectingOrigin || selectingDest) {
            return handleOriginDestSelectionClick(graph, mouseGlobal, panelX);
        }
        // Handle port selection
        else if (selectingPorts) {
            return handlePortSelectionClick(graph, mouseGlobal, panelX);
        }
        // Handle company selection
        else if (selectingCompanies) {
            return handleCompanySelectionClick(mouseGlobal, panelX);
        }
        // Handle field clicks
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
        // Handle navigation buttons (check before show routes to prioritize)
        // Calculate button positions the same way as in draw()
        if (!filteredRoutes.empty()) {
            // Calculate exact Y position using the simulation helper
            float navY = calculateLayoutNavY(graph, font, panelX, winH);
            
            // Reconstruct the hitboxes exactly where draw() puts them
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
        // Check if currentPathResult is in filteredRoutes before deleting
        bool currentIsInFiltered = false;
        for (auto* route : filteredRoutes) {
            if (route == currentPathResult) {
                currentIsInFiltered = true;
                break;
            }
        }
        
        // Clean up old filtered routes
        for (auto* route : filteredRoutes) {
            // Don't delete if it's the currentPathResult (will be handled separately)
            if (route != currentPathResult) {
                delete route;
            }
        }
        filteredRoutes.clear();
        
        // Now safe to delete currentPathResult if it was in filteredRoutes
        if (currentPathResult && currentIsInFiltered) {
            delete currentPathResult;
            currentPathResult = nullptr;
        }
        
        // Find filtered routes from origin to destination
        filteredRoutes = RouteFilter::findFilteredRoutes(
            graph, selectedOriginIndex, selectedDestIndex, 
            selectedPorts, selectedCompanies);
        
        // Set current route
        currentRouteIndex = -1;
        if (!filteredRoutes.empty()) {
            currentRouteIndex = 0;
            // Only delete currentPathResult if it's not in the new filteredRoutes
            if (currentPathResult) {
                bool isInNewFiltered = false;
                for (auto* route : filteredRoutes) {
                    if (route == currentPathResult) {
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
        if (filteredRoutes.empty() || currentRouteIndex <= 0) return;
        
        currentRouteIndex--;
        // Don't delete currentPathResult if it's in filteredRoutes
        // (it will be managed by filteredRoutes cleanup)
        bool isInFiltered = false;
        for (auto* route : filteredRoutes) {
            if (route == currentPathResult) {
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
        if (filteredRoutes.empty() || 
            currentRouteIndex >= (int)filteredRoutes.size() - 1) return;
        
        currentRouteIndex++;
        // Don't delete currentPathResult if it's in filteredRoutes
        // (it will be managed by filteredRoutes cleanup)
        bool isInFiltered = false;
        for (auto* route : filteredRoutes) {
            if (route == currentPathResult) {
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
        // Note: Don't delete filteredRoutes here - they might be referenced by currentPathResult
        // Cleanup will handle deletion
    }

    void cleanup() {
        for (auto* route : filteredRoutes) {
            delete route;
        }
        filteredRoutes.clear();
    }

private:
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
            // Skip if selecting origin and this is the selected destination, or vice versa
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
            // Don't allow selecting origin or destination as layover ports
            if (i == selectedOriginIndex || i == selectedDestIndex) continue;
            
            if (skip < portListOffset) { skip++; continue; }
            if (visibleIndex >= visibleItems) break;
            
            sf::FloatRect itemRect(panelX + 30, startY + visibleIndex * (itemH + 2), 340, itemH);
            if (itemRect.contains(mouseGlobal)) {
                // Toggle selection
                auto it = std::find(selectedPorts.begin(), selectedPorts.end(), i);
                if (it != selectedPorts.end()) {
                    selectedPorts.erase(it);
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
        
        for (size_t i = 0; i < allCompanies.size(); i++) {
            if (skip < companyListOffset) { skip++; continue; }
            if (visibleIndex >= visibleItems) break;
            
            sf::FloatRect itemRect(panelX + 30, startY + visibleIndex * (itemH + 2), 340, itemH);
            if (itemRect.contains(mouseGlobal)) {
                // Toggle selection
                auto it = std::find(selectedCompanies.begin(), selectedCompanies.end(), allCompanies[i]);
                if (it != selectedCompanies.end()) {
                    selectedCompanies.erase(it);
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
        // Draw fields
        window.draw(fieldOrigin);
        window.draw(fieldDest);
        window.draw(portField);
        window.draw(companyField);
        
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
        
        // Update and draw port label
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
        
        // Update and draw company label
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
        
        // Draw show routes button
        bool canShowRoutes = (selectedOriginIndex != -1 && selectedDestIndex != -1);
        showRoutesBtn.setFillColor((canShowRoutes && isHovering(showRoutesBtn, mouseGlobal)) ? 
                                   btnHover : (canShowRoutes ? btnNormal : sf::Color(30, 30, 30)));
        window.draw(showRoutesBtn);
        window.draw(showRoutesTxt);
        
        // Calculate tag area height to adjust results area position
        float tagAreaHeight = 0;
        if (!selectedPorts.empty()) {
            int portRows = (selectedPorts.size() + 2) / 3; // Approximate rows
            tagAreaHeight += portRows * 30 + 10;
        }
        if (!selectedCompanies.empty()) {
            int companyRows = (selectedCompanies.size() + 2) / 3; // Approximate rows
            tagAreaHeight += companyRows * 30 + 10;
            if (!selectedPorts.empty()) tagAreaHeight += 10; // Extra spacing
        }
        
        // Draw tags for selected ports and companies
        float tagsEndY = drawTags(window, graph, font, panelX);
        
        // Adjust results area position based on tags (lower it)
        float resultsY = std::min(winH - 180, tagsEndY + 100);
        resultsArea.setPosition(panelX + 20, resultsY);
        resultHeader.setPosition(panelX + 35, resultsY + 15);
        resultBody.setPosition(panelX + 35, resultsY + 50);
        
        // Adjust navigation buttons position (lower them, closer to results)
        float navY = resultsY - 50;
        prevBtn.setPosition(panelX + 30, navY);
        nextBtn.setPosition(panelX + 320, navY);
        prevBtnTxt.setPosition(panelX + 45, navY + 5);
        nextBtnTxt.setPosition(panelX + 335, navY + 5);
        routeCounterTxt.setPosition(panelX + 100, navY + 5);
        
        // Draw navigation buttons if routes are available
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
            
            // Draw route counter
            std::stringstream ss;
            ss << (currentRouteIndex + 1) << " / " << filteredRoutes.size();
            routeCounterTxt.setString(ss.str());
            window.draw(routeCounterTxt);
        }
        
        // Draw dropdowns
        if (selectingOrigin || selectingDest) {
            drawOriginDestDropdown(window, graph, font, mouseGlobal, panelX);
        } else if (selectingPorts) {
            drawPortDropdown(window, graph, font, mouseGlobal, panelX);
        } else if (selectingCompanies) {
            drawCompanyDropdown(window, font, mouseGlobal, panelX);
        } else {
            // Draw results
            window.draw(resultsArea);
            window.draw(resultHeader);
            
            std::stringstream ss;
            if (!filteredRoutes.empty() && currentRouteIndex >= 0) {
                PathFinding::PathResult* result = filteredRoutes[currentRouteIndex];
                ss << "Route " << (currentRouteIndex + 1) << " of " << filteredRoutes.size() << "\n";
                ss << "Cost: $" << result->totalCost << "\n";
                ss << "Stops: " << result->path.getSize() - 2 << "\n";
                ss << "Path:\n";
                
                // Format path with line breaks to prevent overflow (more compact)
                std::string pathStr = "";
                int lineLength = 0;
                const int maxLineLength = 35; // Reduced for smaller modal
                
                for (int i = 0; i < result->path.getSize(); i++) {
                    std::string portName = graph.vertices[result->path.get(i)].port.name;
                    
                    if (i > 0) {
                        pathStr += " -> ";
                        lineLength += 4;
                    }
                    
                    // Add line break if line would be too long
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
            resultBody.setCharacterSize(14); // Smaller font to fit better
            window.draw(resultBody);
        }
    }

private:

    // Helper to calculate exactly where the navigation buttons should be
    float calculateLayoutNavY(const Graph& graph, const sf::Font& font, float panelX, float winH) {
        float startY = showRoutesBtn.getPosition().y + showRoutesBtn.getSize().y + 10;
        float tagX = panelX + 30;
        float tagY = startY;
        float maxWidth = 340.f;
        float tagHeight = 25.f;
        float tagSpacing = 5.f;
        float maxY = startY;

        // 1. Simulate Port Tags
        for (int portIdx : selectedPorts) {
            if (portIdx < 0 || portIdx >= graph.size) continue;
            std::string name = graph.vertices[portIdx].port.name;
            sf::Text tempText(name, font, 12);
            float width = tempText.getLocalBounds().width + 20;

            if (tagX + width > panelX + 30 + maxWidth) {
                tagX = panelX + 30;
                tagY += tagHeight + tagSpacing;
            }
            // Track the bottom of the current line
            maxY = std::max(maxY, tagY + tagHeight);
            tagX += width + tagSpacing;
        }

        // 2. Simulate Company Tags
        if (!selectedCompanies.empty() && !selectedPorts.empty()) {
            tagY = maxY + 10; // Add spacing between groups (was 30 in draw loop logic, effectively +10 from bottom of prev)
            // In draw(), it does: tagY += 30. But tagY was usually top of line. 
            // Let's stick closer to draw logic:
            // If we just finished a row, tagY is the TOP of that row.
            // We need to move down past that row (+30 seems to be the logic in drawTags).
            // Let's assume the safest way is to take the calculated maxY and add gap.
            tagY += 5; // Adjustment to match visual gap
            tagX = panelX + 30; 
        } else if (!selectedCompanies.empty() && selectedPorts.empty()) {
            tagX = panelX + 30;
        }

        for (const std::string& company : selectedCompanies) {
            sf::Text tempText(company, font, 12);
            float width = tempText.getLocalBounds().width + 20;

            if (tagX + width > panelX + 30 + maxWidth) {
                tagX = panelX + 30;
                tagY += tagHeight + tagSpacing;
            }
            maxY = std::max(maxY, tagY + tagHeight);
            tagX += width + tagSpacing;
        }

        // 3. Calculate Final Button Y (Logic copied from draw())
        // In draw(): resultsY = std::min(winH - 180, tagsEndY + 100);
        //            navY = resultsY - 50;
        
        // Note: tagsEndY in draw() is actually maxY here.
        float resultsY = std::min(winH - 180, maxY + 100);
        return resultsY - 50;
    }    

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
            
            bool isSelected = std::find(selectedPorts.begin(), selectedPorts.end(), i) != selectedPorts.end();
            
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
        for (size_t i = 0; i < allCompanies.size(); i++) {
            if (skip < companyListOffset) { skip++; continue; }
            if (visibleIndex >= 6) break;
            
            bool isSelected = std::find(selectedCompanies.begin(), selectedCompanies.end(), 
                                       allCompanies[i]) != selectedCompanies.end();
            
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
        // Update and draw port tags
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
            
            // Check if we need to wrap to next line
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
        
        // Update company tags
        companyTags.clear();
        companyTagTexts.clear();
        
        // Start company tags below port tags
        if (!selectedPorts.empty()) {
            tagY += 30; // Space after port tags
        }
        tagX = panelX + 30;
        
        for (const std::string& company : selectedCompanies) {
            sf::Text tagText(company, font, 12);
            tagText.setFillColor(sf::Color::White);
            
            sf::FloatRect textBounds = tagText.getLocalBounds();
            float tagWidth = textBounds.width + 20;
            float tagHeight = 25;
            
            // Check if we need to wrap to next line
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
        
        // Draw all tags
        for (size_t i = 0; i < portTags.size(); i++) {
            window.draw(portTags[i]);
            window.draw(portTagTexts[i]);
        }
        
        for (size_t i = 0; i < companyTags.size(); i++) {
            window.draw(companyTags[i]);
            window.draw(companyTagTexts[i]);
        }
        
        // Return the Y position where tags end
        float maxY = tagStartY;
        for (const auto& tag : portTags) {
            maxY = std::max(maxY, tag.getPosition().y + tag.getSize().y);
        }
        for (const auto& tag : companyTags) {
            maxY = std::max(maxY, tag.getPosition().y + tag.getSize().y);
        }
        return maxY;
    }
};

#endif
