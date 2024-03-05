// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStrategy.h"

/**
 * 
 */
class SLASH_API BaseMoveStrategy : public ICharacterStrategy
{
public:

	virtual void Execute() override ;
	virtual void SetupInputBindings(UInputComponent* PlayerInputComponent) = 0;
	virtual void ClearInputBindings(UInputComponent* PlayerInputComponent) = 0;

private:
	virtual void Move() = 0;
};
