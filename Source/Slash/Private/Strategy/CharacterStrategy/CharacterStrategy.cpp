// Fill out your copyright notice in the Description page of Project Settings.


#include "Strategy/CharacterStrategy/CharacterStrategy.h"


void ICharacterStrategy::SetCharacter(ABaseCharacter* NewCharacter)
{
	if(NewCharacter)
	{
		Character = NewCharacter;
	}
	
}

void ICharacterStrategy::ClearCharacter()
{
	Character = nullptr;
}
