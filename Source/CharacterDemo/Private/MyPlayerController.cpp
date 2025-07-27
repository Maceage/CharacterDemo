// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "BaseCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMyPlayerController::StopMove);
		
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Look);
		
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMyPlayerController::StopJumping);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AMyPlayerController::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMyPlayerController::StopSprint);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AMyPlayerController::Attack);
	}
}

void AMyPlayerController::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		ControlledCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
		
		const FRotator Rotation = GetControlRotation();
		const FRotator CameraRotation(0, Rotation.Yaw, 0);
		
		const FVector ForwardDirection = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::X);
		ControlledCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
		
		const FVector RightDirection = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::Y);
		ControlledCharacter->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMyPlayerController::StopMove()
{
	if (ACharacter* ControlledCharacter = CastChecked<ACharacter>(GetCharacter()))
	{
		ControlledCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

void AMyPlayerController::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);
}

void AMyPlayerController::Jump()
{
	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		ControlledCharacter->Jump();
	}
}

void AMyPlayerController::StopJumping()
{
	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		ControlledCharacter->StopJumping();
	}
}

void AMyPlayerController::StartSprint()
{
	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
}

void AMyPlayerController::StopSprint()
{
	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void AMyPlayerController::Attack()
{
	if (ABaseCharacter* ControlledCharacter = CastChecked<ABaseCharacter>(GetCharacter()))
	{
		ControlledCharacter->Attack();
	}
}
