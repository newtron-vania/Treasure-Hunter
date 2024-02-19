// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterTypes.h"

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

	bool RegisterState(IEnemyStateInterface* EnemyStateInterface);
	bool ChangeState(EEnemyState EnemyState);

private:
	TMap<EEnemyState, IEnemyStateInterface*> RegisteredStates;
	
	EEnemyState CurrentState;

	APawn* StateActor;

	APawn* PatrolTarget;

	APawn* CombatTarget;
	
	


	
};
