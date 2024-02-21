// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "GameFramework/Character.h"
#include "TestAttacker.generated.h"

UCLASS()
class SLASH_API ATestAttacker : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATestAttacker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	float AttackDamage = 20;;

private:
	void FindClosestTarget();
	void PlayAttackAnimation();
	void CreateAttackSphere();
	void OnAttackSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void ExecuteGetHit(const FHitResult& BoxHit);

	bool IsAlly(ABaseCharacter* Target);

};
