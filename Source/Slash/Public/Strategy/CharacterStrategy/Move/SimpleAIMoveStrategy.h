// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Strategy/CharacterStrategy/MoveStrategy.h"
#include "SimpleAIMoveStrategy.generated.h"

class UPawnSensingComponent;
class AAIController;
/**
 * 
 */
UCLASS()
class SLASH_API USimpleAIMoveStrategy : public UObject, public IMoveStrategy
{
	GENERATED_BODY()
public:
	virtual void Initialize() override;
	virtual void Clear() override;

private:
	virtual void Move() override;

	void MoveToTarget(AActor* Target);

	AAIController* CharacterController;

	UPawnSensingComponent* PawnSensing;

	APawn* Target;

	float MoveSpeed;

	float AcceptanceRadius;
};
