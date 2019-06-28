// Fill out your copyright notice in the Description page of Project Settings.


#include "HerdsmanPawn.h"
#include "Kismet\KismetSystemLibrary.h"
#include "Engine\World.h"
#include "Components\PrimitiveComponent.h"

// Sets default values
AHerdsmanPawn::AHerdsmanPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	HerdAreaRadius = 400.f;
	HerdingPower = 100000.f;
}

// Called when the game starts or when spawned
void AHerdsmanPawn::BeginPlay()
{
	Super::BeginPlay();

	OverlapObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));
	IgnoredActors.Add(this);
	
}

// Called every frame
void AHerdsmanPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UWorld* myWorld = GetWorld();
	
	if (myWorld)
	{
		TArray<UPrimitiveComponent*> OutComponents;

		bool anySheeps = UKismetSystemLibrary::SphereOverlapComponents(
			myWorld,
			GetActorLocation(),
			HerdAreaRadius,
			OverlapObjectTypes,
			nullptr,
			IgnoredActors,
			OutComponents);

		if (anySheeps)
		{

			for (int32 i = 0; i < OutComponents.Num(); i++)
			{

				float distance = FVector::DistSquared(OutComponents[i]->GetComponentLocation(), GetActorLocation());

				FVector ForceV = OutComponents[i]->GetComponentLocation() - GetActorLocation();

				if (distance != 0.f)
				{
					ForceV /= distance;
					ForceV *= HerdingPower;
					ForceV -= OutComponents[i]->GetComponentVelocity();

					OutComponents[i]->AddForce(ForceV, NAME_None, true);

				}
			}
		}
	}

}

// Called to bind functionality to input
void AHerdsmanPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

