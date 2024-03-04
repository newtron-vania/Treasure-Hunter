// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class ABaseCharacter;
class AEnemy;
class IEnemyStateInterface;
/**
 * 
 */
class SLASH_API BaseState
{
public:
	BaseState()
	{
		
	}
	virtual ~BaseState()
	{
		
	}

	virtual bool Enter() = 0;
	virtual bool Update() = 0;
	virtual bool Exit() = 0;
};
