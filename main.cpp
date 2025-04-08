#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "headers/Random.h"
#include "headers/Objects.h"
#include "headers/Functions.h"
#include "headers/ConstantsOrAttributes.h"
#include <algorithm>
#include <cstdlib>

int main()
{

    std::vector<Target> targets;
    //targets.emplace_back(Random::get(0,600),Random::get(0,500));
    Player pSprite(300,300);
    pSprite.sprite.setTextureRect({ {0, 0}, {50, 80} });

    constexpr float spawnRadius {750.0f};
    sf::CircleShape astSpawn;
    astSpawn.setRadius(spawnRadius);
    astSpawn.setFillColor(sf::Color::Transparent);
    astSpawn.setOutlineColor(sf::Color::Blue);
    astSpawn.setOutlineThickness(2.0f);
    astSpawn.setOrigin(astSpawn.getGeometricCenter());

    std::vector<Projectile> projectiles;
    /*sf::CircleShape gunRect;
    gunRect.setRadius(15.0f);
    gunRect.setFillColor(sf::Color::Red);
    gunRect.setOrigin(gunRect.getGeometricCenter());
    gunRect.setPosition({pSprite.xPlayer,pSprite.yPlayer});
*/
    //*************!!!!!!!!!!!!!!!!!*********************
    sf::RenderWindow window (sf::VideoMode({width,height}),"Aim Trainer");
    window.setFramerateLimit(framerate);
    //*************!!!!!!!!!!!!!!!!!*********************


    sf::Font font("fonts/Jersey_15/Jersey15-Regular.ttf");

    sf::Text text(font);
    text.setString("It works");
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Red);
    text.setPosition({width/2 - 80,30});


    sf::Clock clock;
    float dt {0};

    while(window.isOpen()){

        sf::Time timeElapsed = clock.getElapsedTime();
        dt = timeElapsed.asSeconds();
        secSinceSpawn = secSinceSpawn + dt;
        fireDelay = fireDelay - dt;

        if(pSprite.xPlayer > width+50)
        {
            pSprite.sprite.setPosition({-50.0f,pSprite.yPlayer});

        }
        if(pSprite.yPlayer > height+50)
        {
            pSprite.sprite.setPosition({pSprite.xPlayer,-50.0f});

        }
        if(pSprite.xPlayer < -50.0f)
        {
            pSprite.sprite.setPosition({650.0f,pSprite.yPlayer});

        }

        if(pSprite.yPlayer < -50.0f)
        {
            pSprite.sprite.setPosition({pSprite.xPlayer,650.0f});

        }





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
            //akceleracia
            if(movementSpeed < maxSpeed)
            {
                std::cout << "M speed: " << movementSpeed << '\n';
                std::cout << acceleration*dt << '\n';
                movementSpeed += acceleration*dt/2;
            }

            pSprite.movePlayer(dt,movementSpeed);
        }

        if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && movementSpeed > 0)
        {
            //deakceleracia
            movementSpeed -= acceleration*dt/2;
            pSprite.movePlayer(dt,movementSpeed);
        }




        astSpawn.setPosition({pSprite.xPlayer,pSprite.yPlayer});
        float spriteRotation = pSprite.sprite.getRotation().asDegrees();

        float xGunRect = 40*std::sin(-degToRad(spriteRotation+180))+pSprite.xPlayer;
        float yGunRect = 40*std::cos(-degToRad(spriteRotation+180))+pSprite.yPlayer;

        //gunRect.setPosition({xGunRect,yGunRect});
        //gunRect.setRotation(pSprite.sprite.getRotation());

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && fireDelay<= 0)
        {
            projectiles.emplace_back(Projectile(xGunRect,yGunRect,pSprite.sprite.getRotation()));
            fireDelay = fireDelayOriginal;
        }

        while(const std::optional event = window.pollEvent())
        {

            if(event->is<sf::Event::Closed>())
            {

                window.close();

            }
        else if(const auto*keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if(keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                {
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

        toSpawn = toSpawn - difIncrease*dt;
        //std::cout << "Difficulty: " << toSpawn << '\n';
        if(secSinceSpawn>toSpawn)
        {
            //targets.emplace_back(Random::get(0,600),Random::get(0,500));
            //float xGunRect = 40*std::sin(-degToRad(spriteRotation+180))+pSprite.xPlayer;
            //float yGunRect = 40*std::cos(-degToRad(spriteRotation+180))+pSprite.yPlayer;
            int ranDegree = Random::get(0,360);
            targets.emplace_back(spawnRadius*std::sin(degToRad(ranDegree))+pSprite.xPlayer,spawnRadius*std::cos(degToRad(ranDegree))+pSprite.yPlayer,100);

            targets.back().xMoveVector = (pSprite.xPlayer - targets.back().xTarget);
            targets.back().yMoveVector = (pSprite.yPlayer - targets.back().yTarget);

            secSinceSpawn = 0;
        }


        for(unsigned int i {0}; i<projectiles.size();++i)
            {
                projectiles[i].moveProjectile(dt,30);
            }


       for(unsigned int i {0}; i<targets.size(); ++i)
            {
            targets[i].moveTarget(dt);
            if((targets[i].secondsExisted >= maxTargetLifetime) && !(targets[i].wasClicked))
            {
                targets[i].wasClicked = true;
                ++missed;
            }
            for(unsigned int j {0}; j<projectiles.size();++j)
            {
                    if(projectiles[j].shape.getGlobalBounds().findIntersection(targets[i].shape.getGlobalBounds()) && !projectiles[j].blickSum)
                    {
                            ++hitAmount;
                            ++score;
                            targets[i].wasClicked = true;
                            if(targets[i].radius>25)
                            {
                                targets.emplace_back(targets[i].xTarget,targets[i].yTarget,targets[i].radius/2);
                                targets.back().xMoveVector = (pSprite.xPlayer - targets.back().xTarget);
                                targets.back().yMoveVector = (pSprite.yPlayer - targets.back().yTarget);
                            }
                            projectiles[j].blickSum = true;
                    }
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

    //ANIMACIE

    //Sprajtu:
    pSprite.texTimer += dt;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    {
        if(pSprite.texTimer >= 0.05)
        {
            pSprite.texWidth += 50;

            if(pSprite.texWidth >= shipTexture.getSize().x)
            {
                pSprite.texWidth = 0;
            }
            if(pSprite.texWidth < shipTexture.getSize().x)
            {

                pSprite.sprite.setTextureRect({ {pSprite.texWidth,0} , {50,80} });
            }

            pSprite.texTimer = 0;
        }
    }
    else
    {
        pSprite.sprite.setTextureRect({{0,0},{50,80}});
        pSprite.texWidth = 0;
    }



        //Render
        window.clear(sf::Color::Black);

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
                [](const Projectile& proj) { return proj.lifetime > 3.0f; }
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
        window.draw(text);
        window.display();
    }

    //delete window;

}
