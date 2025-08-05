// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "MainCharacter.h"
#include "GameFramework/PawnMovementComponent.h"

void UMainAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (MainCharacter == nullptr)
	{
		MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
	}
}

void UMainAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (MainCharacter == nullptr)
	{
		MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
	}

	if (MainCharacter)
	{
		FVector Speed = MainCharacter->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0);

		MovementSpeed = LateralSpeed.Size();
		
		bIsInAir = MainCharacter->GetMovementComponent()->IsFalling();
		
		Direction = UKismetAnimationLibrary::CalculateDirection(Speed, MainCharacter->GetActorRotation());

		bIsAiming = MainCharacter->bIsAiming;

		FRotator PawnRotation = MainCharacter->GetActorRotation();
		FRotator ControllerRotation = MainCharacter->GetControlRotation();
		FRotator DeltaRotation = ControllerRotation - PawnRotation;

		FRotator Interp = FMath::RInterpTo(FRotator(AimPitch, AimYaw, 0), DeltaRotation, DeltaTime, 15.0f);
		AimPitch = FMath::ClampAngle(Interp.Pitch, -90, 90);
		AimYaw = FMath::ClampAngle(Interp.Yaw, -90, 90);
	}
}
