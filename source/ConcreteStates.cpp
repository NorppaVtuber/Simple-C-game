#include "ConcreteStates.h"
#include <iostream>

void MyTurn::Enter(newEnemy* character)
{
	std::cout << "Enemy enter's MyTurn!";
}

void MyTurn::Toggle(newEnemy* character)
{
	std::cout << "Enemy MyTurn Toggled!";
	//my turn -> their turn
	character->SetState(TheirTurn::getInstance());
}

void MyTurn::Exit(newEnemy* character)
{
	std::cout << "Enemy exit's MyTurn!";
}
StateMachine& MyTurn::getInstance()
{
	static MyTurn singleton;
	return singleton;
}

void TheirTurn::Enter(newEnemy* character)
{
	std::cout << "Enemy enter's TheirTurn!";
}

void TheirTurn::Toggle(newEnemy* character)
{
	std::cout << "Enemy TheirTurn Toggled!";
	//their turn -> my turn
	character->SetState(MyTurn::getInstance());
}

void TheirTurn::Exit(newEnemy* character)
{

	std::cout << "Enemy exit's TheirTurn!";
}

StateMachine& TheirTurn::getInstance()
{
	static TheirTurn singleton;
	return singleton;
}