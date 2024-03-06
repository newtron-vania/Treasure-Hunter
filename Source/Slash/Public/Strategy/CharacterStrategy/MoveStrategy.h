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
	virtual void Initialize() = 0;
	virtual void Clear() = 0;
private:
	virtual void Move() = 0;
	
};
