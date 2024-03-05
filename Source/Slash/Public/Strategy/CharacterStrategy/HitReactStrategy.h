// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStrategy.h"
#include "HitReactStrategy.generated.h"

UINTERFACE(MinimalAPI)
class UHitReactStrategy : public UCharacterStrategy {
	GENERATED_BODY()
};

/**
 * 
 */
class SLASH_API IHitReactStrategy : public ICharacterStrategy
{
	GENERATED_BODY()
public:

	virtual void Execute() override;
};
