#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "FPSGameMode.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
    PawnSensingComponent->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
    PawnSensingComponent->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnPawnHeard);
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	OriginalRotation = GetActorRotation();
}

void AFPSAIGuard::OnPawnSeen(APawn* Pawn)
{
    if (Pawn == nullptr)
    {
        return;
    }

    AFPSGameMode* GameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->CompleteMission(Pawn, false);
    }
}

void AFPSAIGuard::OnPawnHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
    FVector Direction = Location - GetActorLocation();
    Direction.Normalize();

    FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
    NewLookAt.Pitch = 0.0f;
    NewLookAt.Roll = 0.0f;
    SetActorRotation(NewLookAt);

    GetWorldTimerManager().ClearTimer(TimeHandle_ResetOrientation);
    GetWorldTimerManager().SetTimer(TimeHandle_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, 3.0f);
}

void AFPSAIGuard::ResetOrientation()
{
    SetActorRotation(OriginalRotation);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
