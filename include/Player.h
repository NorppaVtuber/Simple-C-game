#ifndef PLAYER_H
#define PLAYER_H
#include "raylib.h"
#include "raymath.h"
#include "newCharacterbase.h"

class Player : public newCharacterBase
{
public:
	Player(Texture2D idleTex, Texture2D runTex, int playerScale, Vector2 startPos);
	void SetWorldPos() override;
	void Tick(float deltaTime) override;
private:
	void renderCharacter() override;
};

#endif