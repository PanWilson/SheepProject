// Fill out your copyright notice in the Description page of Project Settings.


#include <Sheeps/Public/FlockMovementComponent.h>
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "Sheeps.h"


// Sets default values for this component's properties
UFlockMovementComponent::UFlockMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	AligmentWeight = 1.f;
	CohesiontWeight = 1.f;
	SeparationWeight = 1.f;
	AvoidanceWeight = 1.f;

	NeighbourAreaRadius = 100.f;
	AvoidanceAreaRadius = 400.f;
	MaxFlockingVelocity = 200.f;
	DebugMode = false;
	StartWithRandomVelocity = false;
	StartingVelocity = 5000.f;
	RotateTowardVelocity = true;
	RotationSpeed = 10.f;
	MinVelocityToRotate = 10.f;
}


// Called when the game starts
void UFlockMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OverlapObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));

	IgnoredActors.Add(GetOwner());

	OwnerPrimitiveBody = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());

	if (StartWithRandomVelocity && OwnerPrimitiveBody)
	{
		float direction = FMath::RandRange(0.f, 360.f);
		FVector force = GetOwner()->GetActorForwardVector();
		force = force.RotateAngleAxis(direction, GetOwner()->GetActorUpVector());
		OwnerPrimitiveBody->AddForce(force * StartingVelocity, NAME_None, true);
	}
}


// Called every frame
void UFlockMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	if (ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(UpdatedComponent))
	{
		return;
	}

	UWorld* myWorld = GetWorld();
	if (OwnerPrimitiveBody && myWorld)
	{
		TArray<AActor*> OutActors;

		if (DebugMode)
		{
			DrawDebugSphere(myWorld,
				GetOwner()->GetActorLocation(),
				NeighbourAreaRadius,
				12,
				FColor::Red,
				false,
				-1.f,
				0,
				1.f);
		}

		bool anyNeighbours = UKismetSystemLibrary::SphereOverlapActors(
			myWorld,
			GetOwner()->GetActorLocation(),
			NeighbourAreaRadius,
			OverlapObjectTypes,
			nullptr,
			IgnoredActors,
			OutActors);

		FVector FinalVector = FVector(0.f);
		FVector AligmentV = FVector(0.f);
		FVector SeparationV = FVector(0.f);
		FVector CohesionV = FVector(0.f);
		FVector AvoidanceV = FVector(0.f);

		if (anyNeighbours)
		{
			AligmentV = CalculateAlignment(OutActors);
			SeparationV = CalculateSeparation(OutActors);
			CohesionV = CalculateCohesion(OutActors);
		}

		AvoidanceV = CalculateAvoidance();
		FinalVector += (AligmentWeight * AligmentV) + (SeparationWeight * SeparationV) + (CohesiontWeight * CohesionV) + (AvoidanceWeight * AvoidanceV);

		FinalVector = FinalVector.GetClampedToMaxSize(MaxFlockingVelocity);

		if (DebugMode)
		{
			FVector arrowStart = GetOwner()->GetActorLocation();

			DrawDebugDirectionalArrow(myWorld,
				arrowStart,
				arrowStart + FinalVector,
				1.f,
				FColor::Blue,
				false,
				-1.f,
				0,
				5.f);
		}

		OwnerPrimitiveBody->AddForce(FinalVector, NAME_None, true);


		if (RotateTowardVelocity)
		{
			FVector velocityV = GetOwner()->GetVelocity();
			if (velocityV.Size() > MinVelocityToRotate)
			{

				velocityV.Normalize();
				FVector projectedVelocity = FVector::VectorPlaneProject(velocityV, GetOwner()->GetActorUpVector());

				float rotationDirection = FVector::DotProduct(projectedVelocity, GetOwner()->GetActorRightVector());
				FRotator DeltaRotation = FRotator(0.f, rotationDirection * RotationSpeed, 0.f);

				FQuat FinalRotation = DeltaRotation.Quaternion() * GetOwner()->GetActorRotation().Quaternion();

				FVector DeltaLocation = FVector::ZeroVector;
				MoveUpdatedComponent(DeltaLocation, FinalRotation, true);
			}
		}

	}

}


FVector UFlockMovementComponent::CalculateAlignment(const TArray<AActor*>& Neighbours)
{
	FVector AligmentV = FVector(0.f);

	if (Neighbours.Num() > 0)
	{

		for (int32 i = 0; i < Neighbours.Num(); i++)
		{

			AligmentV += Neighbours[i]->GetVelocity();

		}

		AligmentV /= Neighbours.Num();
	}

	return AligmentV;
}


FVector UFlockMovementComponent::CalculateCohesion(const TArray<AActor*>& Neighbours)
{
	FVector CohesionV = FVector(0.f);

	FVector averageLocation = UGameplayStatics::GetActorArrayAverageLocation(Neighbours);

	CohesionV = averageLocation - GetOwner()->GetActorLocation();

	float distance = CohesionV.Size();

	if ((NeighbourAreaRadius - distance) != 0)
	{
		//Cohesion is proportional to distance
		CohesionV /= FMath::Pow(NeighbourAreaRadius - distance, 2.f);
	}

	return CohesionV;
}


FVector UFlockMovementComponent::CalculateSeparation(const TArray<AActor*>& Neighbours)
{
	FVector SeparationV = FVector(0.f);

	if (Neighbours.Num() > 0)
	{

		for (int32 i = 0; i < Neighbours.Num(); i++)
		{
			FVector direction = GetOwner()->GetActorLocation() - Neighbours[i]->GetActorLocation();
			float distance = direction.Size();

			if (distance != 0)
			{
				//Separation is inversely proportional to distance
				direction /= FMath::Pow(distance, 2.f);
			}

			SeparationV += direction;

		}

		SeparationV /= Neighbours.Num();

	}

	FVector arrowStart = GetOwner()->GetActorLocation();

	DrawDebugDirectionalArrow(GetWorld(),
		arrowStart,
		arrowStart + SeparationV,
		1.f,
		FColor::Red,
		false,
		-1.f,
		0,
		5.f);

	return SeparationV;
}

FVector UFlockMovementComponent::CalculateAvoidance()
{

	UWorld* myWorld = GetWorld();
	FVector AvoidanceV = FVector(0.f);

	if (myWorld)
	{
		TArray<FOverlapResult> OutOverlaps;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GetOwner());

		GetWorld()->OverlapMultiByChannel(
			OutOverlaps,
			GetOwner()->GetActorLocation(),
			FQuat::Identity,
			TraceChanel_AVOIDANCE,
			FCollisionShape::MakeSphere(AvoidanceAreaRadius),
			QueryParams);

		for (int32 i = 0; i < OutOverlaps.Num(); i++)
		{
			FVector OutPoint;
			float sqrtDistance = 0.f;
			OutOverlaps[i].GetComponent()->GetSquaredDistanceToCollision(GetOwner()->GetActorLocation(), sqrtDistance, OutPoint);

			if (sqrtDistance != 0.f)
			{
				AvoidanceV += (GetOwner()->GetActorLocation() - OutPoint) / sqrtDistance;
			}
		}

	}

	return AvoidanceV;
}
