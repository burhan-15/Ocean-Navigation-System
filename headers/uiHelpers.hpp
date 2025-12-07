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

// Helper function to draw an arrow from point1 to point2
inline void drawArrow(sf::RenderWindow& window, sf::Vector2f point1, sf::Vector2f point2, 
                     float thickness, sf::Color color, float arrowheadSize = 15.0f) {
    sf::Vector2f direction = point2 - point1;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (length < 0.1f) return;
    
    float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159f;
    
    // Normalize direction
    direction.x /= length;
    direction.y /= length;
    
    // Shorten endpoint by 10 pixels before the port plus arrowhead size
    float offsetFromPort = 5.0f;
    float totalOffset = arrowheadSize + offsetFromPort;
    sf::Vector2f shortenedEnd = point2 - sf::Vector2f(direction.x * totalOffset, direction.y * totalOffset);
    float lineLength = length - totalOffset;
    
    // Draw the line to the shortened endpoint
    if (lineLength > 0.0f) {
        sf::RectangleShape line(sf::Vector2f(lineLength, thickness));
        line.setPosition(point1);
        line.setRotation(angle);
        line.setFillColor(color);
        window.draw(line);
    }
    
    // Draw arrowhead triangle at the shortened endpoint
    sf::Vector2f arrowTip = shortenedEnd;
    sf::Vector2f perp(-direction.y, direction.x); // Perpendicular vector
    
    // Arrowhead points
    sf::Vector2f arrowBase = shortenedEnd - sf::Vector2f(direction.x * arrowheadSize, direction.y * arrowheadSize);
    sf::Vector2f arrowLeft = arrowBase + sf::Vector2f(perp.x * arrowheadSize * 0.6f, perp.y * arrowheadSize * 0.6f);
    sf::Vector2f arrowRight = arrowBase - sf::Vector2f(perp.x * arrowheadSize * 0.6f, perp.y * arrowheadSize * 0.6f);
    
    // Create triangle
    sf::ConvexShape arrowhead(3);
    arrowhead.setPoint(0, arrowTip);
    arrowhead.setPoint(1, arrowLeft);
    arrowhead.setPoint(2, arrowRight);
    arrowhead.setFillColor(color);
    window.draw(arrowhead);
}

// Helper function to draw a dotted arrow
inline void drawDottedArrow(sf::RenderWindow& window, sf::Vector2f point1, sf::Vector2f point2, 
                           float thickness, sf::Color color, float dashLength = 10.0f, 
                           float gapLength = 5.0f, float arrowheadSize = 15.0f) {
    sf::Vector2f direction = point2 - point1;
    float totalLength = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159f;
    
    if (totalLength < 0.1f) return;
    
    // Normalize direction
    direction.x /= totalLength;
    direction.y /= totalLength;
    
    // Shorten endpoint by 10 pixels before the port plus arrowhead size
    float offsetFromPort = 10.0f;
    float totalOffset = arrowheadSize + offsetFromPort;
    float lineLength = totalLength - totalOffset;
    if (lineLength < 0.0f) lineLength = 0.0f;
    
    // Draw dotted line
    float currentLength = 0.0f;
    bool drawing = true;
    
    while (currentLength < lineLength) {
        float segmentLength = drawing ? dashLength : gapLength;
        if (currentLength + segmentLength > lineLength) {
            segmentLength = lineLength - currentLength;
        }
        
        if (drawing) {
            sf::Vector2f start = point1 + sf::Vector2f(direction.x * currentLength, direction.y * currentLength);
            
            sf::RectangleShape line(sf::Vector2f(segmentLength, thickness));
            line.setPosition(start);
            line.setRotation(angle);
            line.setFillColor(color);
            window.draw(line);
        }
        
        currentLength += segmentLength;
        drawing = !drawing;
    }
    
    // Calculate shortened endpoint
    sf::Vector2f shortenedEnd = point2 - sf::Vector2f(direction.x * totalOffset, direction.y * totalOffset);
    
    // Draw arrowhead at the shortened endpoint
    sf::Vector2f arrowTip = shortenedEnd;
    sf::Vector2f perp(-direction.y, direction.x); // Perpendicular vector
    
    // Arrowhead points
    sf::Vector2f arrowBase = shortenedEnd - sf::Vector2f(direction.x * arrowheadSize, direction.y * arrowheadSize);
    sf::Vector2f arrowLeft = arrowBase + sf::Vector2f(perp.x * arrowheadSize * 0.6f, perp.y * arrowheadSize * 0.6f);
    sf::Vector2f arrowRight = arrowBase - sf::Vector2f(perp.x * arrowheadSize * 0.6f, perp.y * arrowheadSize * 0.6f);
    
    // Create triangle
    sf::ConvexShape arrowhead(3);
    arrowhead.setPoint(0, arrowTip);
    arrowhead.setPoint(1, arrowLeft);
    arrowhead.setPoint(2, arrowRight);
    arrowhead.setFillColor(color);
    window.draw(arrowhead);
}

#endif


