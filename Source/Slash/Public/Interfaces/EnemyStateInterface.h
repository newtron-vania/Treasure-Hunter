// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyStateInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyStateInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SLASH_API IEnemyStateInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Initialize() = 0;
	virtual void Enter() = 0;
	virtual void Update() = 0;
	virtual void Exit() = 0;
};

class IdleState : public IEnemyStateInterface
{
public:
	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
};

class PatrolState : public IEnemyStateInterface
{
public:
	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
};

class ChaseState : public IEnemyStateInterface
{
public:
	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
};

class AttackState : public IEnemyStateInterface
{
public:
	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
};

class EngageState : public IEnemyStateInterface
{
public:
	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
};