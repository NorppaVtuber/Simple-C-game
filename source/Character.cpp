#include "Character.h"
#include "raymath.h"
#include <iostream>

Character::Character(int winWidth, int winHeight, Texture2D tex, Vector2 startPos) :
    windowWidth(winWidth),
    windowHeight(winHeight)
{
    texture = tex;
    width = texture.width / maxFrames;
    height = texture.height / 10;
    worldPos = startPos;
}

Vector2 Character::GetScreenPos()
{
    return Vector2 {
        static_cast<float>(windowWidth)/2.0f - scale * (0.5f * width),
        static_cast<float>(windowHeight)/2.0f - scale * (0.5f * height)
    };
}

Vector2 Character::GetPosition(float tileSize)
{
    Vector2 worldPosition = CharacterBase::GetPosition(tileSize);

    float gridX = static_cast<int>(worldPosition.x + (GetScreenPos().x / tileSize) + (width / 2) / tileSize);
    float gridY = static_cast<int>(worldPosition.y + (GetScreenPos().y / tileSize) + (height / 2) / tileSize);

    if (gridX >= 24 || gridY >= 24)
        std::cout << "Error calculating character screen position!";

    std::cout << "Character position x: " << gridX << ", y: " << gridY;
    return Vector2{gridX, gridY};
}

void Character::TakeDamage(float damage)
{
    health -= damage;
    if(health <= 0.f)
    {
        SetAlive(false);
    }
}

void Character::Tick(float deltaTime, int newMaxFrames, int currentFrameLine)
{
    if(!GetAlive())
        return;
    if(IsKeyDown(KEY_A))
        velocity.x -= 1.0;
    if(IsKeyDown(KEY_D))
        velocity.x += 1.0;
    if(IsKeyDown(KEY_W))
        velocity.y -= 1.0;
    if(IsKeyDown(KEY_S))
        velocity.y += 1.0;
    
    if(isRunning)
    {
        currentFrameLine = 4;
    }
    else
    {
        currentFrameLine = 1;
    }
    CharacterBase::Tick(deltaTime, maxFrames, currentFrameLine);

    //weapon codes
    Vector2 origin = {};
    Vector2 offSet{};
    float rotation = 0.f;
    if(rightLeft > 0.f) //facing right
    {
        origin = {0.f, weapon.height * scale};
        offSet = {100.f, 55.f};
        weaponRec = {
            GetScreenPos().x + offSet.x,
            GetScreenPos().y + offSet.y - weapon.height * scale, 
            weapon.width * scale,
            weapon.height * scale
        };
        rotation = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? 35.f : 0.f;
    }
    else //facing left
    {
        origin = {weapon.width * scale, weapon.height * scale};
        offSet = {30.f, 55.f};
        weaponRec = {
            GetScreenPos().x + offSet.x - weapon.width * scale,
            GetScreenPos().y + offSet.y - weapon.height * scale, 
            weapon.width * scale,
            weapon.height * scale
        };
        rotation = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? -35.f : 0.f;
    }

    //draw the sword
    Rectangle source {0.f, 0.f, static_cast<float>(weapon.width) * rightLeft, static_cast<float>(weapon.height)};
    Rectangle dest {GetScreenPos().x + offSet.x, GetScreenPos().y + offSet.y, weapon.width * scale, weapon.height * scale};

    DrawTexturePro(weapon, source, dest, origin, rotation, WHITE);
}