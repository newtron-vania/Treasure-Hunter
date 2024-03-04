// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseState.h"

class ABaseCharacter;
class AEnemy;
class IEnemyStateInterface;
/**
 * 
 */
class SLASH_API AttackState : public BaseState
{
public:
	AttackState();
	~AttackState() override;

	virtual bool Enter() override;
	virtual bool Update() override;
	virtual bool Exit() override;

	void Attack();
};
