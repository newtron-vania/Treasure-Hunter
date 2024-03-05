// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Strategy/CharacterStrategy/MoveStrategy.h"
#include "SimpleEnemyMoveStrategy.generated.h"
/**
 * 
 */
UCLASS()
class SLASH_API USimpleEnemyMoveStrategy : public UObject, public IMoveStrategy
{
	GENERATED_BODY()
public:
	virtual void SetupInputBindings(UInputComponent* PlayerInputComponent) override;
	virtual void ClearInputBindings(UInputComponent* PlayerInputComponent) override;

private:
	virtual void Move() override;
};
