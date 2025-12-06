#ifndef UIPANEL_HPP
#define UIPANEL_HPP

#include <SFML/Graphics.hpp>

struct UIPanel {
    sf::RectangleShape panel;
    sf::RectangleShape toggleBtn;
    sf::Text toggleBtnTxt;
    sf::Text closeBtnTxt;
    sf::RectangleShape closeBtnHitbox;
    sf::Text backBtnTxt;
    sf::RectangleShape backBtnHitbox;
    
    float panelWidth;
    float panelX;
    bool panelOpen;

    UIPanel(const sf::Font& font, float winH, float panelWidth = 400.f)
        : panel(sf::Vector2f(panelWidth, winH)),
          toggleBtn(sf::Vector2f(80, 40)),
          closeBtnHitbox(sf::Vector2f(40, 40)),
          backBtnHitbox(sf::Vector2f(80, 40)),
          panelWidth(panelWidth),
          panelX(-panelWidth),
          panelOpen(false) {
        
        panel.setFillColor(sf::Color(20, 30, 45, 245));
        panel.setOutlineThickness(-2.f);
        panel.setOutlineColor(sf::Color(0, 150, 200));
        
        toggleBtn.setFillColor(sf::Color(0, 150, 200));
        toggleBtn.setPosition(10, 10);
        
        toggleBtnTxt.setFont(font);
        toggleBtnTxt.setString("MENU");
        toggleBtnTxt.setCharacterSize(16);
        toggleBtnTxt.setFillColor(sf::Color::White);
        toggleBtnTxt.setPosition(25, 20);
        
        closeBtnTxt.setFont(font);
        closeBtnTxt.setString("X");
        closeBtnTxt.setCharacterSize(22);
        closeBtnTxt.setFillColor(sf::Color(200, 50, 50));
        
        closeBtnHitbox.setFillColor(sf::Color::Transparent);
        
        backBtnTxt.setFont(font);
        backBtnTxt.setString("< BACK");
        backBtnTxt.setCharacterSize(18);
        backBtnTxt.setFillColor(sf::Color(100, 200, 255));
        
        backBtnHitbox.setFillColor(sf::Color::Transparent);
    }

    void updatePositions() {
        panel.setPosition(panelX, 0);
        closeBtnHitbox.setPosition(panelX + panelWidth - 50, 10);
        closeBtnTxt.setPosition(panelX + panelWidth - 40, 15);
        backBtnHitbox.setPosition(panelX + 10, 10);
        backBtnTxt.setPosition(panelX + 20, 15);
    }

    void updateAnimation() {
        if (panelOpen && panelX < 0) panelX += 25;
        if (!panelOpen && panelX > -panelWidth) panelX -= 25;
        if (panelX > 0) panelX = 0;
        if (panelX < -panelWidth) panelX = -panelWidth;
    }

    bool handleToggleClick(const sf::Vector2f& mouseGlobal) {
        if (!panelOpen && toggleBtn.getGlobalBounds().contains(mouseGlobal)) {
            panelOpen = true;
            return true;
        }
        return false;
    }

    bool handleCloseClick(const sf::Vector2f& mouseGlobal, int currentMenu) {
        if (currentMenu == 0 && closeBtnHitbox.getGlobalBounds().contains(mouseGlobal)) {
            panelOpen = false;
            return true;
        }
        return false;
    }

    bool handleBackClick(const sf::Vector2f& mouseGlobal, int currentMenu) {
        if (currentMenu != 0 && backBtnHitbox.getGlobalBounds().contains(mouseGlobal)) {
            return true;
        }
        return false;
    }

    void draw(sf::RenderWindow& window, int currentMenu) {
        window.draw(panel);
        
        if (!panelOpen) {
            window.draw(toggleBtn);
            window.draw(toggleBtnTxt);
        } else {
            if (currentMenu == 0) {
                window.draw(closeBtnHitbox);
                window.draw(closeBtnTxt);
            } else {
                window.draw(backBtnTxt);
            }
        }
    }
};

#endif

