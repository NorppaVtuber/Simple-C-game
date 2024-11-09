#include "Enemy.h"
#include "raymath.h"

Enemy::Enemy(Vector2 pos, Texture2D tex, int frames, int spriteAmount)
{
    worldPos = pos;
    texture = tex;
    frameLines = frames;
    maxFrames = spriteAmount;

    width = texture.width / maxFrames;
    height = texture.height / frameLines;
    speed = 3.5f;
}

void Enemy::Tick(float deltaTime, int newMaxFrames, int currentFrameLine)
{
    worldPosLastFrame = worldPos;

    if(!GetAlive())
        return;

    velocity = Vector2Subtract(target->GetScreenPos(), GetScreenPos());

    if(Vector2Length(velocity) < radius)
        velocity = {};

    if(isRunning)
    {
        maxFrames = 6;
        currentFrameLine = 7;
    }
    else
    {
        maxFrames = 3;
        currentFrameLine = 1;
    }

    CharacterBase::Tick(deltaTime, maxFrames, currentFrameLine);

    if(CheckCollisionRecs(target -> GetCollisionRec(), GetCollisionRec()))
    {
        target->TakeDamage(damagePerSec * deltaTime);
    }
}

Vector2 Enemy::GetScreenPos()
{
    return Vector2Subtract(worldPos, target->GetWorldPos());
}