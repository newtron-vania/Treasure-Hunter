// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"

#include "Characters/SlashCharacter.h"
#include "Components/StaticMeshComponent.h"

void ATreasure::PlayCoinSound()
{
	SpawnPickupSound();
}

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		PickupInterface->AddGold(this);

		PlayCoinSound();
		Destroy();
	}
}
