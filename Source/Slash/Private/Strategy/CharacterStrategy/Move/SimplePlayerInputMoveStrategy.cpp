// Fill out your copyright notice in the Description page of Project Settings.


#include "Strategy/CharacterStrategy/Move/SimplePlayerInputMoveStrategy.h"

#include "Characters/BaseCharacter.h"


void USimplePlayerInputMoveStrategy::SetupInputBindings(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &USimplePlayerInputMoveStrategy::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &USimplePlayerInputMoveStrategy::MoveRight);
	PlayerInputComponent->BindAxis(FName("TurnRight"), this, &USimplePlayerInputMoveStrategy::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &USimplePlayerInputMoveStrategy::LookUp);
}

void USimplePlayerInputMoveStrategy::ClearInputBindings(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->RemoveAxisBinding(FName("MoveForward"));
	PlayerInputComponent->RemoveAxisBinding(FName("MoveRight"));
	PlayerInputComponent->RemoveAxisBinding(FName("TurnRight"));
	PlayerInputComponent->RemoveAxisBinding(FName("LookUp"));
}

void USimplePlayerInputMoveStrategy::Move()
{
}


void USimplePlayerInputMoveStrategy::MoveForward(float Value)
{
	if(Character->Controller && (Value != 0.f))
	{
		const FRotator ControlRotation = Character->GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		Character->AddMovementInput(Direction, Value);
	}
}

void USimplePlayerInputMoveStrategy::MoveRight(float Value)
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

void USimplePlayerInputMoveStrategy::Turn(float Value)
{
	Character->AddControllerYawInput(Value);
}

void USimplePlayerInputMoveStrategy::LookUp(float Value)
{
	Character->AddControllerPitchInput(Value);
}