#ifndef CHARACTER_BASE_H
#define CHARACTER_BASE_H
#include "raylib.h"

class CharacterBase
{
    public:
        CharacterBase();
        Vector2 GetWorldPos() { return worldPos; }
        virtual Vector2 GetPosition(float tileSize);
        void UndoMovement();
        Rectangle GetCollisionRec();
        virtual void Tick(float deltaTime, int newMaxFrames, int currentFrameLine);
        virtual void AnimateCharacter (int maxFrames, int frameLine);
        virtual Vector2 GetScreenPos() = 0;
        bool GetAlive() {return alive;}
        void SetAlive(bool isAlive) {alive = isAlive;}
        Vector2 GetSize();
    protected:
        float width = 0.f;
        float height = 0.f;
        float scale = 4.f;
        Texture2D texture {LoadTexture("textures/characters/slime.png")};
        Vector2 worldPos = {};
        Vector2 worldPosLastFrame = {};
        float rightLeft = 1.f; //1 = going to right, -1 = going to left
        Vector2 velocity = {};

        //anim variables
        int frameLines = 10;
        float runningTime = 0.f;
        int frame = 0;
        int maxFrames = 6;
        float updateTime = 1.f/12.f;
        bool isRunning = false;
        float animationLine = 0.f;
        float speed = 4.f;
    private:
        bool alive = true;
};

#endif