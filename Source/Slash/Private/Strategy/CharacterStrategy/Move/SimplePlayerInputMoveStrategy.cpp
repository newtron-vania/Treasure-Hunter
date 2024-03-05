// Fill out your copyright notice in the Description page of Project Settings.


#include "Strategy/CharacterStrategy/Move/SimplePlayerInputMoveStrategy.h"

#include "Characters/BaseCharacter.h"
#include "Characters/SlashCharacter.h"

void USimplePlayerInputMoveStrategy::SetupInputBindings(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("MoveForward");
	PlayerInputComponent->BindAxis("MoveRight");
	PlayerInputComponent->BindAxis("TurnRight");
	PlayerInputComponent->BindAxis("LookUp");
}

void USimplePlayerInputMoveStrategy::ClearInputBindings(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->RemoveAxisBinding("MoveForward");
	PlayerInputComponent->RemoveAxisBinding("MoveRight");
	PlayerInputComponent->RemoveAxisBinding("TurnRight");
	PlayerInputComponent->RemoveAxisBinding("LookUp");
}

void USimplePlayerInputMoveStrategy::Move()
{
	UInputComponent* InputComponent = Character->InputComponent;
	
	if (InputComponent)
	{
		const float ForwardValue = InputComponent->GetAxisValue("MoveForward");
		const float RightValue = InputComponent->GetAxisValue("MoveRight");
		const float TurnRightValue = InputComponent->GetAxisValue("TurnRight");
		const float LookUpValue = InputComponent->GetAxisValue("LookUp");

		MoveForward(ForwardValue);
		MoveRight(RightValue);
		Turn(TurnRightValue);
		LookUp(LookUpValue);
	}
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