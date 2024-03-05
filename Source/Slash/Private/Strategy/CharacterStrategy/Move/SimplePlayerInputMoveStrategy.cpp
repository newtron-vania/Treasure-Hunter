// Fill out your copyright notice in the Description page of Project Settings.


#include "Strategy/CharacterStrategy/Move/SimplePlayerInputMoveStrategy.h"

#include "Characters/BaseCharacter.h"
#include "Characters/SlashCharacter.h"

SimplePlayerInputMoveStrategy::SimplePlayerInputMoveStrategy()
{
}
SimplePlayerInputMoveStrategy::~SimplePlayerInputMoveStrategy()
{
}

void SimplePlayerInputMoveStrategy::SetCharacter(ABaseCharacter* NewCharacter)
{
	BaseMoveStrategy::SetCharacter(NewCharacter);
}

void SimplePlayerInputMoveStrategy::SetupInputBindings(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &SimplePlayerInputMoveStrategy::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &SimplePlayerInputMoveStrategy::MoveRight);
	PlayerInputComponent->BindAxis(FName("TurnRight"), this, &SimplePlayerInputMoveStrategy::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &SimplePlayerInputMoveStrategy::LookUp);
}

void SimplePlayerInputMoveStrategy::ClearInputBindings(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->RemoveAxisBinding(FName("MoveForward"));
	PlayerInputComponent->RemoveAxisBinding(FName("MoveRight"));
	PlayerInputComponent->RemoveAxisBinding(FName("TurnRight"));
	PlayerInputComponent->RemoveAxisBinding(FName("LookUp"));
}

void SimplePlayerInputMoveStrategy::Move()
{
}


void SimplePlayerInputMoveStrategy::MoveForward(float Value)
{
	if(Character->Controller && (Value != 0.f))
	{
		const FRotator ControlRotation = Character->GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		Character->AddMovementInput(Direction, Value);
	}
}

void SimplePlayerInputMoveStrategy::MoveRight(float Value)
{
	if(Character->Controller && (Value != 0.f))
	{
		//Find out which way is right
		const FRotator ControlRotation = Character->GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		Character->AddMovementInput(Direction, Value);
	}
}

void SimplePlayerInputMoveStrategy::Turn(float Value)
{
	Character->AddControllerYawInput(Value);
}

void SimplePlayerInputMoveStrategy::LookUp(float Value)
{
	Character->AddControllerPitchInput(Value);
}