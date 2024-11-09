#ifndef CHARACTER_H
#define CHARACTER_H
#include "raylib.h"
#include "CharacterBase.h"

class Character : public CharacterBase
{
    public:
        Character(int windowWidth, int windowHeight, Texture2D texture, Vector2 startPos);
        virtual void Tick(float deltaTime, int newMaxFrames, int currentFrameLine) override;
        virtual Vector2 GetScreenPos() override;
        virtual Vector2 GetPosition(float tileSize) override;
        Rectangle GetWeaponCollisionRec() const { return weaponRec; }
        float GetHealth() const { return health; }
        void TakeDamage(float damage);
    private:
        int windowWidth = 0;
        int windowHeight = 0;
        Texture2D weapon = {LoadTexture("textures/objects/sword.png")};
        Rectangle weaponRec = {};
        float health = 100.f;
};
#endif