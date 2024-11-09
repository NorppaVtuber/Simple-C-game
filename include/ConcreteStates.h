#ifndef CONCRETE_STATES_H
#define CONCRETE_STATES_H

#include "StateMachine.h"
#include "newEnemy.h"

class MyTurn : public StateMachine
{
public:
	void Enter(newEnemy* character) override;
	void Toggle(newEnemy* character) override;
	void Exit(newEnemy* character) override;
	static StateMachine& getInstance();
private:
	MyTurn() {}
	MyTurn(const MyTurn& other);
	MyTurn& operator=(const MyTurn& other);
};

class TheirTurn : public StateMachine
{
public:
	void Enter(newEnemy* character) override;
	void Toggle(newEnemy* character) override;
	void Exit(newEnemy* character) override;
	static StateMachine& getInstance();
private:
	TheirTurn() {}
	TheirTurn(const TheirTurn& other);
	TheirTurn& operator=(const TheirTurn& other);
};

#endif