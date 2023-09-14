#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <string>

using namespace sf;

bool frameCollisionX(double x, int radius) {
    return (x + radius > 1920 || x - radius < 0); 
}

bool frameCollisionY(double y, int radius) {
    return (y + radius > 1080 || y - radius < 0);
}

double distance(Vector2f vec1, Vector2f vec2) {
    double dx = vec1.x - vec2.x;
    double dy = vec1.y - vec2.y;
    return std::sqrt(dx * dx + dy * dy);
}

int main() {
    std::setlocale(LC_ALL, "rus");

    RenderWindow window(VideoMode(1920, 1080), "Gravity");
    Clock clock;
    Clock clock2;
    Clock clock3;    
    std::vector<CircleShape> circleVec;
    std::vector<Vector2f> velocities;
    std::vector<Vector2f> positions;
    std::vector<double> masses;
    std::vector<int> radiuses;
    std::vector<std::vector<Vector2f>> circleTrack;
    double G = 6.67;
    long double low = 100;
    double rSun = 20;
    double F = 0;
    double a = 0;
    sf::Vertex track;
    track.color = Color::Red;
    CircleShape sun(rSun);
    Vector2f offset(rSun, rSun);
    long double mSun = 200000;
    sun.setFillColor(Color::Yellow);
    sun.setPosition(window.getSize().x / 2, window.getSize().y / 2);
    

    sf::Time timePerFrame = sf::seconds(1.0f / 100.0f); // lps
    sf::Time timePerFrame2 = sf::seconds(1.0f / 100.0f); // fps

    sf::Time accumulatedTime = sf::Time::Zero;
    sf::Time accumulatedTime2 = sf::Time::Zero;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    double radius = std::rand() / 32767.0f * 10 + 5 ;

                    
                    double x = std::rand() / 32767.0f * 4 - 2;
                    double y = std::rand() / 32767.0f * 4 - 2;
                    
                    double mass = 100;

                    CircleShape circle(radius);
                    circle.setFillColor(Color::Color(rand() % 255, rand() % 255, rand() % 255, 255));
                    Vector2f velocity(x, y);
                    Vector2f position(mousePos.x - radius, mousePos.y - radius);
                    std::vector<Vector2f> circTrack;
                    circleVec.push_back(circle);
                    velocities.push_back(velocity);
                    positions.push_back(position);
                    radiuses.push_back(radius);
                    masses.push_back(mass);
                    circleTrack.push_back(circTrack);
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::Num1)) {
                for (int i = 0; i < circleVec.size(); i++) {
                    circleTrack[i].clear();
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::Num2)) {
                for (int i = 0; i < circleVec.size(); i++) {
                    circleVec.clear();
                    circleTrack.clear();
                    masses.clear();
                    positions.clear();
                    velocities.clear();
                    radiuses.clear();
                }
            }
        }
        
        // Обновление с учетом фиксированного шага времени
        accumulatedTime += clock.restart();
        while (accumulatedTime >= timePerFrame) {
            for (int i = 0; i < circleVec.size(); i++) {
                Vector2f pos(positions[i].x + radiuses[i], positions[i].y + radiuses[i]);
                circleTrack[i].push_back(pos);
                if (circleTrack[i].size() > 1) {
                    //circleTrack[i].erase(circleTrack[i].begin());
                }
                double distan = distance(sun.getPosition() + offset, positions[i] + Vector2f(radiuses[i], radiuses[i]));
                Vector2f normalizeDirection;
                F = G * (mSun * masses[i]) / pow(distan, 2) / low;
                a = F / masses[i];
                normalizeDirection = ((positions[i] + Vector2f(radiuses[i], radiuses[i])) - (sun.getPosition() + offset)) / (float)distan;
                velocities[i] -= normalizeDirection * static_cast<float>(a);
                positions[i] += velocities[i];
                /*
                if (frameCollisionX(positions[i].x + radiuses[i], radiuses[i]) || frameCollisionY(positions[i].y + radiuses[i], radiuses[i]) || distan <= radiuses[i] + rSun) {
                    circleVec.erase(circleVec.begin() + i);
                    positions.erase(positions.begin() + i);
                    radiuses.erase(radiuses.begin() + i);
                    masses.erase(masses.begin() + i);
                    velocities.erase(velocities.begin() + i);
                    circleTrack.erase(circleTrack.begin() + i);
                }
                */
                if (distan <= radiuses[i] + rSun) {
                    circleVec.erase(circleVec.begin() + i);
                    positions.erase(positions.begin() + i);
                    radiuses.erase(radiuses.begin() + i);
                    masses.erase(masses.begin() + i);
                    velocities.erase(velocities.begin() + i);
                    circleTrack.erase(circleTrack.begin() + i);
                }
            }
            

            
            accumulatedTime -= timePerFrame;
        }
        
        // Отрисовка с учетом интерполяции
        accumulatedTime2 += clock2.restart();
        if (accumulatedTime2 >= timePerFrame2) {
            window.clear(Color::Black);
            for (int i = 0; i < circleVec.size(); i++) {
                for (int j = 0; j < circleTrack[i].size(); j++) {
                    track.color = circleVec[i].getFillColor();
                    track.position = circleTrack[i][j];
                    window.draw(&track, 1, Points);
                }
                circleVec[i].setPosition(positions[i]);
                window.draw(circleVec[i]);
            }
            window.draw(sun);
            
            window.display();
            accumulatedTime2 -= timePerFrame2;
        }
    }

    return 0;
}
