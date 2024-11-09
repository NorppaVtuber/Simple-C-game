#include "Player.h"
#include <iostream>

Player::Player(Texture2D idleTex, Texture2D runTex, int playerScale, Vector2 startPos)
{
	idleTexture = idleTex;
	runTexture = runTex;
	scale = playerScale;
	gridPos = startPos;
	worldPos = {};
	width = idleTexture.width / 6.f;
	height = idleTexture.height;
	activeTexture = idleTexture;
	frame = 0;
	maxFrames = 6;
	isRunning = false;
}

void Player::SetWorldPos()
{
	newCharacterBase::SetWorldPos();
	std::cout << "Player WorldPos Updated! New x: " << worldPos.x << ", y: " << worldPos.y;
	std::cout << "Current grid pos x: " << gridPos.x << ", y: " << gridPos.y;
}

void Player::Tick(float deltaTime)
{
	newCharacterBase::Tick(deltaTime);
	renderCharacter();
}

void Player::renderCharacter()
{
	if (isRunning)
	{
		activeTexture = runTexture;
		width = runTexture.width / 6.f;
		height = runTexture.height;
	}
	else
	{
		activeTexture = idleTexture;
		width = idleTexture.width / 6.f;
		height = idleTexture.height;
	}

	newCharacterBase::renderCharacter();
}