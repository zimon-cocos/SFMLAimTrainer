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


sf::Texture playerTexture("sprites/theTriangleFixed.png",false,sf::IntRect({0,0},{50,80}));

struct playerInfo
{
    float xPlayer {300.0f};
    float yPlayer {300.0f};

};



struct player
{
    float xPlayer {0};
    float yPlayer {0};
    sf::Sprite sprite(playerTexture);
    player(float xPos, float yPos)
    {

        sprite.setPosition({xPos,yPos});
        xPlayer = xPos;
        yPlayer = yPos;
    }


};


struct projectie
{
    sf::RectangleShape shape;
};


float degToRad(int degrees)
{
    return degrees*(3.1415926535/180);
}

void movePlayer(sf::Sprite& toMove,playerInfo& Info, float dt,int movementSpeed)
{
            toMove.move({movementSpeed*(40*std::sin(degToRad(-toMove.getRotation().asDegrees()+180))+Info.xPlayer - Info.xPlayer)*dt,
                        movementSpeed*(40*std::cos(degToRad(-toMove.getRotation().asDegrees()+180))+Info.yPlayer - Info.yPlayer)*dt});

            Info.xPlayer = Info.xPlayer + 40*std::sin(degToRad(-toMove.getRotation().asDegrees()+180))+Info.xPlayer - Info.xPlayer;
            Info.yPlayer = Info.yPlayer + 40*std::cos(degToRad(-toMove.getRotation().asDegrees()+180))+Info.yPlayer - Info.yPlayer;
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

    float secondsExisted {0};
    bool wasClicked {false};

};

float secSinceSpawn{0};


int main()
{
    std::vector<Target> targets;
    targets.emplace_back(Random::get(0,600),Random::get(0,500));

    player Cuh(300,300);
    //*************!!!!!!!!!!!!!!!!!*********************
    sf::RenderWindow window (sf::VideoMode({width,height}),"Aim Trainer");
    window.setFramerateLimit(60);
    //*************!!!!!!!!!!!!!!!!!*********************


    sf::Font font("fonts/Jersey_15/Jersey15-Regular.ttf");

    sf::Text text(font);
    text.setString("It works");
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Red);
    text.setPosition({width/2 - 80,30});


    sf::Text angleText(font);
    angleText.setString("It works");
    angleText.setCharacterSize(12);
    angleText.setFillColor(sf::Color::Red);
    angleText.setPosition({300.0f,300.0f});


    playerInfo mainInfo;
    mainInfo.xPlayer = 400.0f;
    mainInfo.yPlayer = 400.0f;
    float mVektorX {0};
    float mVektorY {0};

//    sf::Texture playerTexture("sprites/theTriangleFixed.png",false,sf::IntRect({0,0},{50,80}));
    sf::Sprite player(playerTexture);
    player.setOrigin({25.0f,40.0f});
    player.setPosition({mainInfo.xPlayer,mainInfo.yPlayer});


    sf::CircleShape circle;
    circle.setRadius(9.0f);
    circle.setFillColor(sf::Color::Red);
    circle.setOrigin(circle.getGeometricCenter());
    circle.setPosition({300.0f,300.0f});

    sf::CircleShape radCircle;
    radCircle.setRadius(40.0f);
    radCircle.setOrigin(radCircle.getGeometricCenter());
    radCircle.setPosition({mainInfo.xPlayer,mainInfo.yPlayer});
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

    float ix {0};

    sf::Clock clock;
    float dt {0};
    int rotationSpeed {300};
    int movementSpeed {10};



    while(window.isOpen()){

        //circle.setPosition({40*std::sin(degToRad(-player.getRotation().asDegrees()+180))+mainInfo.xPlayer,
        //                   40*std::cos(-degToRad(-player.getRotation().asDegrees()+180))+mainInfo.yPlayer});
        //ix = ix-1*dt*rotationSpeed;
        //movePlayer(player,mainInfo);



        sf::Time timeElapsed = clock.getElapsedTime();
        dt = timeElapsed.asSeconds();
        secSinceSpawn = secSinceSpawn + dt;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            rotatePlayer(player,-1*dt*rotationSpeed);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            rotatePlayer(player,1*dt*rotationSpeed);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            movePlayer(player,mainInfo,dt,movementSpeed);
        }


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
                        std::cout << targets[i].secondsExisted << '\n';
                        ++score;

                    }
                }


        }
        text.setString("Score: " + std::to_string(score) + " Missed: " + std::to_string(missed));
        if(secSinceSpawn>2)
        {
            targets.emplace_back(Random::get(0,600),Random::get(0,500));
            secSinceSpawn = 0;
        }

        for(unsigned int i {0};i<targets.size();++i)
            {
                if((targets[i].secondsExisted >= 5) && !(targets[i].wasClicked))
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
                targets[i].secondsExisted += dt;
            }

        }
        clock.restart();
        window.draw(Cuh.sprite);
        window.draw(radCircle);
        window.draw(circle);
        window.draw(player);
        window.draw(angleText);
        window.draw(text);
        window.display();
    }

    //delete window;

}
