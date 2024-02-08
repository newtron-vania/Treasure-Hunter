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
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void Die() override;
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();
	virtual void Attack() override;
	virtual void PlayAttackMontage() override;
	virtual bool CanAttack() override;

	virtual void PlayHitSound(const FVector& ImpactPoint) override;
	virtual void SpawnHitParticle(const FVector& ImpactPoint) override;
	virtual void HandleDamage(float DamageAmount) override;
	
	bool IsAlive();

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	void CheckCombatTarget();
	void CheckPatrolTarget();

	UPROPERTY(BlueprintReadOnly)
	EDeathOrAlive EnemyAlive = EDeathOrAlive::EDA_Alive;
	
	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;


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
	void ClearPatrolTimer();

	UPROPERTY(EditAnywhere, Category="AI Navigation")
	float WaitMin = 5.f;
	
	UPROPERTY(EditAnywhere, Category="AI Navigation")
	float WaitMax = 10.f;

	/*
	 * AI behavior
	 */
	void UpdateHealthBarPercent();
	void SetHealthBarVisible(bool visible);
	void LooseInterest();
	void StartPatrolling();
	void ChaseTarget();
	bool IsOutsidedCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsDead();
	bool IsEngaged();
	/** Combat **/
	void StartAttackTimer();
	void ClearAttackTimer();

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category=Combat)
	float AttackMin = 0.5f;
	
	UPROPERTY(EditAnywhere, Category=Combat)
	float AttackMax = 1.f;

	UPROPERTY(EditAnywhere, Category=Combat)
	float PatrollingSpeed = 150.f;
	
	UPROPERTY(EditAnywhere, Category=Combat)
	float ChasingSpeed = 300.f;
public:
	void PlayDeathMontage();


};
