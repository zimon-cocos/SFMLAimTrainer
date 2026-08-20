#pragma once
#include "Functions.h"
#include "ConstantsOrAttributes.h"

sf::Texture projectileSprite ("sprites/projectile.png");
sf::Texture shipTexture("sprites/ship_sheet.png");

sf::Texture background("sprites/pozadieMenej.png");
sf::Texture firerateSprite("sprites/boostFirer.png");
sf::Texture armorSprite("sprites/armor.png");
sf::Texture accelerationSprite("sprites/boostAcc.png");
sf::Texture errorSprite("sprites/theTriangle.png");

sf::Texture astTexS("sprites/asteroid_maly.png");
sf::Texture astTexM("sprites/asteroid_stredny.png");
sf::Texture astTexL("sprites/asteroid_velky.png");

sf::Texture bossL("sprites/BossL.png");
sf::Texture bossM("sprites/BossM.png");
sf::Texture bossS("sprites/BossS.png");

struct Drop
{
    enum DropType {firerate, armor, acceleration};

    sf::RectangleShape shape;
    sf::Sprite sprite {accelerationSprite};

    bool pickedUp {false};
    int dropType {Random::get(0, 2)};
    Drop(float xDrop, float yDrop)
    {
        dropType = Random::get(0,2);
        shape.setSize({40.0f,40.0f});
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(2.0f);
        shape.setPosition({xDrop,yDrop});
        sprite.setPosition({xDrop+5,yDrop+5});

        if(dropType == firerate) {sprite.setTexture(firerateSprite); }
        if(dropType == armor) {sprite.setTexture(armorSprite); }
        if(dropType == acceleration) {sprite.setTexture(accelerationSprite); }

    }
};

struct Player
{

    Player()
    {
        sprite.setPosition({300, 300});
        sprite.setOrigin({25.0f,40.0f});
        sprite.setTextureRect({ {0, 0}, {50, 80} });
        sprite.setScale({1.2,1.2});
    }

    void rotatePlayer(int degrees,float dt,int rotationSpeed) {sprite.rotate(dt*rotationSpeed*(sf::degrees(degrees))); }


    float getGunRectXPos() {return 40 * std::sin( -degToRad(sprite.getRotation().asDegrees() + 180) ) + sprite.getPosition().x; }
    float getGunRectYPos() {return 40 * std::cos( -degToRad(sprite.getRotation().asDegrees() + 180) ) + sprite.getPosition().y; }

    void handleScreenWrapping()
    {
        if (sprite.getPosition().x > width + 50)    {sprite.setPosition({-50.0f, sprite.getPosition().y});}
        if (sprite.getPosition().y > height + 50)   {sprite.setPosition({sprite.getPosition().x, -50.0});}
        if (sprite.getPosition().x < -50.0)         {sprite.setPosition({width + 50, sprite.getPosition().y});}
        if (sprite.getPosition().y < -50.0)         {sprite.setPosition({sprite.getPosition().x, height + 50});}      
    }

    void movePlayer(float dt, int movementSpeed)
    {
            sprite.move({movementSpeed*(40*std::sin(degToRad(-sprite.getRotation().asDegrees()+180))) * dt,
                         movementSpeed*(40*std::cos(degToRad(-sprite.getRotation().asDegrees()+180))) * dt});
    }
    
    void handleAnimationForward()
    {
        if(texTimer >= m_FRAME_DURATION)
        {
            texWidth += m_FRAME_WIDTH;
            if(texWidth >= shipTexture.getSize().x) {texWidth = 0; }

            sprite.setTextureRect({ {texWidth, 0} , {m_FRAME_WIDTH, m_FRAME_HEIGHT} });
            texTimer = 0;
        }
    }

    void reset(float startX, float startY)
    {
        blewUp = false;
        health = 100;
        firerateLvl = 1;
        accLvl = 1;
        sprite.setPosition({startX, startY});
        m_movementSpeed = 0;
        m_acceleration = 0;
    }

    

    bool blewUp {false};
    sf::Sprite sprite{shipTexture};

    int texWidth {0};
    float texTimer {0};

    int health {100};
    int accLvl {1};
    int firerateLvl {1};

    float m_movementSpeed {0};
    float m_acceleration {12};

    int m_FRAME_WIDTH {50};
    int m_FRAME_HEIGHT {80};
    float m_FRAME_DURATION {0.05};

};


struct Projectile
{
    sf::CircleShape shape;

    float lifetime {0};
    bool blickSum {false};

    Projectile(float xPos, float yPos, sf::Angle Rotation)
    {
        shape.setTexture(&projectileSprite);
        shape.setRadius(4.0f);
        shape.setOrigin(shape.getGeometricCenter());
        shape.setFillColor(sf::Color::Green);
        shape.setPosition({xPos, yPos});
        shape.setRotation(Rotation);
    }

    void moveProjectile(float dt, int movementSpeed)
    {
            shape.move({movementSpeed * (40 * std::sin(degToRad(-shape.getRotation().asDegrees() + 180))) * dt,
                        movementSpeed * (40 * std::cos(degToRad(-shape.getRotation().asDegrees() + 180))) * dt});
    }

};

struct Target
{
    sf::CircleShape shape;

    float xMoveVector {0};
    float yMoveVector {0};
    float radius {100};

    float secondsExisted {0};
    bool wasClicked {false};

    Target(float x = 0.0, float y = 0.0, float radiusInput = 100.0,
            const sf::Texture* astTexParamS = &astTexS,
            const sf::Texture* astTexParamM = &astTexM,
            const sf::Texture* astTexParamL = &astTexL
            )
    {
        if(radiusInput == 100)
        {
            shape.setTexture(astTexParamL);
            shape.setTextureRect(sf::IntRect({0,0}, {200,200}));
        }
        else if(radiusInput == 50)
        {
            shape.setTexture(astTexParamM);
            shape.setTextureRect(sf::IntRect({0,0}, {100,100}));
        }
        else
        {
            shape.setTexture(astTexParamS);
            shape.setTextureRect(sf::IntRect({0,0}, {50,50}));
        }
        radius = radiusInput;
        shape.setPosition({x, y});
        shape.setRadius(radius);
        shape.setOrigin(shape.getGeometricCenter());
    }



    void moveTarget(float dt) {shape.move({xMoveVector * dt * targSpeed, yMoveVector * dt * targSpeed}); }

    int targetExpiredOrNot() // has to be int, the return value is added to missed asteroids count
    {
        if ((secondsExisted > maxTargetLifetime) && !(wasClicked))
        {
            wasClicked = true;
            return 1;
        }
        else return 0;
    }
};

struct Boss : public Target
{

    float xTarget {0};
    float yTarget {0};

    float health {500};
    Boss(float x, float y, float radiusInput)
    : Target(x, y, radiusInput, &bossS, &bossM, &bossL),
    xTarget {x},
    yTarget {y}
    {
    }
    
    void moveBoss(float dt, const Player& playerObject)
    {
        xMoveVector = 5*(playerObject.sprite.getPosition().x - xTarget);
        yMoveVector = 5*(playerObject.sprite.getPosition().y - yTarget);
        
        shape.move({xMoveVector*dt*bossSpeed,yMoveVector*dt*bossSpeed});
        xTarget = xTarget + xMoveVector*dt*bossSpeed;
        yTarget = yTarget + yMoveVector*dt*bossSpeed;
    }
};

struct GUI
{

    sf::Sprite backgSprite{background};
    sf::Font font{"fonts/Jersey_15/Jersey15-Regular.ttf"};
    sf::Text hudTxt{font};
    sf::Text timerTxt{font};  
    sf::Text hpTxt{font};
    sf::Text accTxt{font}; 
    sf::Text firerateTxt{font};
    sf::Text ambatuTxt{font};   

    sf::Text againBtnTxt{font};       
    sf::RectangleShape againBtn;   

    GUI()
    {
        backgSprite.setPosition({0,0});

        hudTxt.setString("It works");
        hudTxt.setCharacterSize(24);
        hudTxt.setFillColor(sf::Color::Red);
        hudTxt.setPosition({width/2 - 80,30});

        timerTxt.setString("placeholder");
        timerTxt.setCharacterSize(30);
        timerTxt.setFillColor(sf::Color::Red);
        timerTxt.setPosition({width/2-120,60});    
        
        hpTxt.setString("placeholder");
        hpTxt.setCharacterSize(24);
        hpTxt.setFillColor(sf::Color::Red);
        hpTxt.setPosition({675,30});
      
        accTxt.setString("placeholder");
        accTxt.setCharacterSize(24);
        accTxt.setFillColor(sf::Color::Red);
        accTxt.setPosition({675,50});


        firerateTxt.setString("placeholder");
        firerateTxt.setCharacterSize(24);
        firerateTxt.setFillColor(sf::Color::Red);
        firerateTxt.setPosition({675,70});


        ambatuTxt.setString("You blew up!");
        ambatuTxt.setCharacterSize(48);
        ambatuTxt.setFillColor(sf::Color::Magenta);
        ambatuTxt.setPosition({width/2-100,height/2-100});


        againBtn.setSize({290.0f,40.0f});
        againBtn.setPosition({width/2-100-30,height/2-100 + 72});
        againBtn.setFillColor(sf::Color::Transparent);
        againBtn.setOutlineThickness(2.0f);
        againBtn.setOutlineColor(sf::Color::Red);


        againBtnTxt.setString("Play again?");
        againBtnTxt.setCharacterSize(72);
        againBtnTxt.setFillColor(sf::Color::Magenta);
        againBtnTxt.setPosition({width/2-100-30,height/2-100 +40});
    }

    void setLevelsTxt(Player& playerObject)
    {
        hpTxt.setString("Hull integrity: " + std::to_string(playerObject.health) + "%");
        accTxt.setString("Aceleration level: " + std::to_string(playerObject.accLvl));
        firerateTxt.setString("Firerate level: " + std::to_string(playerObject.firerateLvl));    
    }



};
