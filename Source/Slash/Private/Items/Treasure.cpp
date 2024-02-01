// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"

#include "Pawns/Characters/SlashCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);
	if(SlashCharacter)
	{
		if(PickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				GetWorld(),
				PickupSound,
				GetActorLocation()
				);
		}
		Destroy(this);
	}
}

void ATreasure::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
