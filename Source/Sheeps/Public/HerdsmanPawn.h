// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "HerdsmanPawn.generated.h"

UCLASS()
class SHEEPS_API AHerdsmanPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AHerdsmanPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Herding")
	float HerdAreaRadius;

	UPROPERTY(EditAnywhere, Category = "Herding")
	float HerdingPower;

	//Overlap global variables
	TArray <TEnumAsByte<EObjectTypeQuery>> OverlapObjectTypes;
	TArray<AActor*> IgnoredActors;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
