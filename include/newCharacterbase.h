#ifndef NEW_CHARACTER_BASE_H
#define NEW_CHARACTER_BASE_H
#include "raylib.h"
#include "raymath.h"

class StateMachine;

class newCharacterBase 
{
public:
	newCharacterBase();
	Vector2 GetWorldPos() { return worldPos; }
	Vector2 GetGridPos() { return gridPos; }
	virtual void SetWorldPos();
	void MoveCharacter(Vector2 direction);
	void UnloadTextures();
	Vector2 GetSize() { return Vector2{ 1, 1 }; }
	virtual void Tick(float deltaTime);
	int GetTreasure() { return collectedTreasure; }
	void AddTreasure() { collectedTreasure++; }

	
protected:
	Vector2 worldPos;
	Vector2 gridPos;
	Vector2 previousGridPos;
	int scale;
	Texture2D idleTexture;
	Texture2D runTexture;
	Texture2D activeTexture;

	virtual void renderCharacter();

	float width;
	float height;
	int frame = 0;
	int maxFrames = 6;
	float updateTime = 1.f / 12.f;
	float runningTime = 0.f;
	float rightLeft = 1.f;
	bool isRunning;

	int collectedTreasure = 0;
};

#endif