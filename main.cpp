#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "headers/Random.h"
#include "headers/Objects.h"
#include <string>
#include <string_view>
#include <algorithm>
#include <cstdlib>

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
    bool blewUp {0};
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
    sf::CircleShape shape;
    float xProjectile {0};
    float yProjectile {0};
    float lifetime {0};
    bool blickSum {false};

    Projectile(float xPos, float yPos,sf::Angle Rotation)
    {
        shape.setRadius(2.0f);
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


float targSpeed {0.5};
struct Target
{
    sf::CircleShape shape;
    float xTarget {0};
    float yTarget {0};
    float xMoveVector {0};
    float yMoveVector {0};
    Target(float x, float y)
    {
        shape.setPosition({x,y});
        shape.setRadius(30.0f);
        shape.setOrigin(shape.getGeometricCenter());
        shape.setFillColor(sf::Color::Green);
        xTarget = x;
        yTarget = y;
    }

    float secondsExisted {0};
    bool wasClicked {false};


    void moveTarget(float dt)
    {
        shape.move({xMoveVector*dt*targSpeed,yMoveVector*dt*targSpeed});
    }
};

    float secSinceSpawn{0};
    float fireDelay{0.5};


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


    constexpr float spawnRadius {750.0f};
    sf::CircleShape astSpawn;
    astSpawn.setRadius(spawnRadius);
    astSpawn.setFillColor(sf::Color::Transparent);
    astSpawn.setOutlineColor(sf::Color::Blue);
    astSpawn.setOutlineThickness(2.0f);
    astSpawn.setOrigin(astSpawn.getGeometricCenter());



    std::vector<Projectile> projectiles;
    float hitAmount{0};
    /*sf::CircleShape gunRect;
    gunRect.setRadius(15.0f);
    gunRect.setFillColor(sf::Color::Red);
    gunRect.setOrigin(gunRect.getGeometricCenter());
    gunRect.setPosition({pSprite.xPlayer,pSprite.yPlayer});
*/
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
    float maxLifetime {5};
    int rotationSpeed {300};
    int movementSpeed {10};





    while(window.isOpen()){

        sf::Time timeElapsed = clock.getElapsedTime();
        dt = timeElapsed.asSeconds();
        secSinceSpawn = secSinceSpawn + dt;
        fireDelay = fireDelay - dt;

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
        astSpawn.setPosition({pSprite.xPlayer,pSprite.yPlayer});
        float spriteRotation = pSprite.sprite.getRotation().asDegrees();
        //std::cout << spriteRotation << '\n';
        float xGunRect = 40*std::sin(-degToRad(spriteRotation+180))+pSprite.xPlayer;
        float yGunRect = 40*std::cos(-degToRad(spriteRotation+180))+pSprite.yPlayer;
        //gunRect.setPosition({xGunRect,yGunRect});
        //gunRect.setRotation(pSprite.sprite.getRotation());

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && fireDelay<= 0)
        {
            projectiles.emplace_back(Projectile(xGunRect,yGunRect,pSprite.sprite.getRotation()));
            fireDelay = 0.5;
        }



        //std::cout << projectiles.size() << '\n';
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

        if(secSinceSpawn>0.5)
        {
            //targets.emplace_back(Random::get(0,600),Random::get(0,500));
            //float xGunRect = 40*std::sin(-degToRad(spriteRotation+180))+pSprite.xPlayer;
            //float yGunRect = 40*std::cos(-degToRad(spriteRotation+180))+pSprite.yPlayer;
            int ranDegree = Random::get(0,360);
            targets.emplace_back(spawnRadius*std::sin(degToRad(ranDegree))+pSprite.xPlayer,spawnRadius*std::cos(degToRad(ranDegree))+pSprite.yPlayer);


            targets.back().xMoveVector = (pSprite.xPlayer - targets.back().xTarget);
            targets.back().yMoveVector = (pSprite.yPlayer - targets.back().yTarget);
            std::cout << targets.back().xMoveVector << " , " << targets.back().yMoveVector << '\n';

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

        for(unsigned int i {0}; i<projectiles.size();++i)
            {
                projectiles[i].moveProjectile(dt,30);
            }


        for(unsigned int i {0}; i<targets.size();++i)
            {
                targets[i].moveTarget(dt);
            }


       for(unsigned int i {0}; i<targets.size(); ++i)
            {
            for(unsigned int j {0}; j<projectiles.size();++j)
                {
                    if(projectiles[j].shape.getGlobalBounds().findIntersection(targets[i].shape.getGlobalBounds()) && !projectiles[j].blickSum)
                        {
                            std::cout << "Hit\n";
                            ++hitAmount;
                            ++score;
                            std::cout << hitAmount << '\n';
                            targets[i].wasClicked = true;
                            projectiles[j].blickSum = true;

                        }
                    if(!pSprite.blewUp)
                    {
                        if(targets[i].shape.getGlobalBounds().findIntersection(pSprite.sprite.getGlobalBounds()))
                            {
                                pSprite.blewUp = true;
                                std::cout << "Kaboom\n";
                            }
                    }

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

        for(unsigned int i {0};i<projectiles.size();++i)
        {
            projectiles[i].lifetime = projectiles[i].lifetime + dt;
            if(projectiles[i].lifetime <= maxLifetime && !projectiles[i].blickSum)
            {
                window.draw(projectiles[i].shape);
            }

        }


        //Ai slop//
        projectiles.erase(
            std::remove_if(
                projectiles.begin(),
                projectiles.end(),
                [](const Projectile& proj) { return proj.lifetime > 5.0f; }
            ),
            projectiles.end()
        );


        targets.erase(
            std::remove_if(
                targets.begin(),
                targets.end(),
                [](const Target& target) { return target.wasClicked; }
            ),
            targets.end()
        );
        //Ai slop//

        clock.restart();
        window.draw(pSprite.sprite);
        window.draw(astSpawn);
        window.draw(pCircle);
        window.draw(angleText);
        window.draw(text);
        window.display();
    }

    //delete window;

}
