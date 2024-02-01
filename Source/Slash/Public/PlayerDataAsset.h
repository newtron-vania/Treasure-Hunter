// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerDataAsset.generated.h"

USTRUCT()
struct FMyAssetInfo {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString AssetName;

	UPROPERTY(EditAnywhere)
	UTexture2D* AssetThumbnail;

	UPROPERTY(EditAnywhere)
	UStaticMesh* AssetStaticMesh; 
};

/**
 * 
 */
UCLASS()
class SLASH_API UPlayerDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FMyAssetInfo> AssetItems;
	
};
