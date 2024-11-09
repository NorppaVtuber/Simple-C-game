#include "newCharacterbase.h"
#include <iostream>

newCharacterBase::newCharacterBase()
{

}

void newCharacterBase::SetWorldPos() //worldPos on vain renderointia varten
{
	int tileSize = 16 * scale; //tilen koko 16x16 px, skaalataan scalella
	worldPos = {
		gridPos.x * tileSize,
		gridPos.y * tileSize
	};
	std::cout << "newCharacterBase SetWorldPos updated!";
}

void newCharacterBase::MoveCharacter(Vector2 direction)
{
	Vector2 nextPos = Vector2Add(gridPos, direction);
	gridPos = nextPos;

	SetWorldPos();
}

void newCharacterBase::UnloadTextures()
{
	UnloadTexture(idleTexture);
	UnloadTexture(runTexture);
}

void newCharacterBase::renderCharacter() 
{
	Rectangle source{ frame * width, 0.f, rightLeft * width, height };
	Rectangle dest{ worldPos.x, worldPos.y, scale * width, scale * height };

	DrawTexturePro(activeTexture, source, dest, Vector2{}, 0.f, WHITE);
}

void newCharacterBase::Tick(float deltaTime)
{
	runningTime += deltaTime;
	//std::cout << "CharacterBase deltaTime: " << deltaTime << ", update time: " << updateTime << ", runningTime: " << runningTime;
	if (runningTime >= updateTime)
	{
		frame++;
		runningTime = 0.f;
		if (frame >= maxFrames)
			frame = 0;
		//std::cout << "Current frame: " << frame;
	}
}