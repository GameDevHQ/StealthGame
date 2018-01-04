#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

class UPawnSensingComponent;

UENUM(BlueprintType)
enum class EAIState: uint8
{
    Idle,
    Suspicious,
    Alerted
};


UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UPawnSensingComponent* PawnSensingComponent;

    UFUNCTION()
    void OnPawnSeen(APawn* Pawn);

    UFUNCTION()
    void OnPawnHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);

    UFUNCTION()
    void ResetOrientation();

    UFUNCTION(BlueprintImplementableEvent, Category = "AI")
    void OnStateChanged(EAIState NewState);

    void SetCurrentState(EAIState NewState);

    FRotator OriginalRotation;
    FTimerHandle TimeHandle_ResetOrientation;
    EAIState CurrentState;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
