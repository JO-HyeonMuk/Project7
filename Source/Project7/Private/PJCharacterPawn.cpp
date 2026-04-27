#include "PJCharacterPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

APJCharacterPawn::APJCharacterPawn()
{
    PrimaryActorTick.bCanEverTick = false;


    CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
    SetRootComponent(CapsuleComp);

    CapsuleComp->SetSimulatePhysics(false);

    MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetupAttachment(CapsuleComp);

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
    SpringArmComp->SetupAttachment(CapsuleComp);

    CarmeraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
    CarmeraComp->SetupAttachment(SpringArmComp);

    MoveSpeed = 600.f;
    RotationSpeed = 90.f;
}

void APJCharacterPawn::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

void APJCharacterPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APJCharacterPawn::Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APJCharacterPawn::Look);
    }
}

void APJCharacterPawn::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();
    float DeltaTime = GetWorld()->GetDeltaSeconds();

    FVector Forward = GetActorForwardVector() * MovementVector.Y;
    FVector Right = GetActorRightVector() * MovementVector.X;
    FVector Direction = (Forward + Right).GetSafeNormal();

    FVector DeltaOffset = Direction * MoveSpeed * DeltaTime;

    AddActorWorldOffset(DeltaOffset, true);
}

void APJCharacterPawn::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();
    float DeltaTime = GetWorld()->GetDeltaSeconds();

    FRotator CurrentYawRotation = FRotator(0.f, LookAxisVector.X * RotationSpeed * DeltaTime, 0.f);
    AddActorLocalRotation(CurrentYawRotation);

    if (SpringArmComp)
    {
        FRotator NewSpringArmRotation = SpringArmComp->GetRelativeRotation();
        NewSpringArmRotation.Pitch = FMath::Clamp(NewSpringArmRotation.Pitch + (-LookAxisVector.Y * RotationSpeed * DeltaTime), -80.f, 80.f);

        SpringArmComp->SetRelativeRotation(NewSpringArmRotation);
    }
}