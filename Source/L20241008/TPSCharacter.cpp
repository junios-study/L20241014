// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ATPSCharacter::ATPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	SpringArm->TargetArmLength = 150.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);



	GetMesh()->SetRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

}

// Called when the game starts or when spawned
void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = Cast<APlayerController>(GetController());
	//PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if ( IsValid(PC) )
	{
		ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PC->GetLocalPlayer());
		if ( IsValid(LocalPlayer) )
		{
			if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				//if (IsValid(InputMapping.GetClass()))
				if (InputMapping)
				{
					InputSystem->AddMappingContext(InputMapping, 0);
				}
			}
		}
	}
	
}

// Called every frame
void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EIC)
	{
		EIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ATPSCharacter::Move);
		EIC->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ATPSCharacter::Look);
		EIC->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ATPSCharacter::Jump);
		EIC->BindAction(IA_Jump, ETriggerEvent::Canceled, this, &ATPSCharacter::StopJumping);
		EIC->BindAction(IA_Zoom, ETriggerEvent::Triggered, this, &ATPSCharacter::Zoom);
	}

}

void ATPSCharacter::Move(const FInputActionValue& Value)
{
	FVector2D Temp =  Value.Get<FVector2D>();
	FVector Direction(FVector(Temp.Y, Temp.X, 0));
	Direction.Normalize();

	FRotator AbsoluteRotation = GetControlRotation();
	FRotator ForwardRotation(0, AbsoluteRotation.Yaw, 0);

	FVector ForwardVectorXYPlaneBase = UKismetMathLibrary::GetForwardVector(ForwardRotation);

	FVector RightVectorXYPlaneBase = UKismetMathLibrary::GetRightVector(ForwardRotation);

	AddMovementInput(ForwardVectorXYPlaneBase * Temp.Y);
	AddMovementInput(RightVectorXYPlaneBase * Temp.X);
}

void ATPSCharacter::Look(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Look %f %f"), Value.Get<FVector2D>().X, Value.Get<FVector2D>().Y);
	AddControllerPitchInput(Value.Get<FVector2D>().Y);
	AddControllerYawInput(Value.Get<FVector2D>().X);
//	AddActorLocaRotation()
}

void ATPSCharacter::Zoom(const FInputActionValue& Value)
{
	
	SpringArm->TargetArmLength -= (Value.Get<float>() * 60.0f);

	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength, 100, 200);
}

