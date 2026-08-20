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
                fireDelay = 0.25;
                bossTimer = 6*60;
                bossSpawned = false;
                spawnAsteroidInterval = 2;

                boss.clear();
                targets.clear();
                projectiles.clear();
                drops.clear();

                playerObject.reset(width / 2, height / 2);


            }
        }

        while(!playerObject.blewUp)
        {

            sf::Time timeElapsed = clock.getElapsedTime();
            dt = timeElapsed.asSeconds()*timeSpeed;
            secSinceSpawn = secSinceSpawn + dt;
            secSinceFiring = secSinceFiring - dt;
            bossTimer = bossTimer - dt;

            guiObject.setLevelsTxt(playerObject);

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
                if(playerObject.m_movementSpeed < maxSpeed) {playerObject.m_movementSpeed += playerObject.m_acceleration*dt/2; }
                playerObject.movePlayer(dt, playerObject.m_movementSpeed);
            }
            if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && playerObject.m_movementSpeed > 0)
            {
                playerObject.m_movementSpeed -= deacceleration*dt/2;
                playerObject.movePlayer(dt,playerObject.m_movementSpeed);
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && secSinceFiring <= 0)
            {
                projectiles.emplace_back(Projectile(playerObject.getGunRectXPos(), playerObject.getGunRectYPos(), playerObject.sprite.getRotation()));
                secSinceFiring = fireDelay;
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

            spawnAsteroidInterval = spawnAsteroidInterval - difIncrease * dt;
            if(secSinceSpawn > spawnAsteroidInterval && bossTimer > 0)
            {
                int ranDegree = Random::get(0, 360);
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
                boss[0].moveBoss(dt, playerObject);

                for(auto& curr_projectile : projectiles)
                {
                    if(curr_projectile.shape.getGlobalBounds().findIntersection(boss[0].shape.getGlobalBounds()) && !curr_projectile.blickSum)
                    {
                        boss[0].health -= 1;
                        std::cerr << "Boss health: " << boss[0].health << '\n';

                        if(boss[0].health < 250)
                        {
                            boss[0].shape.setRadius(50);
                            boss[0].shape.setTextureRect(sf::IntRect({0,0}, {100,100}));
                            boss[0].shape.setTexture(&bossM);
                        }
                        if(boss[0].health < 100)
                        {
                            boss[0].shape.setRadius(25);
                            boss[0].shape.setTextureRect(sf::IntRect({0,0}, {50,50}));
                            boss[0].shape.setTexture(&bossS);
                        }

                        curr_projectile.blickSum = true;
                    }
                }

                // Collisions for boss
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

            for (auto& curr_projectile : projectiles) {curr_projectile.moveProjectile(dt, projectileSpeed); }
    
            // handle asteroid (target) interactions
            for(auto& curr_target : targets)
            {
                curr_target.moveTarget(dt);
                missed += curr_target.targetExpiredOrNot();


                for(auto& curr_projectile : projectiles)
                {
                    if(curr_projectile.shape.getGlobalBounds().findIntersection(curr_target.shape.getGlobalBounds()) && !curr_projectile.blickSum)
                    {
                        score += 10;
                        curr_target.wasClicked = true;
                        if(score % 100 == 0) {drops.emplace_back(curr_target.xTarget, curr_target.yTarget); }

                        if(curr_target.radius > 25)
                        {
                            targets.emplace_back(curr_target.xTarget, curr_target.yTarget, curr_target.radius/2);
                            targets.back().xMoveVector = (playerObject.sprite.getPosition().x - targets.back().xTarget);
                            targets.back().yMoveVector = (playerObject.sprite.getPosition().y - targets.back().yTarget);
                        }
                        curr_projectile.blickSum = true;
                    }
                }

                if(!playerObject.blewUp)
                {
                    if(curr_target.shape.getGlobalBounds().findIntersection(playerObject.sprite.getGlobalBounds()) && !curr_target.wasClicked)
                    {
                        curr_target.wasClicked = true;
                        if(playerObject.health - astDamage <= 0)
                        {
                            playerObject.blewUp = true;
                            std::cout << "Kaboom\n";
                        }
                        playerObject.health -= astDamage;

                    }
                }
            }
            // handle drop interactions
            for(auto& curr_drop: drops)
            {
                if(curr_drop.shape.getGlobalBounds().findIntersection(playerObject.sprite.getGlobalBounds()) && !curr_drop.pickedUp)
                {
                    if(curr_drop.dropType == curr_drop.firerate)
                    {
                        fireDelay -= 0.02;
                        ++playerObject.firerateLvl;
                        curr_drop.pickedUp = true;
                    }
                    if(curr_drop.dropType == curr_drop.armor)
                    {
                        playerObject.health += astDamage;
                        curr_drop.pickedUp = true;
                    }
                    if(curr_drop.dropType == curr_drop.acceleration)
                    {
                        acceleration += 2;
                        ++playerObject.accLvl;
                        curr_drop.pickedUp = true;
                    }
                }
            }


            //ANIMACIE

            // Sprajtu:
            playerObject.texTimer += dt;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {playerObject.handleAnimationForward(); }
            else
            {
                playerObject.sprite.setTextureRect({sf::Vector2i{0,0}, {playerObject.m_FRAME_WIDTH, playerObject.m_FRAME_HEIGHT}});
                playerObject.texWidth = 0;
                playerObject.texTimer = 0;
            }

            //Render
            window.clear(sf::Color::Black);

            //DRAWING
            window.draw(guiObject.backgSprite);

            for(Target& curr_target: targets)
            {
                if(!(curr_target.wasClicked))
                {
                    window.draw(curr_target.shape);
                    curr_target.secondsExisted += dt;
                }
            }

            for(auto& curr_drop: drops)
            {
                if(!(curr_drop.pickedUp))
                {
                    window.draw(curr_drop.shape);
                    window.draw(curr_drop.sprite);
                }

            }

            for(Projectile& curr_projectile : projectiles)
            {
                curr_projectile.lifetime += dt;
                if(curr_projectile.lifetime <= maxLifetime && !curr_projectile.blickSum) {window.draw(curr_projectile.shape); }
            }

            if (bossTimer <= 0) {window.draw(boss[0].shape);}

            // projectiles and targets cleanup
            std::erase_if(projectiles, [](const Projectile& proj) {return proj.lifetime > maxProjectileLifetime; });
            std::erase_if(targets, [](const Target& target) {return target.wasClicked; });

            clock.restart();

            window.draw(playerObject.sprite);

            window.draw(guiObject.hudTxt);
            window.draw(guiObject.hpTxt);
            window.draw(guiObject.firerateTxt);
            window.draw(guiObject.accTxt);

            if(playerObject.blewUp)
            {
                window.draw(guiObject.ambatuTxt);
                window.draw(guiObject.againBtnTxt);
            }

            if(bossTimer < 301 && bossTimer > 0) {window.draw(guiObject.timerTxt);}

            window.display();
        }
    }
}
