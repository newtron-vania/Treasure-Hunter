// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStrategy.h"
#include "MoveStrategy.generated.h"

UINTERFACE(MinimalAPI)
class UMoveStrategy : public UCharacterStrategy {
	GENERATED_BODY()
};


class SLASH_API IMoveStrategy : public ICharacterStrategy
{
	GENERATED_BODY()
public:

	virtual void Execute() override ;
	virtual void SetupInputBindings(UInputComponent* PlayerInputComponent) = 0;
	virtual void ClearInputBindings(UInputComponent* PlayerInputComponent) = 0;
private:
	virtual void Move() = 0;
	
};
