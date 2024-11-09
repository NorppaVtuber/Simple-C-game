#include "newEnemy.h"
#include <iostream>

newEnemy::newEnemy(Texture2D idleTex, Texture2D runningTex, int enemyScale, Vector2 startPos)
{
	idleTexture = idleTex;
	runTexture = runningTex;
	activeTexture = idleTexture;
	scale = enemyScale;
	gridPos = startPos;
	worldPos = {};
	width = idleTexture.width / 4.f; //slimen idle on vain 4 framea, muista muuttaa width kun tex vaihtuu!
	height = idleTexture.height;
	frame = 0;
	maxFrames = 4;
	isRunning = false;
	
	currentState = &TheirTurn::getInstance(); // CurrentState on aina alussa Their (Player) Turn
	currentState->Enter(this);
}

void newEnemy::SetWorldPos()
{
	newCharacterBase::SetWorldPos();
	std::cout << "Enemy WorldPos Updated! New x: " << worldPos.x << ", y: " << worldPos.y;
	std::cout << "Enemy Current grid pos x: " << gridPos.x << ", y: " << gridPos.y;
}

void newEnemy::renderCharacter()
{
	if (isRunning)
	{
		activeTexture = runTexture;
		width = runTexture.width / 7.f;
		height = runTexture.height;
	}
	else
	{
		activeTexture = idleTexture;
		width = idleTexture.width / 4.f;
		height = idleTexture.height;
	}

	newCharacterBase::renderCharacter();
}

void newEnemy::Tick(float deltaTime)
{
	newCharacterBase::Tick(deltaTime);
	renderCharacter();
}

void newEnemy::MoveTowardsTreasure(std::vector<std::vector<int>>& map, Vector2 playerPos)
{
	if (getCurrentState() != &MyTurn::getInstance()) //varmistetaan, etta on MyTurn ennen liikkumista
		return;

	//kutsu a_star
	std::vector<Vector2> path = aStarSearch(gridPos, map, playerPos);

	// Jos aarre loytyi, liiku askel aarretta kohti
	if (!path.empty() && path.size() > 1) 
	{
		Vector2 nextStep = path[1]; // 0 = taman hetkinen tile, otetaan jonosta seuraava tile

		gridPos = nextStep;
		SetWorldPos();

		std::cout << "Enemy moved towards tresure!";
	}
	else 
	{
		std::cout << "No treasure found :(";
	}

	// tarkista, oletko arteen paalla
	if (map[(int)gridPos.y][(int)gridPos.x] == 2) 
	{
		std::cout << "Enemy found treasure!";
		AddTreasure();
		map[(int)gridPos.y][(int)gridPos.x] = 0; //poistetaan keratty aarre kartalta
	}

	//ihan lopussa vasta toggle
	Toggle();
}

void newEnemy::SetState(StateMachine& newState)
{
	currentState->Exit(this);
	currentState = &newState;
	currentState->Enter(this);
}

void newEnemy::Toggle()
{
	currentState->Toggle(this);
}