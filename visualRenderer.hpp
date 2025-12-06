#ifndef VISUALRENDERER_HPP
#define VISUALRENDERER_HPP

#include <SFML/Graphics.hpp>
#include "Graph.hpp"
#include "pathFinding.h"
#include "uiHelpers.hpp"
#include "vector.h"

class VisualRenderer
{
public:
    static void drawMap(sf::RenderWindow &window, const sf::Sprite &mapSprite)
    {
        window.clear(sf::Color(10, 10, 10));
        window.draw(mapSprite);
    }

    static void drawPath(sf::RenderWindow &window,
                         PathFinding::PathResult *currentPathResult,
                         const Vector<sf::Vector2f> &positions)
    {
        if (currentPathResult && currentPathResult->found)
        {
            for (int i = 0; i < currentPathResult->path.getSize() - 1; i++)
            {
                int u = currentPathResult->path.get(i);
                int v = currentPathResult->path.get(i + 1);

                if (u >= 0 && u < (int)positions.size() && v >= 0 && v < (int)positions.size())
                {
                    drawThickLine(window, positions[u], positions[v], 6.f, sf::Color(0, 0, 0, 150));
                    drawThickLine(window, positions[u], positions[v], 3.f, sf::Color(255, 0, 255));
                }
            }
        }
    }

    static void drawPorts(sf::RenderWindow &window,
                          const Graph &graph,
                          Vector<sf::Sprite> &portSprites,
                          Vector<sf::Text> &labels,
                          const Vector<sf::Vector2f> &positions,
                          PathFinding::PathResult *currentPathResult,
                          const sf::Vector2f &mouseGlobal,
                          float baseScale,
                          bool panelOpen,
                          float panelWidth,
                          const Vector<int> &preferredPorts = Vector<int>())
    {
        for (int i = 0; i < graph.size; i++)
        {
            bool underPanel = panelOpen && positions[i].x < panelWidth;
            if (!underPanel)
            {
                bool inPath = false;
                bool isEndNode = false;
                if (currentPathResult && currentPathResult->found)
                {
                    for (int k = 0; k < currentPathResult->path.getSize(); k++)
                        if (currentPathResult->path.get(k) == i)
                        {
                            if (k == 0 || k == currentPathResult->path.getSize() - 1)
                                isEndNode = true;
                            else
                                inPath = true;
                        }
                }

                float currentScale = baseScale;
                sf::Color tintColor = sf::Color::White;
                
                // Check if port is a preferred port
                bool isPreferredPort = false;
                for (int prefPort : preferredPorts) {
                    if (prefPort == i) {
                        isPreferredPort = true;
                        break;
                    }
                }

                if (inPath)
                {
                    tintColor = sf::Color(255, 100, 100);
                    currentScale = baseScale * 1.3f;
                }
                else if (isEndNode)
                {
                    tintColor = sf::Color::Green;
                    currentScale = baseScale * 1.3f;
                }
                else if (isPreferredPort)
                {
                    // Highlight preferred ports with orange/cyan color
                    tintColor = sf::Color(255, 165, 0); // Orange
                    currentScale = baseScale * 1.2f;
                }
                else
                {
                    tintColor = sf::Color(200, 255, 255);
                }

                if (isHovering(portSprites[i], mouseGlobal))
                {
                    currentScale *= 1.4f;
                    labels[i].setFillColor(sf::Color::Yellow);
                    labels[i].setStyle(sf::Text::Bold);
                    if (!inPath && !isEndNode) {
                        tintColor = sf::Color(255, 255, 0); // Yellow on hover
                    }
                }
                else
                {
                    if (isPreferredPort) {
                        labels[i].setFillColor(sf::Color(255, 200, 100)); // Light orange for preferred ports
                    } else {
                        labels[i].setFillColor(sf::Color::White);
                    }
                    labels[i].setStyle(sf::Text::Regular);
                }

                portSprites[i].setScale(currentScale, currentScale);
                portSprites[i].setColor(tintColor);

                window.draw(portSprites[i]);
            }
        }

        for (int i = 0; i < graph.size; i++)
        {
            bool underPanel = panelOpen && positions[i].x < panelWidth;
            if (!underPanel)
            {
                window.draw(labels[i]);
            }
        }
    }
};

#endif
