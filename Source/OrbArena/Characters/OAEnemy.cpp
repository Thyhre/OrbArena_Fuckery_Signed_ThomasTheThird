// Fill out your copyright notice in the Description page of Project Settings.

#include "OrbArena/Characters/OACharacter2.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "OAEnemy.h"

// Sets default values
AOAEnemy::AOAEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOAEnemy::BeginPlay()
{
	Super::BeginPlay();
	PrintPlayerLocation();
	
}




void AOAEnemy::CheckGameState()
{
	if (EnemyLives <= 0)
	{
		GameState = EGameStates::Lost;
	}
	else if (Score >= ScoreWinCondition)
	{
		GameState = EGameStates::Won;
	}
}


void AOAEnemy::ApplyDamageAmount(float InAmount)
{
	Health -= InAmount;
	
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
	
	
	
	UE_LOG(LogTemp, Warning, TEXT("Health: %.0f"), Health);

	if (Health <= 0)
	{
			LoseLives(1);
	}
	//CheckHealth();
	CheckGameState();
}	

void AOAEnemy::Heal(float InAmount)
{
	Health += InAmount;
		
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
		
	UE_LOG(LogTemp, Warning, TEXT("Health: %.0f"), Health);
}

void AOAEnemy::AddScore(int32 InPoints)
{
	Score += InPoints;
	
	if (Score > 1000)
	{
		Difficulty = EGameDifficulties::Hard;
		BaseMovementSpeed = 400.0f;
	}
	
	else if (Score >= 500)
	{
		Difficulty = EGameDifficulties::Medium;
		BaseMovementSpeed = 800.0f;
	}
	
	else if (Score < 500)
	{
		Difficulty = EGameDifficulties::Easy;
		BaseMovementSpeed = 1200.0f;
	}
	UE_LOG(LogTemp, Warning, TEXT("Movement Speed: %f"), BaseMovementSpeed);
	UE_LOG(LogTemp, Warning, TEXT("Score: %d"), Score);
	
	
}	

void AOAEnemy::ReduceScore(int32 InPoints)
{
	Score -= InPoints;
	
	Score = FMath::Max(Score,0.0f);
	
	UE_LOG(LogTemp, Warning, TEXT("Score: %d"), Score);

	
}

void AOAEnemy::LoseLives(int32 InAmount)
{
	if (EnemyLives > 0)
	{
		EnemyLives -= InAmount;
	
		EnemyLives = FMath::Max(EnemyLives,0.0f);
	
		UE_LOG(LogTemp, Warning, TEXT("Player Died! Lives Remaining: %d"),EnemyLives)
		
		Health = MaxHealth;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GAME OVER!!"))
	}
	
	
	
}



// Called every frame
void AOAEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//UpdateVisionAndTurning();
	
	PrintTextToScreen();
	
	
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	
	if (GameState == EGameStates::Playing)
	{
		if (PC)
		{
			if (PC->WasInputKeyJustPressed(EKeys::One))
			{
				ApplyDamageAmount(TempDamage);
			}
		
			if (PC->WasInputKeyJustPressed(EKeys::Two))
			{
				Heal(TempHeal);
			}
			if (PC->WasInputKeyJustPressed(EKeys::Four))
			{
				AddScore(TempPoints);
			}
			if (PC->WasInputKeyJustPressed(EKeys::Three))
			{
				ReduceScore(LessPoints);
			}
		}
	}
}

// Called to bind functionality to input
void AOAEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AOAEnemy::DealDamage()
{
	
}

void AOAEnemy::PrintPlayerLocation()
{
	FVector PlayerLocation = GetActorLocation();
	
	UE_LOG(LogTemp, Warning, TEXT("PlayerLocation: X=%f, Y=%f, Z=%f"), PlayerLocation.X, PlayerLocation.Y, PlayerLocation.Z);
	
	GEngine->AddOnScreenDebugMessage(1, 480.0f, FColor::Yellow,FString::Printf(TEXT("Player Location: X=%.2f, Y=%.2f, Z=%.2f"),PlayerLocation.X, PlayerLocation.Y, PlayerLocation.Z));
	
}	


void AOAEnemy::PrintTextToScreen()
{
	if (!GEngine) return;
	
	int32 DebugKey = 2;
	
	if (GameState == EGameStates::Playing)
	{
		GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Orange, FString::Printf(TEXT("GameState: Playing")));
	}
	else if (GameState == EGameStates::Won)
	{
		GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Orange, FString::Printf(TEXT("GameState: Won")));
	}
	else if (GameState == EGameStates::Lost)
	{
		GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Orange, FString::Printf(TEXT("GameState: Lost")));
	}
	DebugKey++;
	if (Difficulty == EGameDifficulties::Easy)
	{
		GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Orange, FString::Printf(TEXT("Difficulty: Easy")));
	}
	else if (Difficulty == EGameDifficulties::Medium)
	{
		GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Orange, FString::Printf(TEXT("Difficulty: Medium")));
	}
	else if (Difficulty == EGameDifficulties::Hard)
	{
		GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Orange, FString::Printf(TEXT("Difficulty: Hard")));
	}
	DebugKey++;
	GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Yellow, FString::Printf(TEXT("Lives: %.0d"),EnemyLives));
	DebugKey++;
	GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Yellow, FString::Printf(TEXT("Score: %.0d"),Score));
	DebugKey++;
	GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Cyan, FString::Printf(TEXT("----------------------")));
	DebugKey++;
	GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Blue, FString::Printf(TEXT("Speed:%.0f"), BaseMovementSpeed));
	DebugKey++;
	if (BonusDamage > 0)
	{
		GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Purple, FString::Printf(TEXT("Damage:%.0d "), BonusDamage + Damage));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Purple, FString::Printf(TEXT("Damage:%.0d"), Damage));
	}
	DebugKey++;
	GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Cyan, FString::Printf(TEXT("----------------------")));
	DebugKey++;
	if (Health <= 10.f)
	{
		GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Red, FString::Printf(TEXT("Health:%.0f/%.0f"),Health, MaxHealth));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Green, FString::Printf(TEXT("Health:%.0f/%.0f"),Health, MaxHealth));
	}
	DebugKey++;
	if (Health <= MaxHealth * 0.25)
	{
		GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Red, FString::Printf(TEXT("LOW HEALTH!!!")));
	}
	DebugKey++;
	GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Yellow, FString::Printf(TEXT("Level:%d"), EnemyLevel));
	DebugKey++;
	GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Red, FString::Printf(TEXT("Enemy:%s"), *EnemyName));
	
}

void AOAEnemy::UpdateVisionAndTurning(float Deltatime)
{
	FVector VectorToPlayer = PlayerCharacter->GetActorLocation() - GetActorLocation();
	VectorToPlayer.Z = 0.f;
	FVector ToPlayerDirection = VectorToPlayer.GetSafeNormal();
	
	FVector EnemyForwardVector = GetActorForwardVector();
	EnemyForwardVector.Z = 0.f;
	EnemyForwardVector = EnemyForwardVector.GetSafeNormal();

	//Dot Product
	
	DotProductValue = FVector::DotProduct(EnemyForwardVector, ToPlayerDirection);
	bCanSeePlayer = DotProductValue > DotProductValueToSeePlayer;
	
	if (!bCanSeePlayer)
	{
		return;
	}
	
	//Cross product
	CrossProductValue = FVector::CrossProduct(EnemyForwardVector, ToPlayerDirection);
	
	float TurnDirection = 1.f;
	
	if (CrossProductValue.Z < 0.f)
	{
		TurnDirection = -1.f;
	}
	
	float TurnAmount = TurnDirection = TurnSpeed > Deltatime;
	AddActorLocalRotation(FRotator(0.f, TurnAmount, 0.f));

}










