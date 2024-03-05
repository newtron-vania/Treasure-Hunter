// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterStrategy.generated.h"

class ABaseCharacter;
/**
 * 
 */

UINTERFACE(MinimalAPI, Blueprintable)
class UCharacterStrategy : public UInterface {
	GENERATED_BODY()
};

class SLASH_API ICharacterStrategy
{
	GENERATED_BODY()
	
public:
	virtual void Execute() = 0;
	virtual void SetCharacter(ABaseCharacter* NewCharacter);
	virtual void ClearCharacter();

	ABaseCharacter* Character;
	
};
