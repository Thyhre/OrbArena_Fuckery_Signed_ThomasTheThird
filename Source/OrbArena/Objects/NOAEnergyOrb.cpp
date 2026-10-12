// Fill out your copyright notice in the Description page of Project Settings.


#include "NOAEnergyOrb.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "OrbArena/Characters/OACharacter2.h"

// Sets default values
ANOAEnergyOrb::ANOAEnergyOrb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANOAEnergyOrb::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
#if 0
	//TArray<INT> DynamicArray;
	//int StaticArray [5] = {0,1,2,3,4};
	
	TArray<ANOAEnergyOrb*> Energyorbs;
	
	Energyorbs.Num();
	Energyorbs.Add(this);
	Energyorbs.Remove(this);
	
	
	
	for (ANOAEnergyOrb Oa : Energyorbs.Num())
	{
		Oa;
	}
	for (int i=0; 1<Energyorbs. Num();i++)
	{
		Energyorbs[10];
	}
#endif

}

// Called every frame
void ANOAEnergyOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsValid(PlayerCharacter))
	{
		FVector PlayerLocation = PlayerCharacter->GetActorLocation();
		FVector OrbLocation = GetActorLocation();
	
		if (FVector::Dist(OrbLocation, PlayerLocation) <= PickUpRadius)
		{
			OrbPickedUp(OrbPointValue);
		}
	}

	DrawDebugSphere(GetWorld(), GetActorLocation(),PickUpRadius, 100, OrbDebugColor, false, 0.1f, 0.2f);

}




void ANOAEnergyOrb::OrbPickedUp(int32 InPointWorth)
{
	if (IsValid(PlayerCharacter))
	{
		Cast<AOACharacter2>(PlayerCharacter)->AddScore(InPointWorth);
		UE_LOG(LogTemp, Warning, TEXT("PICKED UP! %d points"), OrbPointValue);
		Destroy();
	}
	
}

