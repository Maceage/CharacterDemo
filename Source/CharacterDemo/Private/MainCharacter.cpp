// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	EquippedWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightHandWeaponHoldSocket"));
	BackWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackWeaponHoldSocket"));
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	StartFOV = FollowCamera->FieldOfView;
	DesiredFOV = StartFOV;

	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewPitchMin = -30;
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewPitchMax = 30;
}

void AMainCharacter::OnConstruction(const FTransform& Transform)
{
	EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("RightHandWeaponHoldSocket"));
	BackWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("BackWeaponHoldSocket"));
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float FOV = FMath::Lerp(FollowCamera->FieldOfView, DesiredFOV, DeltaTime * 10.0f);
	FollowCamera->SetFieldOfView(FOV);
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMainCharacter::Aim(bool aim)
{
	Super::Aim(aim);

	bUseControllerRotationYaw = aim;
	DesiredFOV = (aim ? AimFOV : StartFOV);
}

void AMainCharacter::Attack()
{
	if (bIsAiming)
	{
		const float kLineTraceDistance = 10000.f;

		FVector CameraLocation = FollowCamera->GetComponentLocation();
		FRotator CameraRotation = FollowCamera->GetComponentRotation();

		FVector Start = CameraLocation;
		FVector End = Start + (CameraRotation.Vector() * kLineTraceDistance);

		FCollisionQueryParams TraceParams(FName(TEXT("InteractTrace")), true, nullptr);
		TraceParams.bTraceComplex = false;

		FHitResult HitDetails = FHitResult(ForceInit);

		bool bIsHit = GetWorld()->LineTraceSingleByChannel(
			HitDetails,		// FHitResult object that will be populated with hit info
			Start,			// Starting position
			End,			// End position
			ECC_Camera,		// Camera channel
			TraceParams);	// Additional trace settings

		if (bIsHit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("We hit something"));

			DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 5.f, ECC_WorldStatic, 1.f);

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Hit Actor Name: %s"), *HitDetails.GetActor()->GetName()));
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Distance: %s"), *FString::SanitizeFloat(HitDetails.Distance)));

			DrawDebugBox(GetWorld(), HitDetails.ImpactPoint, FVector(2.f, 2.f, 2.f), FColor::Blue, false, 5.f, ECC_WorldStatic, 1.f);

			UGameplayStatics::ApplyDamage(HitDetails.GetActor(),	// Damaged Actor
				100,												// Damage
				GetController(),									// Instigator (Controller)
				this,												// Damage Causer (Actor)
				UDamageType::StaticClass());						// Default damage type
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Nothing was hit"));

			DrawDebugLine(GetWorld(), Start, End, FColor::Purple, false, 5.f, ECC_WorldStatic, 1.f);
		}
	}
	else
	{
		Super::Attack();
	}
}