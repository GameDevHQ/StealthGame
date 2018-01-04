// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn, bool bMissionSuccess)
{
    if (InstigatorPawn)
    {
        InstigatorPawn->DisableInput(nullptr);
        
        if (SpectatingViewpointClass)
        {
            TArray<AActor*> AvailableActors;
            UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, AvailableActors);

            // Change view target to any valid actor found
            if (AvailableActors.Num() > 0)
            {
                AActor* SpectatingViewpoint = AvailableActors[0];
                APlayerController* Controller = Cast<APlayerController>(InstigatorPawn->GetController());
                if (Controller)
                {
                    Controller->SetViewTargetWithBlend(SpectatingViewpoint, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("SpectactingViewpointClass is null. Please, go to the GameMode class and change to an appropriate class instead."));
        }
    }

    OnMissionCompleted(InstigatorPawn, bMissionSuccess);
}
