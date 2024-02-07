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
#include "Kismet/GameplayStatics.h"
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

	SetHealthBarPercent();
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
	DeathPose = EDeathPose::EDP_Dead;
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
	MoveRequest.SetAcceptanceRadius(15.f);
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

//When Enemy sensing pawn(ex. SlashCharacter) event
void AEnemy::PawnSeen(APawn* SeenPawn)
{
	if(EnemyState == EEnemyState::EES_Chasing) return;
	if(SeenPawn->ActorHasTag(FName("SlashCharacter")))
	{
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		CombatTarget = SeenPawn;

		if(EnemyState != EEnemyState::EES_Attacking)
		{
			EnemyState = EEnemyState::EES_Chasing;
			MoveToTarget(CombatTarget);
		}
	}
}


void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
		
}

void AEnemy::CheckCombatTarget()
{
	if(!InTargetRange(CombatTarget, CombatRadius))
	{
		// Outside combat radius, lose interest
		SetHealthBarVisible(false);
		CombatTarget = nullptr;

		EnemyState = EEnemyState::EES_Patrolling;
		GetCharacterMovement() -> MaxWalkSpeed = 150.f;
		MoveToTarget(PatrolTarget);
	}
	else if(!InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Chasing)
	{
		//Outside attack range, chase character
		EnemyState = EEnemyState::EES_Chasing;
		GetCharacterMovement() -> MaxWalkSpeed = 300.f;
		MoveToTarget(CombatTarget);
	}
	else if(InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Attacking)
	{
		//Inside attack range, attack character
		EnemyState = EEnemyState::EES_Attacking;
		//Play attack animation
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
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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
	if(Attributes && HealthBarWidget)
	{
		Attributes->ReceiveDamage(DamageAmount);
	}
	SetHealthBarPercent();

	CombatTarget = EventInstigator->GetPawn();
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	MoveToTarget(CombatTarget);
	
	return DamageAmount;
}

/*
 * GetHit 인터페이스 구현
 */
void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	SetHealthBarVisible(true);
	
	if(Attributes && Attributes->isAlive())
	{
		DirectionalHitReact(ImpactPoint);
	}
	else
	{
		Die();
	}
		
	PlayHitSound(ImpactPoint);
	PlayHitParticle(ImpactPoint);
}

void AEnemy::PlayHitSound(const FVector& ImpactPoint)
{
	if(HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
			);
	}
}

void AEnemy::PlayHitParticle(const FVector& ImpactPoint)
{
	if(HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
			);
	}
}

void AEnemy::SetHealthBarPercent()
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

