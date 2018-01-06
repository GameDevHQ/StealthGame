#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Net/UnrealNetwork.h"
#include "FPSGameMode.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard():
CurrentState(EAIState::Idle)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
    PawnSensingComponent->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
    PawnSensingComponent->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnPawnHeard);

    SetReplicates(true);
    SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	OriginalRotation = GetActorRotation();

    if (IsPatroller)
    {
        if (StartPatrollingPoint != nullptr && EndPatrollingPoint != nullptr)
        {
            MoveToNextPatrolPoint();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("StartPatrollingPoint and EndPatrollingPoint must be set."));
        }
    }
}

void AFPSAIGuard::OnPawnSeen(APawn* Pawn)
{
    if (Pawn == nullptr)
    {
        return;
    }

    // The player is in front of AI. Don't moving anywhere.
    AController* AIController = Cast<AController>(GetController());
    if (AIController)
    {
        AIController->StopMovement();
    }

    // Switch to game over state.
    AFPSGameMode* GameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->CompleteMission(Pawn, false);
    }

    SetCurrentState(EAIState::Alerted);
}

void AFPSAIGuard::OnPawnHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
    if (CurrentState == EAIState::Alerted)
    {
        return;
    }

    // The player probably is near to AI. Stop an actual movement to a patrolling point.
    AController* AIController = Cast<AController>(GetController());
    if (AIController)
    {
        AIController->StopMovement();
    }

    FVector Direction = Location - GetActorLocation();
    Direction.Normalize();

    FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
    NewLookAt.Pitch = 0.0f;
    NewLookAt.Roll = 0.0f;
    SetActorRotation(NewLookAt);

    // Turn AI to the source of a sound.
    GetWorldTimerManager().ClearTimer(TimeHandle_ResetOrientation);
    GetWorldTimerManager().SetTimer(TimeHandle_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, 3.0f);

    // Set the new state for AI.
    if (CurrentState != EAIState::Alerted)
    {
        SetCurrentState(EAIState::Suspicious);
    }
}

void AFPSAIGuard::ResetOrientation()
{
    if (CurrentState == EAIState::Alerted)
    {
        return;
    }

    SetActorRotation(OriginalRotation);
    SetCurrentState(EAIState::Idle);

    if (IsPatroller && CurrentPatrollingPoint)
    {
        UNavigationSystem::SimpleMoveToActor(GetController(), CurrentPatrollingPoint);
    }
}

void AFPSAIGuard::SetCurrentState(EAIState NewState)
{
    if (CurrentState == NewState)
    {
        return;
    }

    CurrentState = NewState;
    OnRep_GuardState();
}

void AFPSAIGuard::OnRep_GuardState()
{
    OnStateChanged(CurrentState);
}

void AFPSAIGuard::MoveToNextPatrolPoint()
{
    if (CurrentPatrollingPoint == nullptr || CurrentPatrollingPoint == EndPatrollingPoint)
    {
        CurrentPatrollingPoint = StartPatrollingPoint;
    }
    else
    {
        CurrentPatrollingPoint = EndPatrollingPoint;
    }

    UNavigationSystem::SimpleMoveToActor(GetController(), CurrentPatrollingPoint);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (IsPatroller && CurrentPatrollingPoint)
    {
        // If the we are close to the patrolling point, then go to an another.
        FVector Distance = GetActorLocation() - CurrentPatrollingPoint->GetActorLocation();

        if (Distance.Size() < DISTANCE_EPSILON)
        {
            MoveToNextPatrolPoint();
        }
    }
}


void AFPSAIGuard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AFPSAIGuard, CurrentState);
}
