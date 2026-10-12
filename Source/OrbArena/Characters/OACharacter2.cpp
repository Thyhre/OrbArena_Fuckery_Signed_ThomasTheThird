// Fill out your copyright notice in the Description page of Project Settings.


#include "OACharacter2.h"

#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "OrbArena/Objects/NOAEnergyOrb.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AOACharacter2::AOACharacter2()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Create the camera boom component
	CameraStick = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraStick"));

	CameraStick->SetupAttachment(RootComponent);
	CameraStick->TargetArmLength = 400.f;
	CameraStick->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));
	CameraStick->bDoCollisionTest = true;
	CameraStick->bUsePawnControlRotation = true;

	// Create the camera component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(CameraStick, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void AOACharacter2::BeginPlay()
{
	Super::BeginPlay();
	
	
	TArray<AActor*> EnergyOrbs;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),ANOAEnergyOrb::StaticClass(),EnergyOrbs);

	for (AActor* Eo: EnergyOrbs)
	{
		EnergyOrbs.Add(Cast<ANOAEnergyOrb>(Eo));
	}
	
	
	UpdateMovementSpeed();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
				LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				InputSubsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

// Called every frame
void AOACharacter2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	PrintTextToScreen();
	PrintPlayerLocation();

	if (GameState == EGameState::Playing)
	{
		if (Health <= 0 && PlayerLives > 0)
		{
			LoseLife();
		}

		CheckGameState();
	}
}

// Called to bind functionality to input
void AOACharacter2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOACharacter2::MovePlayer);

	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AOACharacter2::StartSprint);

	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AOACharacter2::StopSprint);

	EnhancedInputComponent->BindAction(DamageAction, ETriggerEvent::Triggered, this, &AOACharacter2::DamageInput);

	EnhancedInputComponent->BindAction(HealAction, ETriggerEvent::Triggered, this, &AOACharacter2::HealInput);

	EnhancedInputComponent->BindAction(AddScoreAction, ETriggerEvent::Triggered, this, &AOACharacter2::AddScoreInput);
}

void AOACharacter2::MovePlayer(const FInputActionValue& Value)
{
	if (GameState != EGameState::Playing)
	{
		return;
	}

	FVector2D MovementValue = Value.Get<FVector2D>();

	AddMovementInput(GetActorForwardVector(), MovementValue.X);
	AddMovementInput(GetActorRightVector(), MovementValue.Y);
}

void AOACharacter2::StartSprint()
{
	// Either return early if the game state is not playing, or only do action if the gamestate is playing
	if (GameState != EGameState::Playing)
	{
		return;
	}

	bIsSprinting = true;
	UpdateMovementSpeed();
}

void AOACharacter2::StopSprint()
{
	bIsSprinting = false;
	UpdateMovementSpeed();
}

void AOACharacter2::DamageInput()
{
	// Either return early if the game state is not playing, or only do action if the gamestate is playing
	if (GameState == EGameState::Playing)
	{
		ApplyDamageAmount(TempDamage);
	}
}

void AOACharacter2::HealInput()
{
	// Either return early if the game state is not playing, or only do action if the gamestate is playing
	if (GameState == EGameState::Playing)
	{
		HealPlayer(TempHeal);
	}
}

void AOACharacter2::AddScoreInput()
{
	// Either return early if the game state is not playing, or only do action if the gamestate is playing
	if (GameState == EGameState::Playing)
	{
		AddScore(TempPoints);
	}
}

void AOACharacter2::UpdateMovementSpeed()
{
	float DifficultyMultiplier = MovementMultiplierEasy;

	if (GameDifficulty == EGameDifficulty::Medium)
	{
		DifficultyMultiplier = MovementMultiplierMedium;
	}
	else if (GameDifficulty == EGameDifficulty::Hard)
	{
		DifficultyMultiplier = MovementMultiplierHard;
	}

	float ActiveSprintMultiplier = 1.f;
	if (bIsSprinting)
	{
		ActiveSprintMultiplier = SprintMultiplier;
	}

	CurrentMovementSpeed = BaseMovementSpeed * DifficultyMultiplier * ActiveSprintMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = CurrentMovementSpeed;
}

void AOACharacter2::AddScore(int32 InAmount)
{
	Score += InAmount;

	if (Score > 1000)
	{
		GameDifficulty = EGameDifficulty::Hard;
	}
	else if (Score >= 500)
	{
		GameDifficulty = EGameDifficulty::Medium;
	}
	else
	{
		GameDifficulty = EGameDifficulty::Easy;
	}

	UpdateMovementSpeed();

	UE_LOG(LogTemp, Warning, TEXT("New score is %d"), Score);
}

void AOACharacter2::HealPlayer(float InAmount)
{
	Health += InAmount;
	Health = FMath::Clamp(Health, 0, MaxHealth);

	UE_LOG(LogTemp, Warning, TEXT("Player healed, new Health is %.2f"), Health);
}

void AOACharacter2::ApplyDamageAmount(float InAmount)
{
	Health -= InAmount;
	Health = FMath::Clamp(Health, 0, MaxHealth);

	UE_LOG(LogTemp, Warning, TEXT("Player damaged, new Health is %.2f"), Health);

	if (MaxHealth > 0.f && Health / MaxHealth <= 0.25f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Emerald, FString::Printf(TEXT("LOW HEALTH!!!")));
	}
}

void AOACharacter2::LoseLife()
{
	PlayerLives -= 1;
	PlayerLives = FMath::Max(PlayerLives, 0);

	if (PlayerLives == 0)
	{
		GameState = EGameState::Lost;
		UE_LOG(LogTemp, Warning, TEXT("GAME OVER!!"));
		return;
	}

	Health = MaxHealth;
	UE_LOG(LogTemp, Warning, TEXT("Player died! Current lives: %d"), PlayerLives);
}

void AOACharacter2::CheckGameState()
{
	if (PlayerLives == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("GAME OVER!!"));
		GameState = EGameState::Lost;
	}
	else if (Score >= ScoreWinCondition)
	{
		UE_LOG(LogTemp, Warning, TEXT("YOU WON!!!"));
		GameState = EGameState::Won;
	}
}

void AOACharacter2::PrintPlayerLocation()
{
	FVector PlayerLocation = GetActorLocation();

	GEngine->AddOnScreenDebugMessage(11, 0.1f, FColor::Yellow, FString::Printf(TEXT("Player Location - X: %.1f, Y: %.1f, Z: %.1f"), PlayerLocation.X, PlayerLocation.Y, PlayerLocation.Z));
}

void AOACharacter2::PrintTextToScreen()
{
	int DebugKey = 1;

	if (GameState == EGameState::Playing)
	{
		GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Purple, FString::Printf(TEXT("Game State: Playing")));
	}
	else if (GameState == EGameState::Won)
	{
		GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Purple, FString::Printf(TEXT("Game State: Won")));
	}
	else if (GameState == EGameState::Lost)
	{
		GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Purple, FString::Printf(TEXT("Game State: Lost")));
	}
	DebugKey++;

	GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Cyan, FString::Printf(TEXT("----------------------")));
	DebugKey++;

	GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Emerald, FString::Printf(TEXT("Player Name is: %s"), *PlayerName));
	DebugKey++;

	GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Emerald, FString::Printf(TEXT("Player Level is: %d"), PlayerLevel));
	DebugKey++;

	GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Emerald, FString::Printf(TEXT("Score is: %d"), Score));
	DebugKey++;

	GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Emerald, FString::Printf(TEXT("Current Lives is: %d"), PlayerLives));
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
#if 0
	GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Red, FString::Printf(TEXT("Health is: %.0f/%.0f"), Health,MaxHealth));
	DebugKey++;

	GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Red, FString::Printf(TEXT("Max Health is: %.2f"), MaxHealth));
	DebugKey++;
#endif
	if (CurrentMovementSpeed >= 2000.f)
	{
		GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Red, FString::Printf(TEXT("PLAYER IS SONIC!! Speed is: %.2f"), CurrentMovementSpeed));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(DebugKey, 0.1f, FColor::Red, FString::Printf(TEXT("Movement Speed is: %.2f"), CurrentMovementSpeed));
	}
}

#if 0
void UpdateRemainingOrbCount(TArray<ANOAEnergyOrb*> ArrayOrbs)
{
	for (ANOAEnergyOrb* ArrayOrb: ArrayOrbs)
	{
		if (ArrayOrbs == nullptr)
		{
			ArrayOrbs.Remove(ArrayOrb);
		}
		UE_LOG(LogTemp, Warning, TEXT("Remaining Time %d"))
	}
}

#endif



// void AOrbArenaCharacter::CheckKeyPressed()
// {
// 	if (APlayerController* PC = Cast<APlayerController>(GetController()))
// 	{
// 		if (PC->IsInputKeyDown(EKeys::One))
// 		{
// 			ApplyDamageAmount(TempDamage);
// 		}
//
// 		if (PC->IsInputKeyDown(EKeys::Two))
// 		{
// 			HealPlayer(TempHeal);
// 		}
//
// 		if (PC->IsInputKeyDown(EKeys::Three))
// 		{
// 			AddScore(TempPoints);
// 		}
// 	}
// }
//
// //Mathematical usage of vector concept
// void AOrbArenaCharacter::MovePlayer(float DeltaTime)
// {
// 	//TODO: take the input direction, make it length 1, multiply it by how fast the player should move and how much time passed, then add that small movement to the player’s current position.
//
// 	FVector MoveDirection = FVector::ZeroVector;
//
// 	// Current player location - p = (x,y,z)
// 	FVector CurrentLocation = GetActorLocation();
// 	if (APlayerController* PC = Cast<APlayerController>(GetController()))
// 	{
// 		// X axis = forward/backward in world space
// 		if (PC->IsInputKeyDown(EKeys::W))
// 		{
// 			MoveDirection.X += 1.f;
// 		}
//
// 		if (PC->IsInputKeyDown(EKeys::S))
// 		{
// 			MoveDirection.X -= 1.f;
// 		}
//
// 		// Y axis = left/right in world space
// 		if (PC->IsInputKeyDown(EKeys::D))
// 		{
// 			MoveDirection.Y += 1.f;
// 		}
//
// 		if (PC->IsInputKeyDown(EKeys::A))
// 		{
// 			MoveDirection.Y -= 1.f;
// 		}
// 	}
//
// 	// Pressing W gives d = (1,0,0)
// 	// Pressing W and D together gives d = (1,1,0)
// 	// Normalize to prevent diagonal movement from being 41% faster.
// 	MoveDirection = MoveDirection.GetSafeNormal();
//
// 	// Distance = direction × speed × time
// 	const FVector MovementThisFrame = MoveDirection * CurrentMovementSpeed * DeltaTime;
//
// 	// New position = old position + movement
// 	const FVector NewLocation = CurrentLocation + MovementThisFrame;
//
// 	SetActorLocation(NewLocation);
// }
//
// void AOrbArenaCharacter::PlayerSprint()
// {
// 	if (APlayerController* PC = Cast<APlayerController>(GetController()))
// 	{
// 		bIsSprinting = PC->IsInputKeyDown(EKeys::LeftShift);
//
// 		float DifficultyMultiplier = MovementMultiplierEasy;
//
// 		if (GameDifficulty == EGameDifficulty::Medium)
// 		{
// 			DifficultyMultiplier = MovementMultiplierMedium;
// 		}
// 		else if (GameDifficulty == EGameDifficulty::Hard)
// 		{
// 			DifficultyMultiplier = MovementMultiplierHard;
// 		}
//
// 		float ActiveSprintMultiplier;
// 		if (bIsSprinting)
// 		{
// 			ActiveSprintMultiplier = SprintMultiplier;
// 		}
// 		else
// 		{
// 			ActiveSprintMultiplier = 1.f;
// 		}
//
// 		CurrentMovementSpeed = BaseMovementSpeed * DifficultyMultiplier * ActiveSprintMultiplier;
//
// 		GetCharacterMovement()->MaxWalkSpeed = CurrentMovementSpeed;
// 	}
// }


