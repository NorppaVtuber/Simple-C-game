#include "raylib.h"
#include "CharacterBase.h"
#include "Character.h"

class Enemy : public CharacterBase
{
    public:
        Enemy(Vector2 pos, Texture2D tex, int frameLines, int spriteAmount);
        virtual void Tick(float deltaTime, int newMaxFrames, int currentFrameLine) override;
        void SetTarget(Character* character) { target = character; };
        virtual Vector2 GetScreenPos() override;
    private:
        Character* target;
        float damagePerSec = 10.f;
        float radius = 25.f;
};