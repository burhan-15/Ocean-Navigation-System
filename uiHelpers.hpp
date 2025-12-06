#ifndef UIHELPERS_HPP
#define UIHELPERS_HPP

#include <SFML/Graphics.hpp>
#include <cmath>

// Helper function to draw thick lines
inline void drawThickLine(sf::RenderWindow& window, sf::Vector2f point1, sf::Vector2f point2, float thickness, sf::Color color) {
    sf::Vector2f direction = point2 - point1;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159f;

    sf::RectangleShape line(sf::Vector2f(length, thickness));
    line.setPosition(point1);
    line.setRotation(angle);
    line.setFillColor(color);
    window.draw(line);
}

// Check if mouse is hovering over shapes
inline bool isHovering(const sf::Shape& shape, const sf::Vector2f& mousePos) {
    return shape.getGlobalBounds().contains(mousePos);
}

inline bool isHovering(const sf::RectangleShape& rect, const sf::Vector2f& mousePos) {
    return rect.getGlobalBounds().contains(mousePos);
}

// Overload for Sprites
inline bool isHovering(const sf::Sprite& sprite, const sf::Vector2f& mousePos) {
    return sprite.getGlobalBounds().contains(mousePos);
}

// Helper function to draw dotted lines
inline void drawDottedLine(sf::RenderWindow& window, sf::Vector2f point1, sf::Vector2f point2, 
                          float thickness, sf::Color color, float dashLength = 10.0f, float gapLength = 5.0f) {
    sf::Vector2f direction = point2 - point1;
    float totalLength = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159f;
    
    if (totalLength < 0.1f) return;
    
    // Normalize direction
    direction.x /= totalLength;
    direction.y /= totalLength;
    
    float currentLength = 0.0f;
    bool drawing = true;
    
    while (currentLength < totalLength) {
        float segmentLength = drawing ? dashLength : gapLength;
        if (currentLength + segmentLength > totalLength) {
            segmentLength = totalLength - currentLength;
        }
        
        if (drawing) {
            sf::Vector2f start = point1 + sf::Vector2f(direction.x * currentLength, direction.y * currentLength);
            sf::Vector2f end = point1 + sf::Vector2f(direction.x * (currentLength + segmentLength), 
                                                     direction.y * (currentLength + segmentLength));
            
            sf::RectangleShape line(sf::Vector2f(segmentLength, thickness));
            line.setPosition(start);
            line.setRotation(angle);
            line.setFillColor(color);
            window.draw(line);
        }
        
        currentLength += segmentLength;
        drawing = !drawing;
    }
}

#endif


