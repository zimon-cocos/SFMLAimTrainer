#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "headers/Random.h"
#include <string>
#include <string_view>

constexpr unsigned int width {600};
constexpr unsigned int height {600};
int score {0};
int previousScore {0};
int missed {0};

struct Target
{
    sf::CircleShape shape;

    Target(float x, float y)
    {
        shape.setPosition({x,y});
        shape.setRadius(30.0f);
        shape.setOrigin(shape.getGeometricCenter());
        shape.setFillColor(sf::Color::Green);
    }

    int framesExisted {0};
    bool wasClicked {false};

};





int main()
{
    std::vector<Target> targets;
    targets.emplace_back(Random::get(0,600),Random::get(0,500));

    sf::RenderWindow window (sf::VideoMode({width,height}),"Aim Trainer");
    window.setFramerateLimit(60);

    sf::Clock clock;

    sf::Font font("fonts/Jersey_15/Jersey15-Regular.ttf");
    sf::Text text(font);
    text.setString("It works");
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Red);
    text.setPosition({width/2 - 80,30});



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

            auto mousePos = sf::Mouse::getPosition(window);
            auto transMousePos = window.mapPixelToCoords(mousePos);

            for(unsigned int i {0}; i<targets.size();++i)
                {
                    previousScore = score;
                    if(targets[i].shape.getGlobalBounds().contains(transMousePos) && !(targets[i].wasClicked))
                    {
                        targets[i].shape.setFillColor(sf::Color::Red);
                        targets[i].wasClicked = true;
                        std::cout << targets[i].framesExisted << '\n';
                        ++score;

                    }
                }



        /*if(timeElapsed.asSeconds() > 0.2)
        {
            std::cout << displayX << ", " << displayY << '\n';
            std::cout << Random::get(0,100) << '\n';
            clock.restart();
        }*/




        }
        text.setString("Score: " + std::to_string(score) + " Missed: " + std::to_string(missed));
        if(timeElapsed.asSeconds()>0.2)
        {
            targets.emplace_back(Random::get(0,600),Random::get(0,500));
            clock.restart();
        }

        for(unsigned int i {0};i<targets.size();++i)
            {
                if((targets[i].framesExisted >= 300) && !(targets[i].wasClicked))
                {
                    targets[i].wasClicked = true;
                    ++missed;
                }
            }


        //Render
        window.clear(sf::Color::White);

        //DRAWING
        //window->draw(sprite);
        for(unsigned int i {0};i<targets.size();++i)
        {
            if(!(targets[i].wasClicked))
            {
                window.draw(targets[i].shape);
                ++targets[i].framesExisted;
            }

        }

        window.draw(text);
        window.display();
    }

    //delete window;

}
