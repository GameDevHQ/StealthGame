#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

class UPawnSensingComponent;

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


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
