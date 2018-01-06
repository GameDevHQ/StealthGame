#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

class UPawnSensingComponent;

const float DISTANCE_EPSILON = 50;


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

    UFUNCTION()
    void OnRep_GuardState();

    FRotator OriginalRotation;
    FTimerHandle TimeHandle_ResetOrientation;
   
    UPROPERTY(ReplicatedUsing=OnRep_GuardState)
    EAIState CurrentState;

    UPROPERTY(EditInstanceOnly, Category = "AI")
    bool IsPatroller;

    UPROPERTY(EditInstanceOnly, Category = "Patrolling", meta = (EditCondition = "IsPatroller"))
    AActor* StartPatrollingPoint;
    
    UPROPERTY(EditInstanceOnly, Category = "Patrolling", meta = (EditCondition = "IsPatroller"))
    AActor* EndPatrollingPoint;

    AActor* CurrentPatrollingPoint;

    void MoveToNextPatrolPoint();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
