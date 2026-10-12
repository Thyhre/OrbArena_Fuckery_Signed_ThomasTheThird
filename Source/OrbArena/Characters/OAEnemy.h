// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "OAEnemy.generated.h"


UENUM()
enum class EGameStates : uint8
{
	Playing,
	Won,
	Lost
};

UENUM()
enum class EGameDifficulties : uint8
{
	Easy,
	Medium,
	Hard
};

UCLASS()
class ORBARENA_API AOAEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOAEnemy();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
	UPROPERTY(EditAnywhere, Category = "Enemy | Stats");
	float DamageRadius = 25.f;
// Denne Kan ha vært helt unødvendig og kommer sikkert til å fjernes senere.	
//	UPROPERTY(EditAnywhere, Category = "Enemy | Stats");
//	float InAmount = 10.f;

	
	UPROPERTY()
	ACharacter*PlayerCharacter;
	
	
	
	UPROPERTY(EditAnywhere, Category = "Enemy | Stats")
	float Health = 50.f;
	
	UPROPERTY(EditAnywhere, Category = "Enemy | Stats")
	float MaxHealth = 100.f;
	
	UPROPERTY(EditAnywhere, Category = "Enemy | Stats")
	int32 EnemyLevel = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Enemy | Stats")
	int32 EnemyLives = 3.f;
	
	
	UPROPERTY(EditAnywhere, Category = "Enemy | Movement")
	float BaseMovementSpeed = 100.f;
	
	UPROPERTY(EditAnywhere, Category = "Enemy | Movement")
	bool bIsSprinting = false;
	
	UPROPERTY(EditAnywhere, Category = "Enemy | Movement")
	float SprintMultiplier = 1.5f;
	
	
	UPROPERTY(EditAnywhere, Category = "Enemy | Stats")
	int Damage = 10.f;
	
	UPROPERTY(EditAnywhere, Category = "Enemy | Stats")
	int BonusDamage = 0.f;	
	
	UPROPERTY(EditAnywhere, Category = "Enemy | Stats")
	float TempDamage = 10.0f;
	
	UPROPERTY(EditAnywhere, Category = "Enemy | Stats")
	float TempHeal = 10.0f;
	
	UPROPERTY(EditAnywhere, Category = "Enemy | Stats")
	int32 TempPoints = 200.0f;
	
	
	UPROPERTY(EditAnywhere, Category = "Enemy | Info")
	int32 Score = 10.0f;
	
	UPROPERTY(EditAnywhere, Category = "Enemy | Info")
	int32 LessPoints = 200.0f;
	
	UPROPERTY(EditAnywhere, Category = "Enemy | System")
	EGameStates GameState = EGameStates::Playing;	

	UPROPERTY(EditAnywhere, Category = "Enemy | System")
	EGameDifficulties Difficulty = EGameDifficulties::Easy;
	
	UPROPERTY(EditAnywhere, Category = "Enemy | System")
	int ScoreWinCondition = 2000.0f;	
	
	/*
	 * Enemy Variables
	 */
	UPROPERTY(EditAnywhere, Category = "Enemy | System")
	FString EnemyName = "Tobben";
		
	
	// Helpers
	UPROPERTY()
	/*
	 * AI Variables
	 */
	//UPROPERTY(EditAnywhere, Category = "Enemy | Vision", meta = (ClampMin = "-1.0", ClampMax = "1.0"))
	float DotProductValueToSeePlayer = 0.5f;
	
	UPROPERTY(VisibleAnywhere, Category = "Enemy | Vision")
	float DotProductValue= 0.f;
	
	UPROPERTY(VisibleAnywhere, Category = "Enemy | Vision")
	FVector CrossProductValue = FVector(0.f, 0.f, 0.f);
	
	UPROPERTY(VisibleAnywhere, Category = "Enemy | Vision")
	bool bCanSeePlayer = false;
	
	UPROPERTY(EditAnywhere, Category = "Enemy | Vision")
	float TurnSpeed = 90.f;
	
	UPROPERTY(EditAnywhere, Category = "Enemy | Vision")
	bool bRequireVisionToTurn = true;
	
	/*
	 * Debugging
	 */
	
	UPROPERTY(EditAnywhere, Category = "Enemy | Debug")
	bool bDrawDebug = true;
	
	
	/*
	 * Functions
	 */
	
	void AttackPlayer();
	void DealDamage();
	void Debugging();
	void UpdateVisionAndTurning(float Deltatime);
	
	// How much Damage?
	//Players Health?
	// Is it in Range??
	
	void PrintTextToScreen();
	void ApplyDamageAmount(float InAmount);
	void Heal(float InAmount);
	void AddScore(int32 InPoints);
	void ReduceScore(int32 InPoints);
	void LoseLives(int32 InAmount);
	void CheckGameState();
	//void CheckHealth();
	void PrintPlayerLocation();
	
};
