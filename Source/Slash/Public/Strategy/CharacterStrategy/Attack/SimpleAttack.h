// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Strategy/CharacterStrategy/BaseAttackStrategy.h"

/**
 * 
 */
class SLASH_API SimpleAttack : public BaseAttackStrategy
{
public:

	virtual void Execute() override;
};
