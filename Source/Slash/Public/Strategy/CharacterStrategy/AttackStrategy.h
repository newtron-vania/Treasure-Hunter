// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStrategy.h"
#include "AttackStrategy.generated.h"

UINTERFACE(MinimalAPI)
class UAttackStrategy : public UCharacterStrategy {
	GENERATED_BODY()
};

/**
 * 
 */
class SLASH_API IAttackStrategy : public ICharacterStrategy
{
	GENERATED_BODY()
public:

	virtual void Execute() override;
};
