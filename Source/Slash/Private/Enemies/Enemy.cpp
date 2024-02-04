// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/HealthBarComponent.h"
#include "AIController.h"
#include "NavigationPath.h"
#include "Slash/DebugMacros.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	SetHealthBarPercent();

	SetHealthBarVisible(false);
}

//hitReact Montage 실행
void AEnemy::PlayHitReactMontage(const FName SectionName) const
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if(AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName);
	}
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

//Enemy Death 실행
void AEnemy::Die()
{
	DeathPose = EDeathPose::EDP_Dead;
	PlayDeathMontage();
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetHealthBarVisible(false);
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	DRAW_SPHERE_SingleFrame(GetActorLocation());
	DRAW_SPHERE_SingleFrame(Target->GetActorLocation());
	return DistanceToTarget <= Radius;
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(CombatTarget)
	{
		if(!InTargetRange(CombatTarget, CombatRadius))
		{
			SetHealthBarVisible(false);
			CombatTarget = nullptr;
		}
	}
	if(PatrolTarget && EnemyController)
	{
		if(InTargetRange(PatrolTarget, PatrolRadius))
		{
			TArray<AActor*> ValidTargets;
			for(auto Target : PatrolTargets)
			{
				if(Target != PatrolTarget)
				{
					ValidTargets.Add(Target);
				}
			}
			
			const int32 NumPatrolTargets = PatrolTargets.Num();
			if(PatrolTargets.Num() > 0)
			{
				const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets-1);
				AActor* Target = PatrolTargets[TargetSelection];
				PatrolTarget = Target;

				FAIMoveRequest MoveRequest;
				MoveRequest.SetGoalActor(PatrolTarget);
				MoveRequest.SetAcceptanceRadius(15.f);
				FNavPathSharedPtr NavPath;
				EnemyController->MoveTo(MoveRequest, &NavPath);
			}
			
		}
	}

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z); 
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();
	// Forward * ToHit = |Forward||ToHot| * cos(theta)
	const double CosTheta = FVector::DotProduct(Forward,ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	const FVector CrossProduct = FVector::CrossProduct(Forward,ToHit);
	if(CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	FName Section("FromBack");
	if(Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
	}
	else if(Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("FromRight");
	}
	else if(Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
	}
	PlayHitReactMontage(Section);

	/*
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f, 5.f, FColor::Red, 5.f);
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta : %f"), Theta));
	}
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);
	*/
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

