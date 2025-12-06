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

#endif


