#ifndef NEW_ENEMY_H
#define NEW_ENEMY_H

#include "raylib.h"
#include "raymath.h"
#include "newCharacterbase.h"
#include "a_star.h"
#include "StateMachine.h"
#include "ConcreteStates.h"

class newEnemy : public newCharacterBase
{
public:
	newEnemy(Texture2D idleTex, Texture2D runningTex, int enemyScale, Vector2 startPos);
	void SetWorldPos() override;
	void Tick(float deltaTime) override;
	void MoveTowardsTreasure(std::vector<std::vector<int>>& map, Vector2 playerPos);

	//FSM stuffs
	inline StateMachine* getCurrentState() const { return currentState; }
	void Toggle();
	void SetState(StateMachine& newState);
private:
	void renderCharacter() override;

	StateMachine* currentState;
};

#endif