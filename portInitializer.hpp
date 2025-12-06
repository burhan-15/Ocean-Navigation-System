#ifndef PORTINITIALIZER_HPP
#define PORTINITIALIZER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Graph.hpp"

class PortInitializer {
public:
    static void initializePorts(const Graph& graph,
                               const sf::Texture& portTexture,
                               const sf::Font& font,
                               const std::vector<sf::Vector2f>& positions,
                               std::vector<sf::Sprite>& portSprites,
                               std::vector<sf::Text>& labels,
                               float baseScale) {
        portSprites.resize(graph.size);
        labels.resize(graph.size);

        for (int i = 0; i < graph.size; i++) {
            // Setup Sprite
            portSprites[i].setTexture(portTexture);
            
            // Center the sprite origin
            sf::Vector2u pSize = portTexture.getSize();
            portSprites[i].setOrigin(pSize.x / 2.0f, pSize.y / 2.0f);
            
            if (i < (int)positions.size()) {
                portSprites[i].setPosition(positions[i]);
            }
            
            portSprites[i].setScale(baseScale, baseScale);
            portSprites[i].setColor(sf::Color(255, 255, 255));

            // Setup Labels
            labels[i].setFont(font);
            labels[i].setString(graph.vertices[i].port.name);
            labels[i].setCharacterSize(12);
            labels[i].setFillColor(sf::Color::White);
            labels[i].setOutlineColor(sf::Color::Black);
            labels[i].setOutlineThickness(2.f); 

            auto b = labels[i].getLocalBounds();
            labels[i].setOrigin(b.width / 2, b.height + 8);
            if (i < (int)positions.size()) {
                labels[i].setPosition(positions[i].x, positions[i].y - (pSize.y * baseScale)/2 - 5);
            }
        }
    }

    static std::vector<sf::Vector2f> getDefaultPositions() {
        return {
            {1150.f, 330.f}, {1050.f, 300.f}, {730.f, 170.f}, {790.f, 200.f}, {925.f, 350.f},
            {1120.f, 420.f}, {1400.f, 630.f}, {1350.f, 650.f}, {490.f, 200.f}, {970.f, 310.f},
            {1275.f, 430.f}, {880.f, 280.f}, {920.f, 220.f}, {1200.f, 450.f}, {1300.f, 250.f},
            {1350.f, 50.f}, {420.f, 130.f}, {850.f, 230.f}, {820.f, 620.f}, {810.f, 130.f},
            {1000.f, 360.f}, {880.f, 620.f}, {790.f, 250.f}, {860.f, 160.f}, {1250.f, 150.f},
            {1275.f, 350.f}, {1250.f, 500.f}, {730.f, 250.f}, {450.f, 350.f}, {250.f, 300.f},
            {1070.f, 350.f}, {400.f, 250.f}, {550.f, 600.f}, {950.f, 80.f}, {925.f, 500.f},
            {700.f, 350.f}, {1275.f, 300.f}, {800.f, 450.f}, {1350.f, 200.f}, {1500.f, 550.f}
        };
    }
};

#endif

