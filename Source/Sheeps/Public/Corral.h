// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Corral.generated.h"


UCLASS()
class SHEEPS_API ACorral : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACorral();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Corral")
	UBoxComponent* BoxComp;

	UPROPERTY(BlueprintReadOnly, Category = "Corral")
	int32 SheepCount;

	UFUNCTION()
	void AddSheep(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void RemoveSheep(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintNativeEvent, Category = "Corral")
	void OnCountChanged();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
