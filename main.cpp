#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <cmath>

#include <SFML/Graphics.hpp>
#include "headers/Random.h"
#include "headers/Objects.h"
#include "headers/Functions.h"
#include "headers/ConstantsOrAttributes.h"


int main()
{
    Player playerObject{};

    std::vector<Drop> drops;    
    std::vector<Projectile> projectiles;
    std::vector<Target> targets;
    std::vector<Boss> boss;


    sf::CircleShape asteroidSpawn;
    asteroidSpawn.setRadius(spawnRadius);
    asteroidSpawn.setFillColor(sf::Color::Transparent);
    asteroidSpawn.setOutlineColor(sf::Color::Blue);
    asteroidSpawn.setOutlineThickness(2.0f);
    asteroidSpawn.setOrigin(asteroidSpawn.getGeometricCenter());
    asteroidSpawn.setPosition({playerObject.sprite.getPosition().x, playerObject.sprite.getPosition().y});

    sf::RenderWindow window (sf::VideoMode({width,height}),"Asteroids");
    window.setFramerateLimit(framerate);

    GUI guiObject{};
    sf::Clock clock;
    float dt {0};

    while(window.isOpen())
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            auto mousePos = sf::Mouse::getPosition(window);
            auto transMousePos = window.mapPixelToCoords(mousePos);
            //If the user clicks the play againBtn text, reset some data
            if(guiObject.againBtn.getGlobalBounds().contains(transMousePos))
            {

                score = 0;
                previousScore = 0;
                missed = 0;
                fireDelayOriginal = 0.25;
                bossTimer = 6*60;
                bossSpawned = false;
                acceleration  = 12;
                spawnAsteroidInterval = 2;
                movementSpeed = 0;

                boss.clear();
                targets.clear();
                projectiles.clear();
                drops.clear();

                playerObject.blewUp = false;
                playerObject.health = 100;
                playerObject.firerateLvl = 1;
                playerObject.accLvl = 1;
                playerObject.sprite.setPosition({width/2,height/2});


            }
        }

        while(!playerObject.blewUp)
        {

            sf::Time timeElapsed = clock.getElapsedTime();
            dt = timeElapsed.asSeconds()*timeSpeed;
            secSinceSpawn = secSinceSpawn + dt;
            fireDelay = fireDelay - dt;
            bossTimer = bossTimer - dt;

            guiObject.hpTxt.setString("Hull integrity: " + std::to_string(playerObject.health) + "%");
            guiObject.accTxt.setString("Aceleration level: " + std::to_string(playerObject.accLvl));
            guiObject.firerateTxt.setString("Firerate level: " + std::to_string(playerObject.firerateLvl));


            if(static_cast<int>(bossTimer)%60<10)
            {
                guiObject.timerTxt.setString("The lord is here in: " + std::to_string(static_cast<int>(bossTimer)/60) + ":0" + std::to_string(static_cast<int>(bossTimer)%60));
            }
            else
            {
                guiObject.timerTxt.setString("The lord is here in: " + std::to_string(static_cast<int>(bossTimer)/60) + ":" + std::to_string(static_cast<int>(bossTimer)%60));
            }

            playerObject.handleScreenWrapping();

            // player movement and shooting handling
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {playerObject.rotatePlayer(-1, dt, rotationSpeed); }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {playerObject.rotatePlayer(1, dt, rotationSpeed); }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            {
                if(movementSpeed < maxSpeed) {movementSpeed += acceleration*dt/2; }
                playerObject.movePlayer(dt,movementSpeed);
            }
            if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && movementSpeed > 0)
            {
                movementSpeed -= deacceleration*dt/2;
                playerObject.movePlayer(dt,movementSpeed);
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && fireDelay <= 0)
            {
                projectiles.emplace_back(Projectile(playerObject.getGunRectXPos(), playerObject.getGunRectYPos(), playerObject.sprite.getRotation()));
                fireDelay = fireDelayOriginal;
            }
            

            while(const std::optional event = window.pollEvent())
            {
                if(event->is<sf::Event::Closed>())
                {
                    playerObject.blewUp = true;
                    window.close();
                }
                else if(const auto*keyPressed = event->getIf<sf::Event::KeyPressed>())
                {
                    if(keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    {
                        playerObject.blewUp = true;
                        window.close();
                    }
                }
            }

            guiObject.hudTxt.setString("Score: " + std::to_string(score) + " Missed: " + std::to_string(missed));

            spawnAsteroidInterval = spawnAsteroidInterval - difIncrease*dt;
            if(secSinceSpawn > spawnAsteroidInterval && bossTimer >0)
            {
                int ranDegree = Random::get(0,360);
                targets.emplace_back(spawnRadius*std::sin(degToRad(ranDegree))+playerObject.sprite.getPosition().x,spawnRadius*std::cos(degToRad(ranDegree))+playerObject.sprite.getPosition().y,100);

                targets.back().xMoveVector = (playerObject.sprite.getPosition().x - targets.back().xTarget);
                targets.back().yMoveVector = (playerObject.sprite.getPosition().y - targets.back().yTarget);

                secSinceSpawn = 0;
            }

            if(bossTimer < 0 && bossSpawned == false)
            {
                bossSpawned = true;
                int ranDegree = Random::get(0,360);
                boss.emplace_back(spawnRadius*std::sin(degToRad(ranDegree))+playerObject.sprite.getPosition().x,spawnRadius*std::cos(degToRad(ranDegree))+playerObject.sprite.getPosition().y,100);

            }

            if(bossSpawned == true)
            {
                boss.back().xMoveVector = 5*(playerObject.sprite.getPosition().x - boss.back().xTarget);
                boss.back().yMoveVector = 5*(playerObject.sprite.getPosition().y - boss.back().yTarget);
                boss[0].moveBoss(dt);

                for(unsigned int j {0}; j<projectiles.size();++j)
                {
                    if(projectiles[j].shape.getGlobalBounds().findIntersection(boss[0].shape.getGlobalBounds()) && !projectiles[j].blickSum)
                    {
                        boss[0].health-= 1;
                        std::cerr << "Bos health: " << boss[0].health << '\n';

                        if(boss[0].health<250)
                        {
                            boss[0].shape.setRadius(50);
                            boss[0].shape.setTextureRect(sf::IntRect({0,0},{100,100}));
                            boss[0].shape.setTexture(&bossM);
                        }
                        if(boss[0].health<100)
                        {
                            boss[0].shape.setRadius(25);
                            boss[0].shape.setTextureRect(sf::IntRect({0,0},{50,50}));
                            boss[0].shape.setTexture(&bossS);
                        }

                        projectiles[j].blickSum = true;
                    }


                }

                //Collisions for boss
                if(boss[0].shape.getGlobalBounds().findIntersection(playerObject.sprite.getGlobalBounds()))
                {
                    if(playerObject.health - bossDamage <= 0)
                    {
                        playerObject.blewUp = true;
                        std::cout << "Kaboom\n";
                    }
                    playerObject.health -= bossDamage;
                }
            }

            for(unsigned int i {0}; i<projectiles.size();++i)
            {
                projectiles[i].moveProjectile(dt, 30);
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
                            score=score+10;
                            targets[i].wasClicked = true;
                            if(score % 100 == 0)
                            {
                                drops.emplace_back(targets[i].xTarget,targets[i].yTarget);
                                goto dropCreated;
                            }
                            dropCreated:
                            if(targets[i].radius>25)
                            {
                                targets.emplace_back(targets[i].xTarget,targets[i].yTarget,targets[i].radius/2);
                                targets.back().xMoveVector = (playerObject.sprite.getPosition().x - targets.back().xTarget);
                                targets.back().yMoveVector = (playerObject.sprite.getPosition().y - targets.back().yTarget);
                            }
                            projectiles[j].blickSum = true;
                        }
                }
                if(!playerObject.blewUp)
                {
                    if(targets[i].shape.getGlobalBounds().findIntersection(playerObject.sprite.getGlobalBounds()) && !targets[i].wasClicked)
                    {

                        targets[i].wasClicked = true;
                        if(playerObject.health - astDamage <= 0)
                        {
                            playerObject.blewUp = true;
                            std::cout << "Kaboom\n";
                        }
                        playerObject.health -= astDamage;

                    }


                }
            }


            for(unsigned int i {0}; i<drops.size(); ++i)
            {
                if(drops[i].shape.getGlobalBounds().findIntersection(playerObject.sprite.getGlobalBounds()) && !drops[i].pickedUp)
                {
                    if(drops[i].ranNum == 0)
                    {
                        drops[i].sprite.setTexture(firerateSprite);
                        fireDelayOriginal -= 0.02;
                        ++playerObject.firerateLvl;
                        drops[i].pickedUp = true;
                    }
                    if(drops[i].ranNum == 1)
                    {
                        drops[i].sprite.setTexture(armorSprite);
                        playerObject.health += astDamage;
                        drops[i].pickedUp = true;
                    }
                    if(drops[i].ranNum == 2)
                    {
                        drops[i].sprite.setTexture(accelerationSprite);
                        acceleration += 2;
                        ++playerObject.accLvl;
                        drops[i].pickedUp = true;
                    }
                }
            }


            //ANIMACIE

            // Sprajtu:
            playerObject.texTimer += dt;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {playerObject.handleAnimationForward(); }
            else
            {
                playerObject.sprite.setTextureRect({{0,0},{playerObject.m_FRAME_WIDTH, playerObject.m_FRAME_HEIGHT}});
                playerObject.texWidth = 0;
                playerObject.texTimer = 0;
            }

            //Render
            window.clear(sf::Color::Black);

            //DRAWING
            window.draw(guiObject.backgSprite);
            for(unsigned int i {0};i<targets.size();++i)
            {
                if(!(targets[i].wasClicked))
                {
                    window.draw(targets[i].shape);
                    targets[i].secondsExisted += dt;
                }
            }

            for(unsigned int i {0};i < drops.size();++i)
            {
                if(!(drops[i].pickedUp))
                {
                    window.draw(drops[i].shape);
                    window.draw(drops[i].sprite);
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

            for(unsigned int i {0}; i<boss.size(); ++i)
                {
                    window.draw(boss[i].shape);
                }


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

            clock.restart();

            window.draw(playerObject.sprite);
            window.draw(asteroidSpawn);
            window.draw(guiObject.hudTxt);
            window.draw(guiObject.hpTxt);
            window.draw(guiObject.firerateTxt);
            window.draw(guiObject.accTxt);

            if(playerObject.blewUp)
            {
                window.draw(guiObject.ambatuTxt);
                window.draw(guiObject.againBtnTxt);
            }
            if(bossTimer < 301 && bossTimer > 0)
            {
                window.draw(guiObject.timerTxt);
            }


            window.display();

        }
    }

}
