#include <iostream>
#include <algorithm>
#include <vector>

#include <SFML/Graphics.hpp>
#include "headers/Random.h"
#include "headers/Objects.h"

int main()
{

    enum class GameState
    {
        playing,
        blewUp
    };


    Player playerObject{};
    
    std::vector<Drop> drops;    
    std::vector<Projectile> projectiles;
    std::vector<Target> targets;

    projectiles.reserve(200); 
    targets.reserve(100);
    drops.reserve(50);

    std::vector<Boss> boss;

    sf::CircleShape asteroidSpawn;
    asteroidSpawn.setRadius(Constants::spawnRadius);
    asteroidSpawn.setFillColor(sf::Color::Transparent);
    asteroidSpawn.setOrigin(asteroidSpawn.getGeometricCenter());
    asteroidSpawn.setPosition({playerObject.sprite.getPosition().x, playerObject.sprite.getPosition().y});

    sf::RenderWindow window (sf::VideoMode({Constants::width, Constants::height}),"Asteroids");
    window.setFramerateLimit(Constants::framerate);

    GUI guiObject{};

    sf::Clock clock;
    float dt {0};

    GameState currState = GameState::playing;
    while(window.isOpen())
    {

        while(const std::optional event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>()) {window.close(); }
            else if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) 
            {
                if(keyPressed->scancode == sf::Keyboard::Scancode::Escape) {window.close(); }
            }
        }
        
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            auto mousePos = sf::Mouse::getPosition(window);
            auto transMousePos = window.mapPixelToCoords(mousePos);

            //If the user clicks the play againBtn text, reset some data
            if(guiObject.againBtn.getGlobalBounds().contains(transMousePos))
            {

                Attributes::score = 0;
                Attributes::previousScore = 0;
                Attributes::missed = 0;
                Attributes::fireDelay = 0.25;
                Attributes::bossTimer = 6*60;
                Attributes::bossSpawned = false;
                Attributes::spawnAsteroidInterval = 2;

                boss.clear();
                targets.clear();
                projectiles.clear();
                drops.clear();

                playerObject.reset(Constants::width / 2, Constants::height / 2);

                currState = GameState::playing;

            }
        }

        if (currState == GameState::playing)
        {

            sf::Time timeElapsed = clock.getElapsedTime();
            dt = timeElapsed.asSeconds() * Constants::timeSpeed;
            Attributes::secSinceSpawn = Attributes::secSinceSpawn + dt;
            Attributes::secSinceFiring = Attributes::secSinceFiring - dt;
            Attributes::bossTimer = Attributes::bossTimer - dt;

            guiObject.setLevelsTxt(playerObject);

            if(static_cast<int>(Attributes::bossTimer)%60 < 10)
            {
                guiObject.timerTxt.setString("The lord is here in: " + std::to_string(static_cast<int>(Attributes::bossTimer)/60) + ":0" + std::to_string(static_cast<int>(Attributes::bossTimer)%60));
            }
            else
            {
                guiObject.timerTxt.setString("The lord is here in: " + std::to_string(static_cast<int>(Attributes::bossTimer)/60) + ":" + std::to_string(static_cast<int>(Attributes::bossTimer)%60));
            }

            playerObject.handleScreenWrapping();

            // player movement and shooting handling
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {playerObject.handleMovement(sf::Keyboard::Key::A, dt); }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {playerObject.handleMovement(sf::Keyboard::Key::D, dt); }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {playerObject.handleMovement(sf::Keyboard::Key::W, dt); }
            

            if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && playerObject.m_movementSpeed > 0)
            {
                playerObject.m_movementSpeed -= playerObject.m_deacceleration*dt/2;
                playerObject.movePlayer(dt);
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && Attributes::secSinceFiring <= 0)
            {
                projectiles.emplace_back(Projectile(playerObject.getGunRectXPos(), playerObject.getGunRectYPos(), playerObject.sprite.getRotation()));
                Attributes::secSinceFiring = Attributes::fireDelay;
            }

            guiObject.hudTxt.setString("Score: " + std::to_string(Attributes::score) + " Missed: " + std::to_string(Attributes::missed));

            Attributes::spawnAsteroidInterval = Attributes::spawnAsteroidInterval - Constants::difIncrease * dt;
            if(Attributes::secSinceSpawn > Attributes::spawnAsteroidInterval && Attributes::bossTimer > 0)
            {
                int ranDegree = Random::get(0, 360);
                targets.emplace_back(Constants::spawnRadius*std::sin(degToRad(ranDegree))+playerObject.sprite.getPosition().x, Constants::spawnRadius*std::cos(degToRad(ranDegree)) + playerObject.sprite.getPosition().y,100);

                targets.back().xMoveVector = (playerObject.sprite.getPosition().x - targets.back().shape.getPosition().x);
                targets.back().yMoveVector = (playerObject.sprite.getPosition().y - targets.back().shape.getPosition().y);

                Attributes::secSinceSpawn = 0;
            }

            if(Attributes::bossTimer < 0 && !Attributes::bossSpawned)
            {
                Attributes::bossSpawned = true;
                int ranDegree = Random::get(0,360);
                boss.emplace_back(Constants::spawnRadius*std::sin(degToRad(ranDegree))+playerObject.sprite.getPosition().x, Constants::spawnRadius*std::cos(degToRad(ranDegree)) + playerObject.sprite.getPosition().y,100);

            }

            if(Attributes::bossSpawned)
            {
                boss[0].moveBoss(dt, playerObject);

                for(auto& curr_projectile : projectiles)
                {
                    if(curr_projectile.shape.getGlobalBounds().findIntersection(boss[0].shape.getGlobalBounds()) && !curr_projectile.blickSum)
                    {
                        boss[0].health -= 1;
                        if(boss[0].health < 250) {boss[0].setStage(Boss::second); }
                        if(boss[0].health < 100) {boss[0].setStage(Boss::final); }
                        curr_projectile.blickSum = true;
                    }
                }

                // Collisions for boss
                if(boss[0].shape.getGlobalBounds().findIntersection(playerObject.sprite.getGlobalBounds()))
                {
                    if(playerObject.health - Constants::bossDamage <= 0)
                    {
                        currState = GameState::blewUp;
                        std::cout << "Kaboom\n";
                    }
                    playerObject.health -= Constants::bossDamage;
                }
            }

            for (auto& curr_projectile : projectiles) {curr_projectile.moveProjectile(dt, Constants::projectileSpeed); }
    
            // handle asteroid (target) interactions
            for(auto& curr_target : targets)
            {
                curr_target.moveTarget(dt);
                Attributes::missed += curr_target.targetExpiredOrNot();

                for(auto& curr_projectile : projectiles)
                {
                    if(!curr_projectile.blickSum && curr_projectile.shape.getGlobalBounds().findIntersection(curr_target.shape.getGlobalBounds()))
                    {
                        Attributes::score += 10;
                        curr_target.wasClicked = true;
                        if(Attributes::score % 100 == 0) {drops.emplace_back(curr_target.shape.getPosition().x, curr_target.shape.getPosition().y); }

                        if(curr_target.radius > 25)
                        {
                            targets.emplace_back(curr_target.shape.getPosition().x, curr_target.shape.getPosition().y, curr_target.radius/2);
                            targets.back().xMoveVector = (playerObject.sprite.getPosition().x - targets.back().shape.getPosition().x);
                            targets.back().yMoveVector = (playerObject.sprite.getPosition().y - targets.back().shape.getPosition().y);
                        }
                        curr_projectile.blickSum = true;
                    }
                }

                if(currState == GameState::playing)
                {
                    if(!curr_target.wasClicked && curr_target.shape.getGlobalBounds().findIntersection(playerObject.sprite.getGlobalBounds()) )
                    {
                        curr_target.wasClicked = true;
                        if(playerObject.health - Constants::astDamage <= 0) {currState = GameState::blewUp; }
                        playerObject.health -= Constants::astDamage;
                    }
                }
            }
            // handle drop interactions
            for(auto& curr_drop: drops)
            {
                if(!curr_drop.pickedUp && curr_drop.shape.getGlobalBounds().findIntersection(playerObject.sprite.getGlobalBounds()) )
                {
                    if(curr_drop.dropType == curr_drop.firerate)
                    {
                        Attributes::fireDelay -= 0.02f;
                        ++playerObject.firerateLvl;
                        curr_drop.pickedUp = true;
                    }
                    if(curr_drop.dropType == curr_drop.armor)
                    {
                        playerObject.health += Constants::astDamage;
                        curr_drop.pickedUp = true;
                    }
                    if(curr_drop.dropType == curr_drop.acceleration)
                    {
                        playerObject.m_acceleration += 2;
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
                if(curr_projectile.lifetime <= Constants::maxLifetime && !curr_projectile.blickSum) {window.draw(curr_projectile.shape); }
            }

            if (Attributes::bossTimer <= 0) {window.draw(boss[0].shape);}

            // projectiles and targets cleanup
            std::erase_if(projectiles, [](const Projectile& proj) {return proj.lifetime > Constants::maxProjectileLifetime; });
            std::erase_if(targets, [](const Target& target) {return target.wasClicked; });

            clock.restart();

            window.draw(playerObject.sprite);

            window.draw(guiObject.hudTxt);
            window.draw(guiObject.hpTxt);
            window.draw(guiObject.firerateTxt);
            window.draw(guiObject.accTxt);

            if(currState == GameState::blewUp)
            {
                window.draw(guiObject.ambatuTxt);
                window.draw(guiObject.againBtnTxt);
            }

            if(Attributes::bossTimer < 301 && Attributes::bossTimer > 0) {window.draw(guiObject.timerTxt);}

            window.display();
        }
    }
}
