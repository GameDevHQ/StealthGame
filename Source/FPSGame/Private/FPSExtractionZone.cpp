// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
    OverlapComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComponent"));
    OverlapComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    OverlapComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    OverlapComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
    OverlapComponent->SetBoxExtent(FVector(200.f));
    RootComponent = OverlapComponent;

    OverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap);

    DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
    DecalComponent->DecalSize = FVector(200.f, 200.f, 200.f);
    DecalComponent->SetupAttachment(OverlapComponent);
}

void AFPSExtractionZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
                                       bool bFromSweep, const FHitResult& SweepResult)
{
    AFPSCharacter* Character = Cast<AFPSCharacter>(OtherActor);

    if (Character == nullptr)
    {
        return;
    }

    if (Character->bIsCarryingObjective)
    {
        AFPSGameMode* GameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->CompleteMission(Character);
        }
    }
    else
    {
        UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound);
    }
}
