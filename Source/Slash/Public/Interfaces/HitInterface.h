#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitInterface.generated.h"

UINTERFACE(MinimalAPI)
class UHitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SLASH_API IHitInterface
{
	GENERATED_BODY()

public:
	/*
	 * BlueprintNativeEvent : 해당 인터페이스를 작성한 C++ 구현 함수를 Blueprint Graph에서 호출 가능
	 * 상속받은 클래스들은 구현할 때 함수명_Implementation으로 함수명을 작성
	 */
	UFUNCTION(BlueprintNativeEvent)
	void GetHit(const FVector& ImpactPoint, AActor* Hitter);
};
