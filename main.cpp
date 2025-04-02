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




struct playerInfo
{
    float xPlayer {0};
    float yPlayer {0};

};


void movePlayer(sf::Sprite& toMove,playerInfo& Info)
{
    toMove.setPosition({Info.xPlayer,Info.yPlayer});
    ++Info.xPlayer;
    ++Info.yPlayer;
}


void rotatePlayer(sf::Sprite& toRotate,int degrees)
{
    toRotate.rotate(sf::degrees(degrees));
}



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


float degToRad(int degrees)
{
    return degrees*(3.1415926535/180);
}

int main()
{
    std::vector<Target> targets;
    targets.emplace_back(Random::get(0,600),Random::get(0,500));

    sf::RenderWindow window (sf::VideoMode({width,height}),"Aim Trainer");
    window.setFramerateLimit(60);

    sf::Clock clock;

    sf::Font font("fonts/Jersey_15/Jersey15-Regular.ttf");
    sf::Text text(font);

    sf::Text angleText(font);

    text.setString("It works");
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Red);
    text.setPosition({width/2 - 80,30});

    angleText.setString("It works");
    angleText.setCharacterSize(12);
    angleText.setFillColor(sf::Color::Red);
    angleText.setPosition({300.0f,300.0f});


    playerInfo mainInfo;
    sf::Texture playerTexture("sprites/theTriangleFixed.png",false,sf::IntRect({0,0},{50,80}));
    sf::Sprite player(playerTexture);
    player.setOrigin({25.0f,40.0f});
    player.setPosition({300.0f,300.0f});


    sf::CircleShape circle;
    circle.setRadius(9.0f);
    circle.setFillColor(sf::Color::Red);
    circle.setOrigin(circle.getGeometricCenter());
    circle.setPosition({300.0f,300.0f});

    sf::CircleShape radCircle;
    radCircle.setRadius(40.0f);
    radCircle.setOrigin(radCircle.getGeometricCenter());
    radCircle.setPosition({300.0f,300.0f});
    radCircle.setFillColor(sf::Color::Transparent);
    radCircle.setOutlineColor(sf::Color::Magenta);
    radCircle.setOutlineThickness(2.0f);



    //sprite circle radius: 40
    //origin of sprite circle = sprite.getPosition
    //temp origin sprite opisanej kruznice na vypocty: [300,300]
    std::cout << 40*std::sin(90)+300 << '\n';
    std::cout << 40*std::cos(90)+300 << '\n';

    //X bodu na 90 stupnoch kruznice: 40*sin(90)
    //Y bodu na 90 s kruznice: 40*cos(90)

    std::cout << playerTexture.getSize().x << " je x velkost\n";
    float ix {0};
    std::cout << degToRad(ix) << '\n';


    while(window.isOpen()){

            circle.setPosition({40*std::sin(degToRad(ix+180))+300,40*std::cos(-degToRad(ix+180))+300});
            --ix;
        //movePlayer(player,mainInfo);


        rotatePlayer(player,1);
        //std::cout << player.getRotation().asDegrees() << '\n';





        sf::Time timeElapsed = clock.getElapsedTime();

        angleText.setString(std::to_string(player.getRotation().asDegrees()));


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
        if(timeElapsed.asSeconds()>2)
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

        window.draw(radCircle);
        window.draw(circle);
        window.draw(player);
        window.draw(angleText);
        window.draw(text);
        window.display();
    }

    //delete window;

}
