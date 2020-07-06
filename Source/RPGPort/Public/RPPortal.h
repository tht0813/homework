// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGPort.h"
#include "GameFramework/Actor.h"
#include "RPPortal.generated.h"

UCLASS()
class RPGPORT_API ARPPortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARPPortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	UPROPERTY(VisibleAnywhere, Category = Portal)
	UBoxComponent* Trigger;

	UPROPERTY(VisibleAnywhere, Category = Portal)
	UStaticMeshComponent* Portal;

	UPROPERTY(VisibleAnyWhere, Category = Effect)
	UParticleSystemComponent* Effect;

	UPROPERTY(VisibleAnywhere, Category = Level)
	FString LevelName;

private:
	UFUNCTION()
	void OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
