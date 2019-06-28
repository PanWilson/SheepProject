// Fill out your copyright notice in the Description page of Project Settings.


#include "Corral.h"

// Sets default values
ACorral::ACorral()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("SheepDetection"));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComp->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);

	SheepCount = 0;

}

// Called when the game starts or when spawned
void ACorral::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACorral::AddSheep);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &ACorral::RemoveSheep);

}

// Called every frame
void ACorral::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ACorral::AddSheep(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SheepCount++;
	OnCountChanged();
}

void ACorral::RemoveSheep(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	SheepCount--;
	OnCountChanged();
}

void ACorral::OnCountChanged_Implementation()
{

}
