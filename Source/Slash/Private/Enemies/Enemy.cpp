// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "HUD/HealthBarComponent.h"
#include "AIController.h"
#include "AI/Navigation/NavigationTypes.h"
#include "Characters/SlashCharacter.h"

#include "Slash/DebugMacros.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing-> SightRadius = 4000.f;
	PawnSensing-> SetPeripheralVisionAngle(45.f);
}

//Death Montage 실행
void AEnemy::PlayDeathMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if(AnimInstance && DeathMontages.Num() > 0)
	{
		UAnimMontage* Montage = DeathMontages[FMath::RandRange(0, DeathMontages.Num()-1)];
		AnimInstance->Montage_Play(Montage);
	}
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	UpdateHealthBarPercent();
	SetHealthBarVisible(false);

	EnemyController = Cast<AAIController>(GetController());
	MoveToTarget(PatrolTarget);

	if(PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	}
}

void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}

//Enemy Death 실행
void AEnemy::Die()
{
	EnemyAlive = EDeathOrAlive::EDA_Dead;
	PlayDeathMontage();
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetHealthBarVisible(false);
}

// 거리 내 타겟 유무
bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if(Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}

// 타겟으로 이동
void AEnemy::MoveToTarget(AActor* Target)
{
	if(EnemyController == nullptr || Target == nullptr) return;

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(50.f);
	EnemyController->MoveTo(MoveRequest);
	
}

// 수색 위치를 선택
AActor* AEnemy::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for(auto Target : PatrolTargets)
	{
		if(Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}
			
	const int32 NumPatrolTargets = ValidTargets.Num();
	if(NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets-1);
		return ValidTargets[TargetSelection];
	}
	return nullptr;
}

void AEnemy::Attack()
{
	Super::Attack();
	PlayAttackMontage();
}

void AEnemy::PlayAttackMontage()
{
	Super::PlayAttackMontage();
}

bool AEnemy::CanAttack()
{
	bool bCanAttack =
		IsInsideAttackRadius() &&
			!IsAttacking()&&
				!IsDead();
	return bCanAttack;
}

void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

bool AEnemy::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;
}

//When Enemy sensing pawn(ex. SlashCharacter) event
void AEnemy::PawnSeen(APawn* SeenPawn)
{
	const bool bShouldChaseTarget =
		!IsDead() &&
			!IsChasing()&&
				EnemyState < EEnemyState::EES_Attacking &&
					SeenPawn->ActorHasTag(FName("SlashCharacter"));
	if(bShouldChaseTarget)
	{
		CombatTarget = SeenPawn;
		ClearPatrolTimer();
		ChaseTarget();
	}
}


bool AEnemy::IsDead()
{
	return EnemyAlive == EDeathOrAlive::EDA_Dead;
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsDead())
	{
		
	}
	if(EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
		
}

// Outside combat radius, lose interest
void AEnemy::LooseInterest()
{
	SetHealthBarVisible(false);
	CombatTarget = nullptr;
}

void AEnemy::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement() -> MaxWalkSpeed = PatrollingSpeed;
	MoveToTarget(PatrolTarget);
}

//Outside attack range, chase character
void AEnemy::ChaseTarget()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement() -> MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
}

bool AEnemy::IsOutsidedCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);
}

bool AEnemy::IsOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsInsideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}

void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer,this, &AEnemy::Attack, AttackTime);
}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

bool AEnemy::IsEngaged()
{
	return EnemyState == EEnemyState::EES_Engaged;
}

void AEnemy::CheckCombatTarget()
{
	if(IsOutsidedCombatRadius())
	{
		ClearAttackTimer();
		LooseInterest();
		if(!IsEngaged()) StartPatrolling();
	}
	else if(IsOutsideAttackRadius() && !IsChasing())
	{
		ClearAttackTimer();
		if(!IsEngaged()) ChaseTarget();
	}
	else if(CanAttack())
	{
		ClearAttackTimer();
		StartAttackTimer();
	}
}

void AEnemy::CheckPatrolTarget()
{
	if(InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(WaitMin,WaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime);
	}
}

/*
 * 언리얼 엔진 피격 이벤트 자동 실행
 * DamageAmount : 데미지량
 * DamageEvent : 데미지 실행 이벤트
 * EventInstigator : 공격 실행자(플레이어)
 * DamageCauser : 충돌 이벤트 대상자(무기)
 */
float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	CombatTarget = EventInstigator->GetPawn();
	ChaseTarget();
	
	return DamageAmount;
}

bool AEnemy::IsAlive()
{
	return Attributes && Attributes->isAlive();
}

/*
 * GetHit 인터페이스 구현
 */
void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	SetHealthBarVisible(true);
	
	if(IsAlive())
	{
		DirectionalHitReact(ImpactPoint);
	}
	else
	{
		Die();
	}
		
	PlayHitSound(ImpactPoint);
	SpawnHitParticle(ImpactPoint);
}

void AEnemy::PlayHitSound(const FVector& ImpactPoint)
{
	Super::PlayHitSound(ImpactPoint);
}

void AEnemy::SpawnHitParticle(const FVector& ImpactPoint)
{
	Super::SpawnHitParticle(ImpactPoint);
}

void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);
	UpdateHealthBarPercent();
}

void AEnemy::UpdateHealthBarPercent()
{
	if(Attributes && HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
}

void AEnemy::SetHealthBarVisible(bool visible)
{	if(HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(visible);
	}
}

