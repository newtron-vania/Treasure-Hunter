// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Strategy/CharacterStrategy/MoveStrategy.h"
#include "SimplePlayerInputMoveStrategy.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API USimplePlayerInputMoveStrategy : public UObject, public IMoveStrategy
{
	GENERATED_BODY()
	
	virtual void Initialize() override;
	virtual void Clear() override;

private:
	virtual void Move() override;
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
};
