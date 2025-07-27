// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseCharacter::Attack()
{
	// Alternative method
	// UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//
	// if (AnimInstance && AttackMontage)
	// {
	// 	if (!AnimInstance->Montage_IsPlaying(AttackMontage))
	// 	{
	// 		AnimInstance->Montage_Play(AttackMontage, 1.0f);
	// 	}
	// }
	// else
	// {
	// 	//...
	// }
	
	if (AttackMontage)
	{
		if (GetCurrentMontage() == nullptr)
		{
			PlayAnimMontage(AttackMontage);
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, TEXT("MyBaseCharacter: AttackMontage already playing"));
			}
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, TEXT("MyBaseCharacter: Cannot play AttackMontage"));
		}
	}
}

