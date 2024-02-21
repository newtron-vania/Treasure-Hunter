// Fill out your copyright notice in the Description page of Project Settings.


#include "TestAttacker.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATestAttacker::ATestAttacker()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestAttacker::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestAttacker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestAttacker::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// 1. 가장 가까운 타겟 선택
void ATestAttacker::FindClosestTarget()
{
    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this); // 자기 자신은 무시
    GetWorld()->OverlapMultiByChannel(OverlapResults, GetActorLocation(), FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1, FCollisionShape::MakeSphere(75), QueryParams);

    ABaseCharacter* ClosestTarget = nullptr;
    float MinDistance = 200.f;

    for (auto& Result : OverlapResults)
    {
        ABaseCharacter* Target = Cast<ABaseCharacter>(Result.GetActor());
        if (Target)
        {
            float Distance = (Target->GetActorLocation() - GetActorLocation()).Size();
            if (Distance < MinDistance)
            {
                MinDistance = Distance;
                ClosestTarget = Target;
            }
        }
    }

    // 공격 대상을 ClosestTarget으로 설정
}

// 2. Sphere 충돌체 생성 및 4. 충돌 처리
void ATestAttacker::CreateAttackSphere()
{
    // Sphere 컴포넌트 생성 및 설정
    USphereComponent* AttackSphere = NewObject<USphereComponent>(this);
    AttackSphere->SetSphereRadius(10.0f);
    AttackSphere->SetCollisionProfileName(TEXT("OverlapAll"));
    AttackSphere->OnComponentBeginOverlap.AddDynamic(this, &ATestAttacker::OnAttackSphereOverlap);
    AttackSphere->RegisterComponent();

    // 0.1초 후에 Sphere 컴포넌트 제거
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, [AttackSphere]() { AttackSphere->DestroyComponent(); }, 0.1f, false);
}

// 충돌 이벤트 핸들러
void ATestAttacker::OnAttackSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ABaseCharacter* Target = Cast<ABaseCharacter>(OtherActor);
    if (Target && !IsAlly(Target))
    {
        // 대상에게 데미지 적용
        UGameplayStatics::ApplyDamage(OtherActor, AttackDamage, GetInstigator()->GetController(), this, UDamageType::StaticClass());
        ExecuteGetHit(SweepResult);
    }
}

void ATestAttacker::ExecuteGetHit(const FHitResult& BoxHit)
{
    IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
    if (HitInterface)
    {
        HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, GetOwner());
    }
}

bool ATestAttacker::IsAlly(ABaseCharacter* Target)
{
    return false;
}
