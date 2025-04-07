#pragma once
#include "Functions.h"
#include "ConstantsOrAttributes.h"

sf::Texture shipTexture("sprites/ship_sheet.png");
sf::Texture astTexS("sprites/asteroid_maly.png");
sf::Texture astTexM("sprites/asteroid_stredny.png");
sf::Texture astTexL("sprites/asteroid_velky.png");


struct Player
{
    float xPlayer {0};
    float yPlayer {0};
    bool blewUp {0};
    sf::Sprite sprite{shipTexture};
    int texWidth {0};
    float texTimer {0};
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
        shape.setRadius(4.0f);
        shape.setOrigin(shape.getGeometricCenter());
        shape.setFillColor(sf::Color::Green);
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

    float xTarget {0};
    float yTarget {0};
    float xMoveVector {0};
    float yMoveVector {0};
    float targSpeed {0.5};
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
};
