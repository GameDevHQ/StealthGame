// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSLaunchPad.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSLaunchPad::AFPSLaunchPad():
AppliedStrength(1500.f),
AppliedStrengthAngle(40.f)
{
    OverlapBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBoxComponent"));
    OverlapBoxComponent->SetBoxExtent(FVector(60.f, 60.f, 10.f));
    RootComponent = OverlapBoxComponent;

    OverlapBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::OverlapLaunchPad);

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(OverlapBoxComponent);
}

void AFPSLaunchPad::OverlapLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
                                     bool bFromSweep, const FHitResult& SweepResult)
{
    FRotator Direction = GetActorRotation();
    Direction.Add(AppliedStrengthAngle, 0.f, 0.f);
    FVector Velocity = Direction.Vector() * AppliedStrength;

    ACharacter* Character = Cast<ACharacter>(OtherActor);
    if (Character)
    {
        // Apply a force to a player
        Character->LaunchCharacter(Velocity, true, true);

        // And spawn some particle effect on the place where we step in
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystemEffect, GetActorLocation());
    }
    else if (OtherComp && OtherComp->IsSimulatingPhysics())
    {
        // Apply a force to an object
        OtherComp->AddImpulse(Velocity, NAME_None, true);

        // And spawn some particle effect on the place where we step in
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystemEffect, GetActorLocation());
    }
}
