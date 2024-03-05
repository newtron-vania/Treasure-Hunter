#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"

class IHitReactStrategy;
class IMoveStrategy;
class IAttackStrategy;
class AWeapon;
class UAttributeComponent;
class UAnimMontage;

UCLASS()
class SLASH_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;


protected:
	virtual void BeginPlay() override;

	/** Combat */
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual void Attack();
	virtual void Die();
	void DisableMeshCollision();

	void DirectionalHitReact(const FVector& ImpactPoint);
	virtual void HandleDamage(float DamageAmount);
	virtual void PlayHitSound(const FVector& ImpactPoint);
	virtual void SpawnHitParticle(const FVector& ImpactPoint);
	virtual void DisableCapsule();
	virtual bool CanAttack();
	bool IsAlive();
	
	/** Montage */
	void PlayHitReactMontage(const FName& SectionName);
	int32 PlayAttackMontage();
	virtual int32 PlayDeathMontage();
	void PlayDodgeMontage();
	void StopAttackMontage();

	/** Strategy */
	void ChangeMoveStrategy(IMoveStrategy* MoveStrategy);

	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();

	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	UFUNCTION(BlueprintCallable)
	virtual void DodgeEnd();
	
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAttributeComponent* Attributes;

	UPROPERTY(BlueprintReadOnly, Category=Combat)
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category=Combat)
	double WarpTargetDistance = 75.f;

	// Strategy
	IAttackStrategy* AttackStrategy;

	IMoveStrategy* MoveStrategy;

	IHitReactStrategy* HitReactStrategy;
private:
	void PlayMontageSection(UAnimMontage* Montage, FName SectionName);
	int32 PlayRandomMontageSection(UAnimMontage* Montage);

	UPROPERTY(EditAnywhere, Category = Combat)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = Combat)
	UParticleSystem* HitParticles;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* DodgeMontage;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	TArray<UAnimMontage*> DeathMontages;
};