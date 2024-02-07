// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashAnimInstance.h"
#include "Characters/SlashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void USlashAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	bISInAir = false;
	
	SlashCharacter = Cast<ASlashCharacter>(TryGetPawnOwner());
	if(SlashCharacter)
	{
		SlashCharacterMovement = SlashCharacter->GetCharacterMovement();
	}
}

void USlashAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(SlashCharacterMovement == nullptr)
	{
		return;
	}

	FVector direction = SlashCharacterMovement->Velocity;
	float distanceXY = UKismetMathLibrary::VSizeXY(direction);
	GroundSpeed = distanceXY;

	float distanceZ = direction.Z;

	bISInAir = SlashCharacterMovement->IsFalling();

	//Set CharacterState
	CharacterState = SlashCharacter->GetCharacterState();
	
}
