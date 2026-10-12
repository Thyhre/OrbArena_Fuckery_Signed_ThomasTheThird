// Fill out your copyright notice in the Description page of Project Settings.


#include "Tomas.h"

#include "InputDebugKeyDelegateBinding.h"

// Sets default values
ATomas::ATomas()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATomas::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATomas::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATomas::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

void ATomas::PrintTextToScreen()
{
	int DebugKey = 1;
	
	GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Emerald, FString::Printf(TEXT("Name: %s"), *PlayerName));
	
}