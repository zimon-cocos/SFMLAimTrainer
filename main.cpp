#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "headers/Random.h"

constexpr unsigned int width {600};
constexpr unsigned int height {600};


struct Target
{
    sf::CircleShape shape;

    Target(float x, float y)
    {
        shape.setPosition({x,y});
        shape.setRadius(5.0f);
        shape.setOrigin(shape.getGeometricCenter());
        shape.setFillColor(sf::Color::Green);
    }
};





int main()
{
    std::vector<Target> targets;
    targets.emplace_back(Random::get(0,600),Random::get(0,600));

    sf::RenderWindow window (sf::VideoMode({width,height}),"Aim Trainer");
    window.setFramerateLimit(60);

    sf::Clock clock;

    while(window.isOpen()){


        sf::Time timeElapsed = clock.getElapsedTime();



        while(const std::optional event = window.pollEvent()){

        if(event->is<sf::Event::Closed>()){

            window.close();

            }
        else if(const auto*keyPressed = event->getIf<sf::Event::KeyPressed>()){
            if(keyPressed->scancode == sf::Keyboard::Scancode::Escape){
                window.close();
            }
            }

        }



        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
            float xPos  = sf::Mouse::getPosition(window).x;
            float yPos = sf::Mouse::getPosition(window).y;
            int displayX = xPos;
            int displayY = yPos;

        /*if(timeElapsed.asSeconds() > 0.2)
        {
            std::cout << displayX << ", " << displayY << '\n';
            std::cout << Random::get(0,100) << '\n';
            clock.restart();
        }*/




        }

        if(timeElapsed.asSeconds()>0.5)
        {
            targets.emplace_back(Random::get(0,600),Random::get(0,600));
            clock.restart();
        }


        //Render
        window.clear(sf::Color::White);

        //DRAWING
        //window->draw(sprite);
        for(unsigned int i {0};i<targets.size();++i)
        {
            window.draw(targets[i].shape);
        }


        window.display();
    }

    //delete window;

}
