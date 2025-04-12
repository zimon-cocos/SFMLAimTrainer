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

    sf::Sprite backgSprite (background);
    backgSprite.setPosition({0,0});

    std::vector<Target> targets;
    std::vector<Boss> boss;

    Player pSprite(300,300);
    pSprite.sprite.setTextureRect({ {0, 0}, {50, 80} });
    pSprite.sprite.setScale({1.2,1.2});

    sf::CircleShape astSpawn;
    astSpawn.setRadius(spawnRadius);
    astSpawn.setFillColor(sf::Color::Transparent);
    astSpawn.setOutlineColor(sf::Color::Blue);
    astSpawn.setOutlineThickness(2.0f);
    astSpawn.setOrigin(astSpawn.getGeometricCenter());

    std::vector<Projectile> projectiles;
    std::vector<Drop> drops;

    sf::RenderWindow window (sf::VideoMode({width,height}),"Asteroids");
    window.setFramerateLimit(framerate);

    sf::Font font("fonts/Jersey_15/Jersey15-Regular.ttf");
    sf::Text text(font);
    text.setString("It works");
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Red);
    text.setPosition({width/2 - 80,30});

    sf::Text timerTxt(font);
    timerTxt.setString("placeholder");
    timerTxt.setCharacterSize(30);
    timerTxt.setFillColor(sf::Color::Red);
    timerTxt.setPosition({width/2-120,60});

    sf::Text hpTxt(font);
    hpTxt.setString("placeholder");
    hpTxt.setCharacterSize(24);
    hpTxt.setFillColor(sf::Color::Red);
    hpTxt.setPosition({675,30});

    sf::Text accTxt(font);
    accTxt.setString("placeholder");
    accTxt.setCharacterSize(24);
    accTxt.setFillColor(sf::Color::Red);
    accTxt.setPosition({675,50});

    sf::Text firerateTxt(font);
    firerateTxt.setString("placeholder");
    firerateTxt.setCharacterSize(24);
    firerateTxt.setFillColor(sf::Color::Red);
    firerateTxt.setPosition({675,70});

    sf::Text ambatuTxt(font);
    ambatuTxt.setString("You blew up!");
    ambatuTxt.setCharacterSize(48);
    ambatuTxt.setFillColor(sf::Color::Magenta);
    ambatuTxt.setPosition({width/2-100,height/2-100});

    sf::Text againTxt(font);
    againTxt.setString("Play again?");
    againTxt.setCharacterSize(72);
    againTxt.setFillColor(sf::Color::Magenta);
    againTxt.setPosition({width/2-100-30,height/2-100 +40});

    sf::RectangleShape again;
    again.setSize({290.0f,40.0f});
    again.setPosition({width/2-100-30,height/2-100 + 72});
    again.setFillColor(sf::Color::Transparent);
    again.setOutlineThickness(2.0f);
    again.setOutlineColor(sf::Color::Red);

    sf::Clock clock;
    float dt {0};

    while(window.isOpen()){
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

                    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                    {

                    auto mousePos = sf::Mouse::getPosition(window);
                    auto transMousePos = window.mapPixelToCoords(mousePos);
                        //If the user clicks the play again text, reset some data
                        if(again.getGlobalBounds().contains(transMousePos))
                        {

                            score = 0;
                            previousScore = 0;
                            missed = 0;
                            fireDelayOriginal = 0.25;
                            bossTimer = 6*60;
                            bossSpawned = false;
                            acceleration  = 12;
                            toSpawn = 2;
                            movementSpeed = 0;

                            boss.clear();
                            targets.clear();
                            projectiles.clear();
                            drops.clear();

                            pSprite.blewUp = false;
                            pSprite.health = 100;
                            pSprite.firerateLvl = 1;
                            pSprite.accLvl = 1;
                            pSprite.xPlayer = 300;
                            pSprite.yPlayer = 300;
                            pSprite.sprite.setPosition({width/2,height/2});


                        }
                    }
            while(!pSprite.blewUp)
            {

                    sf::Time timeElapsed = clock.getElapsedTime();
                    dt = timeElapsed.asSeconds()*timeSpeed;
                    secSinceSpawn = secSinceSpawn + dt;
                    fireDelay = fireDelay - dt;
                    bossTimer = bossTimer - dt;

                    hpTxt.setString("Hull integrity: " + std::to_string(pSprite.health) + "%");
                    accTxt.setString("Aceleration level: " + std::to_string(pSprite.accLvl));
                    firerateTxt.setString("Firerate level: " + std::to_string(pSprite.firerateLvl));


                    if(static_cast<int>(bossTimer)%60<10)
                    {
                        timerTxt.setString("The lord is here in: " + std::to_string(static_cast<int>(bossTimer)/60) + ":0" + std::to_string(static_cast<int>(bossTimer)%60));
                    }
                    else
                    {
                        timerTxt.setString("The lord is here in: " + std::to_string(static_cast<int>(bossTimer)/60) + ":" + std::to_string(static_cast<int>(bossTimer)%60));
                    }

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
                        pSprite.sprite.setPosition({width+50,pSprite.yPlayer});

                    }

                    if(pSprite.yPlayer < -50.0f)
                    {
                        pSprite.sprite.setPosition({pSprite.xPlayer,height+50});

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
                        //acceleration
                        if(movementSpeed < maxSpeed)
                        {
                            movementSpeed += acceleration*dt/2;
                        }

                        pSprite.movePlayer(dt,movementSpeed);
                    }

                    if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && movementSpeed > 0)
                    {
                        //deacceleration
                        movementSpeed -= deacceleration*dt/2;
                        pSprite.movePlayer(dt,movementSpeed);
                    }

                    astSpawn.setPosition({pSprite.xPlayer,pSprite.yPlayer});
                    float spriteRotation = pSprite.sprite.getRotation().asDegrees();

                    float xGunRect = 40*std::sin(-degToRad(spriteRotation+180))+pSprite.xPlayer;
                    float yGunRect = 40*std::cos(-degToRad(spriteRotation+180))+pSprite.yPlayer;

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
                    if(secSinceSpawn>toSpawn && bossTimer >0)
                    {
                        int ranDegree = Random::get(0,360);
                        targets.emplace_back(spawnRadius*std::sin(degToRad(ranDegree))+pSprite.xPlayer,spawnRadius*std::cos(degToRad(ranDegree))+pSprite.yPlayer,100);

                        targets.back().xMoveVector = (pSprite.xPlayer - targets.back().xTarget);
                        targets.back().yMoveVector = (pSprite.yPlayer - targets.back().yTarget);

                        secSinceSpawn = 0;
                    }

                    if(bossTimer < 0 && bossSpawned == false)
                    {
                        bossSpawned = true;
                        int ranDegree = Random::get(0,360);

                        boss.emplace_back(spawnRadius*std::sin(degToRad(ranDegree))+pSprite.xPlayer,spawnRadius*std::cos(degToRad(ranDegree))+pSprite.yPlayer,100);


                    }
                    if(bossSpawned == true)
                    {
                        boss.back().xMoveVector = 5*(pSprite.xPlayer - boss.back().xTarget);
                        boss.back().yMoveVector = 5*(pSprite.yPlayer - boss.back().yTarget);
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
                            if(boss[0].shape.getGlobalBounds().findIntersection(pSprite.sprite.getGlobalBounds()))
                            {
                                if(pSprite.health - bossDamage <= 0)
                                {
                                    pSprite.blewUp = true;
                                    std::cout << "Kaboom\n";
                                }
                                pSprite.health -= bossDamage;
                            }
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
                                            targets.back().xMoveVector = (pSprite.xPlayer - targets.back().xTarget);
                                            targets.back().yMoveVector = (pSprite.yPlayer - targets.back().yTarget);
                                        }
                                        projectiles[j].blickSum = true;
                                }
                        }
                        if(!pSprite.blewUp)
                        {
                            if(targets[i].shape.getGlobalBounds().findIntersection(pSprite.sprite.getGlobalBounds()) && !targets[i].wasClicked)
                            {

                                targets[i].wasClicked = true;
                                if(pSprite.health - astDamage <= 0)
                                {
                                    pSprite.blewUp = true;
                                    std::cout << "Kaboom\n";
                                }
                                pSprite.health -= astDamage;

                            }


                    }
                    }


                    for(unsigned int i {0}; i<drops.size(); ++i)
                        {
                        if(drops[i].shape.getGlobalBounds().findIntersection(pSprite.sprite.getGlobalBounds()) && !drops[i].pickedUp)
                        {
                            if(drops[i].ranNum == 0)
                            {
                                drops[i].sprite.setTexture(firerateSprite);
                                fireDelayOriginal -= 0.02;
                                ++pSprite.firerateLvl;
                                drops[i].pickedUp = true;
                            }
                            if(drops[i].ranNum == 1)
                            {
                                drops[i].sprite.setTexture(armorSprite);
                                pSprite.health += astDamage;
                                drops[i].pickedUp = true;
                            }
                            if(drops[i].ranNum == 2)
                            {
                                drops[i].sprite.setTexture(accelerationSprite);
                                acceleration += 2;
                                ++pSprite.accLvl;
                                drops[i].pickedUp = true;
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

                    window.draw(backgSprite);
                    for(unsigned int i {0};i<targets.size();++i)
                    {
                        if(!(targets[i].wasClicked))
                        {
                            window.draw(targets[i].shape);
                            targets[i].secondsExisted += dt;
                        }
                    }

                    for(unsigned int i {0};i<drops.size();++i)
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

                    window.draw(pSprite.sprite);
                    window.draw(astSpawn);
                    window.draw(text);
                    window.draw(hpTxt);
                    window.draw(firerateTxt);
                    window.draw(accTxt);

                    if(pSprite.blewUp)
                    {
                        window.draw(ambatuTxt);
                        window.draw(againTxt);
                    }
                    if(bossTimer <301 && bossTimer > 0)
                    {
                        window.draw(timerTxt);
                    }


                    window.display();

                }
            }

    //delete window;
}
