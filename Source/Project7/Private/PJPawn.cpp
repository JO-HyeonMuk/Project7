#include "PJPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

APJPawn::APJPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
    RootComponent = CapsuleComp;
    CapsuleComp->SetSimulatePhysics(false);

    MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetupAttachment(RootComponent);

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
    SpringArmComp->SetupAttachment(RootComponent);
    SpringArmComp->TargetArmLength = 400.0f;

    SpringArmComp->bInheritPitch = true;
    SpringArmComp->bInheritYaw = true;
    SpringArmComp->bInheritRoll = true;

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
}

void APJPawn::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultContext, 0);
        }
    }
}

void APJPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bIsOnGround)
    {
        Velocity.Z += GravityZ * DeltaTime;
    }

    FVector DeltaLocation = Velocity * DeltaTime;
    FHitResult HitResult;
    AddActorWorldOffset(DeltaLocation, true, &HitResult);

    if (HitResult.IsValidBlockingHit())
    {
        if (HitResult.Normal.Z > 0.5f && Velocity.Z < 0.0f)
        {
            Velocity.Z = 0.0f;
            bIsOnGround = true;
        }
    }
    else
    {
        bIsOnGround = false;
    }
}

void APJPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APJPawn::Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APJPawn::Look);
    }
}

void APJPawn::Move(const FInputActionValue& Value)
{
    FVector MovementVector = Value.Get<FVector>();

    UE_LOG(LogTemp, Warning, TEXT("Input: %s"), *MovementVector.ToString());

    if (Controller != nullptr)
    {
        float CurrentSpeed = bIsOnGround ? MoveSpeed : (MoveSpeed * AirControlFactor);

        FVector Forward = GetActorForwardVector() * MovementVector.Y;
        FVector Right = GetActorRightVector() * MovementVector.X;
        FVector Up = GetActorUpVector() * MovementVector.Z; 

        FVector DesiredVelocity = (Forward + Right + Up) * CurrentSpeed;

        Velocity.X = DesiredVelocity.X;
        Velocity.Y = DesiredVelocity.Y;

        if (FMath::Abs(MovementVector.Z) > 0.1f)
        {
            Velocity.Z = DesiredVelocity.Z;
            bIsOnGround = false;
        }
    }
}

void APJPawn::Look(const FInputActionValue& Value)
{
    FVector LookVector = Value.Get<FVector>();
    float DeltaTime = GetWorld()->GetDeltaSeconds();

    if (Controller != nullptr)
    {
        FRotator DeltaRotation(
            LookVector.Y * RotationSpeed * DeltaTime * -1.0f,
            LookVector.X * RotationSpeed * DeltaTime,
            LookVector.Z * RotationSpeed * DeltaTime
        );

        AddActorLocalRotation(DeltaRotation);
    }
}