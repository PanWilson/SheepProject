// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "FlockMovementComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = Movement, meta = (BlueprintSpawnableComponent))
class SHEEPS_API UFlockMovementComponent : public UMovementComponent
{
	GENERATED_BODY()
	
public:
	UFlockMovementComponent();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Flocking")
	float AligmentWeight;

	UPROPERTY(EditAnywhere, Category = "Flocking")
	float CohesiontWeight;

	UPROPERTY(EditAnywhere, Category = "Flocking")
	float SeparationWeight;

	UPROPERTY(EditAnywhere, Category = "Flocking")
	float AvoidanceWeight;

	UPROPERTY(EditAnywhere, Category = "Flocking")
	float NeighbourAreaRadius;

	UPROPERTY(EditAnywhere, Category = "Flocking")
	float AvoidanceAreaRadius;

	UPROPERTY(EditAnywhere, Category = "Flocking")
	float MaxFlockingVelocity;

	UPROPERTY(EditAnywhere, Category = "Flocking")
	bool RotateTowardVelocity;

	UPROPERTY(EditAnywhere, Category = "Flocking")
	float RotationSpeed;

	UPROPERTY(EditAnywhere, Category = "Flocking")
	float MinVelocityToRotate;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool DebugMode;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool StartWithRandomVelocity;

	UPROPERTY(EditAnywhere, Category = "Debug")
	float StartingVelocity;

	FVector CalculateAlignment(const TArray<AActor*>& Neighbours);

	//Returns vector toward average positions of Neighbours
	FVector CalculateCohesion(const TArray<AActor*>& Neighbours);

	FVector CalculateSeparation(const TArray<AActor*>& Neighbours);

	FVector CalculateAvoidance();

	UPrimitiveComponent* OwnerPrimitiveBody;

	//Overlap global variables
	TArray <TEnumAsByte<EObjectTypeQuery>> OverlapObjectTypes;

	TArray<AActor*> IgnoredActors;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};
