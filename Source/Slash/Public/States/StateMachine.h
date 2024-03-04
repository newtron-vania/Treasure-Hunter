// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class BaseState;
class ABaseCharacter;
class AEnemy;
class IEnemyStateInterface;
/**
 * 
 */
class SLASH_API StateMachine
{
public:
	StateMachine();
	~StateMachine();

	bool RegisterState(BaseState* State);
	bool Update();

private:
	TUniquePtr<BaseState> CurrentState;

	ABaseCharacter* BasedCharacter;
};
