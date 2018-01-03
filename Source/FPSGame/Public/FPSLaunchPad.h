#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSLaunchPad.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UParticleSystem;

UCLASS()
class FPSGAME_API AFPSLaunchPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSLaunchPad();

protected:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UBoxComponent* OverlapBoxComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Launchpad Settings")
    float AppliedStrength;

    UPROPERTY(EditDefaultsOnly, Category = "Launchpad Settings")
    float AppliedStrengthAngle;

    UPROPERTY(EditDefaultsOnly, Category = "Launchpad Settings")
    UParticleSystem* ParticleSystemEffect;

    UFUNCTION()
    void OverlapLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                          bool bFromSweep, const FHitResult& SweepResult);
};
