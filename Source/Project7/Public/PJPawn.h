#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "PJPawn.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class PROJECT7_API APJPawn : public APawn
{
    GENERATED_BODY()

public:
    APJPawn();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputMappingContext* DefaultContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* LookAction;


    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

private:

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UCapsuleComponent* CapsuleComp;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    USkeletalMeshComponent* MeshComp;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    USpringArmComponent* SpringArmComp;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UCameraComponent* CameraComp;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float MoveSpeed = 1500.0f;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float RotationSpeed = 100.0f;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float AirControlFactor = 0.4f; 

    UPROPERTY(EditAnywhere, Category = "Physics")
    float GravityZ = -980.0f;      

    FVector Velocity = FVector::ZeroVector;
    bool bIsOnGround = false;
}; 