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

	/** <AActor> **/
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
	/** </AActor> **/

	/** <IHitInterface> **/
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/** </IHitInterface> **/
	
	protected:
	/** <AActor> **/
	virtual void BeginPlay() override;
	/** </AActor> **/

	/** <ABaseCharacter> */
	virtual void Die() override;
	virtual void Attack() override;
	virtual bool CanAttack() override;
	virtual void AttackEnd() override;
	virtual void HandleDamage(float DamageAmount) override;
	/** </ABaseCharacter> */

	UPROPERTY(BlueprintReadOnly)
	EDeathOrAlive EnemyAlive = EDeathOrAlive::EDA_Alive;

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
	
	UPROPERTY(BlueprintReadOnly, Category=Combat)
	AActor* CombatTarget;
private:
	/** AI behavior */
	void InitializeEnemy();
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTimerFinished();
	void UpdateHealthBarPercent() const;
	void SetHealthBarVisible(const bool visible) const;
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
	void ClearPatrolTimer();
	void StartAttackTimer();
	void ClearAttackTimer();
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();
	void SpawnDefaultWeapon();

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);
	
	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly)
	double CombatRadius = 500.f;

	UPROPERTY(EditDefaultsOnly)
	double AttackRadius = 150.f;

	/*
	 * Navigation
	 */
	UPROPERTY()
	AAIController* EnemyController;

	/** Patrol */
	UPROPERTY(EditInstanceOnly, Category="AI Navigation")
	AActor* PatrolTarget;
	
	UPROPERTY(EditInstanceOnly, Category="AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditDefaultsOnly)
	double PatrolRadius = 200.f;

	FTimerHandle PatrolTimer;

	UPROPERTY(EditAnywhere, Category="AI Navigation")
	float PatrolWaitMin = 5.f;
	
	UPROPERTY(EditAnywhere, Category="AI Navigation")
	float PatrolWaitMax = 10.f;

	UPROPERTY(EditAnywhere, Category=Combat)
	float PatrollingSpeed = 150.f;

	/** Combat **/
	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category=Combat)
	float AttackWaitMin = 0.5f;
	
	UPROPERTY(EditAnywhere, Category=Combat)
	float AttackWaitMax = 1.f;
	
	UPROPERTY(EditAnywhere, Category=Combat)
	float ChasingSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category=Combat)
	float DeathLifeSpan = 3.f;

	
};
