#pragma once
#include "Functions.h"
#include "ConstantsOrAttributes.h"

sf::Texture projectile ("sprites/projectile.png");
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

        if(dropType == 0) {sprite.setTexture(firerateSprite); }
        if(dropType == 1) {sprite.setTexture(armorSprite); }
        if(dropType == 2) {sprite.setTexture(accelerationSprite); }


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

    void rotatePlayer(int degrees,float dt,int rotationSpeed)
    {
        sprite.rotate(dt*rotationSpeed*(sf::degrees(degrees)));
    }


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
            sprite.move({movementSpeed*(40*std::sin(degToRad(-sprite.getRotation().asDegrees()+180)) + sprite.getPosition().x - sprite.getPosition().x) * dt,
                         movementSpeed*(40*std::cos(degToRad(-sprite.getRotation().asDegrees()+180)) + sprite.getPosition().y - sprite.getPosition().y) * dt});
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
    float m_acceleration {0};

    int m_FRAME_WIDTH {50};
    int m_FRAME_HEIGHT {80};
    double m_FRAME_DURATION {0.05};

};


struct Projectile
{
    sf::CircleShape shape;
    float xProjectile {0};
    float yProjectile {0};
    float lifetime {0};
    bool blickSum {false};

    Projectile(float xPos, float yPos, sf::Angle Rotation)
    {
        shape.setTexture(&projectile);
        shape.setRadius(4.0f);
        shape.setOrigin(shape.getGeometricCenter());
        shape.setFillColor(sf::Color::Green);
        shape.setPosition({xPos,yPos});
        shape.setRotation(Rotation);
        xProjectile = xPos;
        yProjectile = yPos;


    }
    void moveProjectile(float dt, int movementSpeed)
    {
            shape.move({movementSpeed*(40*std::sin(degToRad(-shape.getRotation().asDegrees()+180))+xProjectile - xProjectile)*dt,
                        movementSpeed*(40*std::cos(degToRad(-shape.getRotation().asDegrees()+180))+yProjectile - yProjectile)*dt});
            xProjectile = shape.getPosition().x;
            yProjectile = shape.getPosition().y;
    }

};


struct Boss
{
    sf::CircleShape shape;

    float xTarget {0};
    float yTarget {0};
    float xMoveVector {0};
    float yMoveVector {0};
    float radius {100};
    float health {500};
    Boss(float x, float y,float radiusInput)
    {
        if(radiusInput == 100)
        {
            shape.setTexture(&bossL);
            shape.setTextureRect(sf::IntRect({0,0},{200,200}));
        }
        else if(radiusInput == 50)
        {
            shape.setTexture(&bossM);
            shape.setTextureRect(sf::IntRect({0,0},{100,100}));
        }
        else
        {
            shape.setTexture(&bossS);
            shape.setTextureRect(sf::IntRect({0,0},{50,50}));
        }
        radius = radiusInput;
        shape.setPosition({x,y});
        shape.setRadius(radius);
        shape.setOrigin(shape.getGeometricCenter());
        xTarget = x;
        yTarget = y;
    }

    float secondsExisted {0};
    bool wasClicked {false};


    void moveBoss(float dt, const Player& playerObject)
    {
        xMoveVector = 5*(playerObject.sprite.getPosition().x - xTarget);
        yMoveVector = 5*(playerObject.sprite.getPosition().y - yTarget);

        shape.move({xMoveVector*dt*bossSpeed,yMoveVector*dt*bossSpeed});
        xTarget = xTarget + xMoveVector*dt*bossSpeed;
        yTarget = yTarget + yMoveVector*dt*bossSpeed;
    }
};


struct Target
{
    sf::CircleShape shape;

    float xTarget {0};
    float yTarget {0};
    float xMoveVector {0};
    float yMoveVector {0};
    float radius {100};

    Target(float x, float y,float radiusInput)
    {
        if(radiusInput == 100)
        {
            shape.setTexture(&astTexL);
            shape.setTextureRect(sf::IntRect({0,0},{200,200}));
        }
        else if(radiusInput == 50)
        {
            shape.setTexture(&astTexM);
            shape.setTextureRect(sf::IntRect({0,0},{100,100}));
        }
        else
        {
            shape.setTexture(&astTexS);
            shape.setTextureRect(sf::IntRect({0,0},{50,50}));
        }
        radius = radiusInput;
        shape.setPosition({x,y});
        shape.setRadius(radius);
        shape.setOrigin(shape.getGeometricCenter());
        xTarget = x;
        yTarget = y;
    }

    float secondsExisted {0};
    bool wasClicked {false};


    void moveTarget(float dt)
    {
        shape.move({xMoveVector*dt*targSpeed,yMoveVector*dt*targSpeed});
        xTarget = xTarget + xMoveVector*dt*targSpeed;
        yTarget = yTarget + yMoveVector*dt*targSpeed;
    }
    int targetExpiredOrNot()
    {
        if ((secondsExisted > maxTargetLifetime) && !(wasClicked))
        {
            wasClicked = true;
            return 1;
        }
        else return 0;
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


        againBtnTxt.setString("Play againBtn?");
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
