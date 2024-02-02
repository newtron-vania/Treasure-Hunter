// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/HealthBarComponent.h"


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
	
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	SetHealthBarPercent();
}

void AEnemy::PlayHitReactMontage(const FName SectionName) const
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if(AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}


void AEnemy::PlayDeathMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if(AnimInstance && DeathMontages.Num() > 0)
	{
		UAnimMontage* Montage = DeathMontages[FMath::RandRange(0, DeathMontages.Num()-1)];
		AnimInstance->Montage_Play(Montage);
		
		DeathPose = EDeathPose::EDP_Dead;
	}
}

void AEnemy::Die()
{
	PlayDeathMontage();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if(Attributes && HealthBarWidget)
	{
		Attributes->ReceiveDamage(DamageAmount);
	}
	SetHealthBarPercent();
	return DamageAmount;
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
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

