// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Strategy/CharacterStrategy/AttackStrategy.h"
#include "SimpleAttack.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API USimpleAttack : public UObject, public IAttackStrategy
{
	GENERATED_BODY()
public:

	virtual void Execute() override;
};
