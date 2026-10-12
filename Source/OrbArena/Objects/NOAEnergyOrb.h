// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NOAEnergyOrb.generated.h"

UCLASS()
class ORBARENA_API ANOAEnergyOrb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANOAEnergyOrb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	UPROPERTY(EditAnywhere, Category = "Orb|Config")
	float PickUpRadius = 100.0f;
	
	UPROPERTY(EditAnywhere, Category = "Orb|Config")
	int32 OrbPointValue = 10;

	UPROPERTY()
	ACharacter * PlayerCharacter;
	
	UPROPERTY(EditAnywhere, Category = "Orb|Config")
	FString OrbName = "Orb";

	UPROPERTY(EditAnywhere, Category ="Orb|Config")
	FColor OrbDebugColor = FColor::Blue;
	
	void OrbPickedUp(int32 InPointWorth);
};
