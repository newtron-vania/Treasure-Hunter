// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStrategy.h"

/**
 * 
 */
class SLASH_API BaseMoveStrategy : public CharacterStrategy
{
public:
	BaseMoveStrategy();
	~BaseMoveStrategy();

	virtual void Execute() override = 0;
};
