#include "CharacterBase.h"
#include "raymath.h"
#include <iostream>

CharacterBase::CharacterBase()
{
    
}

void CharacterBase::UndoMovement()
{
    worldPos = worldPosLastFrame;
}

Rectangle CharacterBase::GetCollisionRec() //missa hahmo on ruudulla
{
    return Rectangle{
        GetScreenPos().x,
        GetScreenPos().y,
        width * scale,
        height * scale
    };
}

Vector2 CharacterBase::GetPosition(float tileSize)
{
    //ota ensin worldPos
    //laske sen perusteella missa pain karttaa hahmo on
    //palauta positio 1:n perustuvana koordinaattina

    int x = 0;
    int y = 0;

    x = static_cast<int>(worldPos.x / tileSize);
    y = static_cast<int>(worldPos.y / tileSize);

    if (x > 24 || y > 24)
    {
        std::cout << "Position calculation error!!!";
    }

    //std::cout << "Character position x: " << x << ", y: " << y;
    return Vector2{ static_cast<float>(x), static_cast<float>(y)};
}

void CharacterBase::AnimateCharacter(int newMaxFrames, int currentFrameLine)
{
    //Draw the character
    if(isRunning)
        animationLine = currentFrameLine * static_cast<float>(texture.height) / frameLines;
    else
        animationLine = currentFrameLine * static_cast<float>(texture.height) / frameLines;
    
    maxFrames = newMaxFrames;

    Rectangle source{frame * width, animationLine, rightLeft * width, height};
    Rectangle dest{GetScreenPos().x, GetScreenPos().y, scale * width, scale * height};

    DrawTexturePro(texture, source, dest, Vector2{}, 0.0f, WHITE);
}

Vector2 CharacterBase::GetSize()
{
    return Vector2{ 1, 1 }; //sovitaan pelihahmon koon olevan 1
}

void CharacterBase::Tick(float deltaTime, int newMaxFrames, int currentFrameLine)
{
    worldPosLastFrame = worldPos;

    //update animation
    runningTime += deltaTime;
    if(runningTime >= updateTime)
    {
        frame++;
        runningTime = 0.f;
        if(frame > maxFrames)
            frame = 0;
    }

    if(Vector2Length(velocity) != 0.0)
    {
        //set worldPos = worldPos + direction
        worldPos = Vector2Add(worldPos, Vector2Scale(Vector2Normalize(velocity), speed));
        velocity.x < 0.f ? rightLeft = -1.f : rightLeft = 1.f;
        isRunning = true;
    }
    else
    {
        isRunning = false;
    }
    velocity = {};

    AnimateCharacter(newMaxFrames, currentFrameLine);
}