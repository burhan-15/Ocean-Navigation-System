#ifndef ROUTEFINDINGMENU_HPP
#define ROUTEFINDINGMENU_HPP

#include <SFML/Graphics.hpp>
#include <sstream>
#include "Graph.hpp"
#include "pathFinding.h"
#include "uiHelpers.hpp"

struct RouteFindingMenu {
    // Input fields
    sf::RectangleShape fieldOrigin;
    sf::RectangleShape fieldDest;
    sf::Text originLabel;
    sf::Text destLabel;
    
    // Buttons
    sf::RectangleShape subBtn1;
    sf::RectangleShape subBtn2;
    sf::Text sub1_A;
    sf::Text sub1_B;
    
    // Results panel
    sf::RectangleShape resultsArea;
    sf::Text resultHeader;
    sf::Text resultBody;
    
    // State
    bool selectingOrigin;
    bool selectingDest;
    int selectedOriginIndex;
    int selectedDestIndex;
    int listOffset;
    
    sf::Color btnNormal;
    sf::Color btnHover;

    RouteFindingMenu(const sf::Font& font)
        : fieldOrigin(sf::Vector2f(340.f, 40.f)),
          fieldDest(sf::Vector2f(340.f, 40.f)),
          subBtn1(sf::Vector2f(360.f, 50.f)),
          subBtn2(sf::Vector2f(360.f, 50.f)),
          resultsArea(sf::Vector2f(360.f, 200.f)),
          selectingOrigin(false),
          selectingDest(false),
          selectedOriginIndex(-1),
          selectedDestIndex(-1),
          listOffset(0),
          btnNormal(50, 60, 75),
          btnHover(70, 80, 100) {
        
        fieldOrigin.setFillColor(sf::Color(30, 40, 50));
        fieldOrigin.setOutlineColor(sf::Color(120,120,120));
        fieldOrigin.setOutlineThickness(1.f);
        
        fieldDest.setFillColor(sf::Color(30, 40, 50));
        fieldDest.setOutlineColor(sf::Color(120,120,120));
        fieldDest.setOutlineThickness(1.f);
        
        subBtn1.setFillColor(btnNormal);
        subBtn2.setFillColor(btnNormal);
        
        originLabel.setFont(font);
        originLabel.setString("Select Origin...");
        originLabel.setCharacterSize(15);
        originLabel.setFillColor(sf::Color(150,150,150));
        
        destLabel.setFont(font);
        destLabel.setString("Select Destination...");
        destLabel.setCharacterSize(15);
        destLabel.setFillColor(sf::Color(150,150,150));
        
        sub1_A.setFont(font);
        sub1_A.setString("Find Shortest TIME Route");
        sub1_A.setCharacterSize(16);
        sub1_A.setFillColor(sf::Color::White);
        
        sub1_B.setFont(font);
        sub1_B.setString("Find Cheapest COST Route");
        sub1_B.setCharacterSize(16);
        sub1_B.setFillColor(sf::Color::White);
        
        resultsArea.setFillColor(sf::Color(0, 0, 0, 80));
        resultsArea.setOutlineColor(sf::Color(0, 255, 200));
        resultsArea.setOutlineThickness(1.f);
        
        resultHeader.setFont(font);
        resultHeader.setString("ROUTE RESULTS");
        resultHeader.setCharacterSize(14);
        resultHeader.setFillColor(sf::Color(0, 255, 200));
        
        resultBody.setFont(font);
        resultBody.setString("Select origin and destination\nto calculate path.");
        resultBody.setCharacterSize(16);
        resultBody.setFillColor(sf::Color::White);
        resultBody.setLineSpacing(1.4f);
    }

    void updatePositions(float panelX, float winH) {
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
    }

    void handleMouseWheel(float delta, const Graph& graph) {
        // Calculate max offset based on available items
        int availableItems = 0;
        for (int i = 0; i < graph.size; i++) {
            if ((selectingOrigin && i == selectedDestIndex) || 
                (selectingDest && i == selectedOriginIndex)) continue;
            availableItems++;
        }
        
        int maxOffset = std::max(0, availableItems - 8); // 8 visible items
        
        if (delta > 0) listOffset--;
        else listOffset++;
        if (listOffset < 0) listOffset = 0;
        if (listOffset > maxOffset) listOffset = maxOffset;
    }

    bool handleClick(Graph& graph, 
                    const sf::Vector2f& mouseGlobal,
                    float panelX,
                    PathFinding::PathResult*& currentPathResult,
                    std::string& resultTextString) {
        if (selectingOrigin || selectingDest) {
            int const visibleItems = 8;
            float itemH = 30.f;
            float startY = (selectingOrigin) ? fieldOrigin.getPosition().y + 45 : fieldDest.getPosition().y + 45;

            bool clickedItem = false;
            int visibleIndex = 0;
            int skip = 0;

            for(int i = 0; i < graph.size; i++) {
                if(selectingOrigin && i == selectedDestIndex) continue;
                if(selectingDest && i == selectedOriginIndex) continue;
                
                if(skip < listOffset) { skip++; continue; }
                if(visibleIndex >= visibleItems) break;

                sf::FloatRect itemRect(panelX + 30, startY + visibleIndex*(itemH+2), 340, itemH);
                if(itemRect.contains(mouseGlobal)) {
                    if(selectingOrigin) selectedOriginIndex = i;
                    else selectedDestIndex = i;
                    
                    selectingOrigin = selectingDest = false;
                    if(currentPathResult) { 
                        delete currentPathResult; 
                        currentPathResult = nullptr; 
                    }
                    clickedItem = true;
                    break;
                }
                visibleIndex++;
            }
            if(!clickedItem) { 
                selectingOrigin = selectingDest = false; 
            }
            return true;
        }
        else if (fieldOrigin.getGlobalBounds().contains(mouseGlobal)) { 
            selectingOrigin = true; 
            selectingDest = false; 
            return true;
        }
        else if (fieldDest.getGlobalBounds().contains(mouseGlobal)) { 
            selectingDest = true; 
            selectingOrigin = false; 
            return true;
        }
        else if (selectedOriginIndex != -1 && selectedDestIndex != -1) {
            if (subBtn1.getGlobalBounds().contains(mouseGlobal)) {
                if(currentPathResult) delete currentPathResult;
                currentPathResult = PathFinding::findShortestTimePath(
                    graph, selectedOriginIndex, selectedDestIndex);
                resultTextString = "Optimization: FASTEST";
                return true;
            }
            else if (subBtn2.getGlobalBounds().contains(mouseGlobal)) {
                if(currentPathResult) delete currentPathResult;
                currentPathResult = PathFinding::findCheapestPath(
                    graph, selectedOriginIndex, selectedDestIndex);
                resultTextString = "Optimization: CHEAPEST";
                return true;
            }
        }
        return false;
    }

    void reset() {
        selectingOrigin = selectingDest = false;
        selectedOriginIndex = selectedDestIndex = -1;
        listOffset = 0;
    }

    void draw(sf::RenderWindow& window,
             const Graph& graph,
             const sf::Font& font,
             const sf::Vector2f& mouseGlobal,
             float panelX,
             PathFinding::PathResult* currentPathResult,
             const std::string& resultTextString) {
        window.draw(fieldOrigin);
        window.draw(fieldDest);
        
        if(selectedOriginIndex != -1) {
            originLabel.setString(graph.vertices[selectedOriginIndex].port.name);
            originLabel.setFillColor(sf::Color::White);
        } else { 
            originLabel.setString("Select Origin..."); 
            originLabel.setFillColor(sf::Color(150,150,150)); 
        }
        
        if(selectedDestIndex != -1) {
            destLabel.setString(graph.vertices[selectedDestIndex].port.name);
            destLabel.setFillColor(sf::Color::White);
        } else { 
            destLabel.setString("Select Destination..."); 
            destLabel.setFillColor(sf::Color(150,150,150)); 
        }

        window.draw(originLabel);
        window.draw(destLabel);

        subBtn1.setFillColor(isHovering(subBtn1, mouseGlobal) ? btnHover : btnNormal);
        subBtn2.setFillColor(isHovering(subBtn2, mouseGlobal) ? btnHover : btnNormal);
        
        window.draw(subBtn1);
        window.draw(sub1_A);
        window.draw(subBtn2);
        window.draw(sub1_B);

        if (selectingOrigin || selectingDest) {
            sf::RectangleShape dropBg(sf::Vector2f(340, 250));
            int yPosition = (selectingOrigin) ? fieldOrigin.getPosition().y + 45 : fieldDest.getPosition().y + 45;
            dropBg.setPosition(panelX + 30, yPosition); 
            dropBg.setFillColor(sf::Color(40, 50, 60));
            dropBg.setOutlineColor(sf::Color::Cyan);
            dropBg.setOutlineThickness(1);
            window.draw(dropBg);

            int visibleIndex = 0;
            int skip = 0;
            for(int i = 0; i < graph.size; i++) {
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
            
            std::stringstream ss;
            if(currentPathResult && currentPathResult->found) {
                ss << resultTextString << "\n\n";
                ss << "Total Cost: $" << currentPathResult->totalCost << "\n";
                int totalMins = currentPathResult->totalTime;
                int days = totalMins / (24 * 60);
                int hours = (totalMins % (24 * 60)) / 60;
                int mins = totalMins % 60;
                ss << "Total Time: " << days << "d " << hours << "h " << mins << "m\n";
                ss << "Stops: " << currentPathResult->path.getSize() - 2;
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
};

#endif


