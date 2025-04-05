#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "headers/Random.h"
#include "headers/Objects.h"
#include <string>
#include <string_view>

constexpr unsigned int width {600};
constexpr unsigned int height {600};
int score {0};
int previousScore {0};
int missed {0};




float degToRad(int degrees)
{
    return degrees*(3.1415926535/180);
}

sf::Texture playerTexture("sprites/theTriangleFixed.png",false,sf::IntRect({0,0},{50,80}));

struct Player
{
    float xPlayer {0};
    float yPlayer {0};
    sf::Sprite sprite{playerTexture};
    Player(float xPos, float yPos)
    {

        sprite.setPosition({xPos,yPos});
        xPlayer = xPos;
        yPlayer = yPos;
        sprite.setOrigin({25.0f,40.0f});
    }


    void rotatePlayer(int degrees,float dt,int rotationSpeed)
    {
        sprite.rotate(dt*rotationSpeed*(sf::degrees(degrees)));
    }

    void movePlayer(float dt,int movementSpeed)
    {
            sprite.move({movementSpeed*(40*std::sin(degToRad(-sprite.getRotation().asDegrees()+180))+xPlayer - xPlayer)*dt,
                        movementSpeed*(40*std::cos(degToRad(-sprite.getRotation().asDegrees()+180))+yPlayer - yPlayer)*dt});
            xPlayer = sprite.getPosition().x;
            yPlayer = sprite.getPosition().y;
    }

};


struct Projectile
{
    sf::RectangleShape shape;
    float xProjectile {0};
    float yProjectile {0};

    Projectile(float xPos, float yPos,sf::Angle Rotation)
    {
        shape.setSize({5.0f,30.0f});
        shape.setOrigin(shape.getGeometricCenter());
        shape.setFillColor(sf::Color::Red);
        shape.setPosition({xPos,yPos});
        shape.setRotation(Rotation);
        xProjectile = xPos;
        yProjectile = yPos;


    }
    void moveProjectile(float dt,int movementSpeed)
    {
            shape.move({movementSpeed*(40*std::sin(degToRad(-shape.getRotation().asDegrees()+180))+xProjectile - xProjectile)*dt,
                        movementSpeed*(40*std::cos(degToRad(-shape.getRotation().asDegrees()+180))+yProjectile - yProjectile)*dt});
            xProjectile = shape.getPosition().x;
            yProjectile = shape.getPosition().y;
    }

};

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
    Player pSprite(300,300);

    sf::CircleShape pCircle;
    pCircle.setRadius(40.0f);
    pCircle.setFillColor(sf::Color::Transparent);
    pCircle.setOutlineThickness(2.0f);
    pCircle.setOutlineColor(sf::Color::Magenta);
    pCircle.setOrigin(pCircle.getGeometricCenter());
    pCircle.setPosition({pSprite.xPlayer,pSprite.yPlayer});


    std::vector<Projectile> projectiles;

    sf::RectangleShape gunRect;
    gunRect.setSize({5.0f,30.0f});
    gunRect.setFillColor(sf::Color::Red);
    gunRect.setOrigin(gunRect.getGeometricCenter());
    gunRect.setPosition({pSprite.xPlayer,pSprite.yPlayer});

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
    angleText.setCharacterSize(30);
    angleText.setFillColor(sf::Color::Red);
    angleText.setPosition({150.0f,150.0f});




    sf::Clock clock;
    float dt {0};
    int rotationSpeed {300};
    int movementSpeed {10};



    while(window.isOpen()){

        sf::Time timeElapsed = clock.getElapsedTime();
        dt = timeElapsed.asSeconds();
        secSinceSpawn = secSinceSpawn + dt;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            pSprite.rotatePlayer(-1,dt,rotationSpeed);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            pSprite.rotatePlayer(1,dt,rotationSpeed);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            pSprite.movePlayer(dt,movementSpeed);

        }

        pCircle.setPosition({pSprite.xPlayer,pSprite.yPlayer});
        float spriteRotation = pSprite.sprite.getRotation().asDegrees();
        std::cout << spriteRotation << '\n';
        float xGunRect = 40*std::sin(-degToRad(spriteRotation+180))+pSprite.xPlayer;
        float yGunRect = 40*std::cos(-degToRad(spriteRotation+180))+pSprite.yPlayer;
        gunRect.setPosition({xGunRect,yGunRect});
        gunRect.setRotation(pSprite.sprite.getRotation());

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
        {
            projectiles.emplace_back(Projectile(xGunRect,yGunRect,pSprite.sprite.getRotation()));

        }



        std::cout << projectiles.size() << '\n';
        angleText.setString(std::to_string(pSprite.sprite.getRotation().asDegrees()));

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
        for(unsigned int i {0}; i<projectiles.size();++i)
            {
                projectiles[i].moveProjectile(dt,30);
            }


        for(unsigned int i {0};i<projectiles.size();++i)
        {
            window.draw(projectiles[i].shape);
        }

        clock.restart();
        window.draw(pSprite.sprite);
        window.draw(pCircle);
        window.draw(gunRect);
        window.draw(angleText);
        window.draw(text);
        window.display();
    }

    //delete window;

}
