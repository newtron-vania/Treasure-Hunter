// Fill out your copyright notice in the Description page of Project Settings.


#include "Strategy/CharacterStrategy/BaseHitReactStrategy.h"

#include "Characters/BaseCharacter.h"

BaseHitReactStrategy::BaseHitReactStrategy()
{
}

BaseHitReactStrategy::~BaseHitReactStrategy()
{
}

void BaseHitReactStrategy::Execute()
{
	Character->GetCharacterMovement();
}

