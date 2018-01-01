// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSBlackHole.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RootComponent = MeshComponent;

    InnerSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InnerSphereComponent"));
    InnerSphereComponent->SetSphereRadius(200.f);
    InnerSphereComponent->SetupAttachment(MeshComponent);
    InnerSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::OverlapInnerSphere);

    OuterSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("OuterSphereComponent"));
    OuterSphereComponent->SetSphereRadius(1500.f);
    OuterSphereComponent->SetupAttachment(MeshComponent);
}

// Called when the game starts or when spawned
void AFPSBlackHole::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSBlackHole::OverlapInnerSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
                                       bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor)
    {
        OtherActor->Destroy();
    }
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    TArray<UPrimitiveComponent*> OverlappedObjects;
    OuterSphereComponent->GetOverlappingComponents(OverlappedObjects);

    for (int i = 0; i < OverlappedObjects.Num(); ++i)
    {
        auto object = OverlappedObjects[i];

        // We can apply a force only for object with enable "Simulate physics" option
        if (object && object->IsSimulatingPhysics())
        {
            float SphereRadius = OuterSphereComponent->GetScaledSphereRadius();
            float Strength = -3000.f;
            object->AddRadialForce(GetActorLocation(), SphereRadius, Strength, ERadialImpulseFalloff::RIF_Linear, true);
        }
    }
}
