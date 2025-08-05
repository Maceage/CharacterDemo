// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttackCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttackCapsule"));
	AttackCapsule->InitCapsuleSize(10.f, 30.f);
	AttackCapsule->CanCharacterStepUpOn = ECB_No;
	AttackCapsule->SetGenerateOverlapEvents(true);
	AttackCapsule->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	AttackCapsule->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("LeftHandSocket"));
	AttackCapsule->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnOverlapBegin_AttackCapsule);

	AttackCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::Aim(bool aim)
{
	bIsAiming = aim;
}

bool ABaseCharacter::IsAttacking()
{
	UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;

	if (AnimInstance && AttackMontage)
	{
		return AnimInstance->Montage_IsPlaying(AttackMontage);
	}

	return false;
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

			Aim(false);
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

void ABaseCharacter::ActivateAttack(bool Activate)
{
	if (Activate)
	{
		AttackCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		AttackCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ABaseCharacter::OnOverlapBegin_AttackCapsule(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this && OtherActor->IsA(ACharacter::StaticClass()))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("Hit Other Character"));
		}
	}

	UGameplayStatics::ApplyDamage(OtherActor,	// Damaged Actor
		25,										// Damage
		Cast<APawn>(this)->GetController(),		// Instigator (Controller)
		this,									// Damage Causer (Actor)
		UDamageType::StaticClass());			// Default damage type
}
