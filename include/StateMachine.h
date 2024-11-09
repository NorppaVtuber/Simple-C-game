#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H
#include "newCharacterbase.h"

class newEnemy;

class StateMachine 
{
public:
	virtual void Enter(newEnemy* character) = 0;
	virtual void Toggle(newEnemy* character) = 0;
	virtual void Exit(newEnemy* character) = 0;
};

#endif