// Fill out your copyright notice in the Description page of Project Settings.


#include "Strategy/CharacterStrategy/Move/SimpleAIMoveStrategy.h"

#include "AIController.h"
#include "Characters/BaseCharacter.h"
#include "Perception/PawnSensingComponent.h"

void USimpleAIMoveStrategy::Initialize()
{
	if (!Character->FindComponentByClass<UPawnSensingComponent>())
	{
		PawnSensing = Character->CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	}
	
	PawnSensing-> SightRadius = 4000.f;
	PawnSensing-> SetPeripheralVisionAngle(45.f);
	
	CharacterController = Cast<AAIController>(Character->GetController());
}

void USimpleAIMoveStrategy::Clear()
{
}

void USimpleAIMoveStrategy::Move()
{
	MoveToTarget(Target);
}

void USimpleAIMoveStrategy::MoveToTarget(AActor* Target)
{
	if(CharacterController == nullptr || Target == nullptr) return;

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);
	CharacterController->MoveTo(MoveRequest);
}