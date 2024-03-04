// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SLASH_API CharacterStrategy
{
public:
	CharacterStrategy();
	~CharacterStrategy();

	virtual void Execute() = 0;
};
