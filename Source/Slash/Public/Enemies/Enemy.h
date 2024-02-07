// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterTypes.h"
#include "Characters/BaseCharacter.h"
#include "Enemy.generated.h"

class UPawnSensingComponent;
class AAIController;
class UHealthBarComponent;
class UAnimMontage;
class USoundBase;
class UAttributeComponent;

UCLASS()
class SLASH_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	void PlayHitSound(const FVector& ImpactPoint);
	void PlayHitParticle(const FVector& ImpactPoint);
	void SetHealthBarPercent();
	void SetHealthBarVisible(bool visible);



protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void Die() override;
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);
	
	void CheckCombatTarget();
	void CheckPatrolTarget();

	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;

private:
	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY(EditDefaultsOnly)
	double CombatRadius = 500.f;

	UPROPERTY(EditDefaultsOnly)
	double AttackRadius = 150.f;

	/*
	 * Navigation
	 */
	UPROPERTY()
	AAIController* EnemyController;

	// 현재 탐색 상대
	UPROPERTY(EditInstanceOnly, Category="AI Navigation")
	AActor* PatrolTarget;
	
	UPROPERTY(EditInstanceOnly, Category="AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditDefaultsOnly)
	double PatrolRadius = 200.f;

	FTimerHandle PatrolTimer;
	void PatrolTimerFinished();

	UPROPERTY(EditAnywhere, Category="AI Navigation")
	float WaitMin = 5.f;
	
	UPROPERTY(EditAnywhere, Category="AI Navigation")
	float WaitMax = 10.f;

	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

public:
	void PlayDeathMontage();


};
