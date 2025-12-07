#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include <SFML/Graphics.hpp>
#include "uiHelpers.hpp"

struct MainMenu {
    sf::RectangleShape menuRect1;
    sf::RectangleShape menuRect2;
    sf::RectangleShape menuRect3;
    sf::RectangleShape menuRect4;
    sf::RectangleShape menuRect5;
    sf::Text menuHeading;
    sf::Text menu1;
    sf::Text menu2;
    sf::Text menu3;
    sf::Text menu4;
    sf::Text menu5;
    sf::Color btnNormal;
    sf::Color btnHover;

    MainMenu(const sf::Font& font) 
        : menuRect1(sf::Vector2f(360.f, 50.f)), 
          menuRect2(sf::Vector2f(360.f, 50.f)), 
          menuRect3(sf::Vector2f(360.f, 50.f)),
          menuRect4(sf::Vector2f(360.f, 50.f)),
          menuRect5(sf::Vector2f(360.f, 50.f)),
          btnNormal(50, 60, 75),
          btnHover(70, 80, 100) {
        
        menuRect1.setFillColor(btnNormal);
        menuRect2.setFillColor(btnNormal);
        menuRect3.setFillColor(btnNormal);
        menuRect4.setFillColor(btnNormal);
        menuRect5.setFillColor(btnNormal);

        menuHeading.setFont(font);
        menuHeading.setString("Ocean Navigation System");
        menuHeading.setCharacterSize(24);
        menuHeading.setFillColor(sf::Color::White);

        menu1.setFont(font);
        menu1.setString("1. Find Optimized Routes");
        menu1.setCharacterSize(16);
        menu1.setFillColor(sf::Color::White);

        menu2.setFont(font);
        menu2.setString("2. Preferences & Filters");
        menu2.setCharacterSize(16);
        menu2.setFillColor(sf::Color::White);

        menu3.setFont(font);
        menu3.setString("3. Booking & Connections");
        menu3.setCharacterSize(16);
        menu3.setFillColor(sf::Color::White);

        menu4.setFont(font);
        menu4.setString("4. Boat Simulation");
        menu4.setCharacterSize(16);
        menu4.setFillColor(sf::Color::White);

        menu5.setFont(font);
        menu5.setString("5. Multi-leg Journey");
        menu5.setCharacterSize(16);
        menu5.setFillColor(sf::Color::White);
    }

    void updatePositions(float panelX) {
        menuRect1.setPosition(panelX + 20, 120);
        menuRect2.setPosition(panelX + 20, 120 + 65);
        menuRect3.setPosition(panelX + 20, 120 + 130);
        menuRect4.setPosition(panelX + 20, 120 + 195);
        menuRect5.setPosition(panelX + 20, 120 + 260);
        menuHeading.setPosition(panelX + 15, 50);
        menu1.setPosition(panelX + 40, 135);
        menu2.setPosition(panelX + 40, 200);
        menu3.setPosition(panelX + 40, 265);
        menu4.setPosition(panelX + 40, 330);
        menu5.setPosition(panelX + 40, 395);
    }

    int handleClick(const sf::Vector2f& mouseGlobal) {
        if (menuRect1.getGlobalBounds().contains(mouseGlobal)) return 1;
        if (menuRect2.getGlobalBounds().contains(mouseGlobal)) return 2;
        if (menuRect3.getGlobalBounds().contains(mouseGlobal)) return 3;
        if (menuRect4.getGlobalBounds().contains(mouseGlobal)) return 4;
        if (menuRect5.getGlobalBounds().contains(mouseGlobal)) return 5;
        return 0;
    }

    void draw(sf::RenderWindow& window, const sf::Vector2f& mouseGlobal) {
        menuRect1.setFillColor(isHovering(menuRect1, mouseGlobal) ? btnHover : btnNormal);
        menuRect2.setFillColor(isHovering(menuRect2, mouseGlobal) ? btnHover : btnNormal);
        menuRect3.setFillColor(isHovering(menuRect3, mouseGlobal) ? btnHover : btnNormal);
        menuRect4.setFillColor(isHovering(menuRect4, mouseGlobal) ? btnHover : btnNormal);
        menuRect5.setFillColor(isHovering(menuRect5, mouseGlobal) ? btnHover : btnNormal);
        
        window.draw(menuRect1);
        window.draw(menu1);
        window.draw(menuRect2);
        window.draw(menu2);
        window.draw(menuRect3);
        window.draw(menu3);
        window.draw(menuRect4);
        window.draw(menu4);
        window.draw(menuRect5);
        window.draw(menu5);
    }
};

#endif


