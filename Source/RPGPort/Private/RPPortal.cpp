// Fill out your copyright notice in the Description page of Project Settings.


#include "RPPortal.h"
#include "RPCharacter.h"

// Sets default values
ARPPortal::ARPPortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));

	RootComponent = Trigger;
	Effect->SetupAttachment(RootComponent);

	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_PORTAL(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Ability/Summon/P_Summon_Portal.P_Summon_Portal'"));
	if (P_PORTAL.Succeeded())
	{
		Effect->SetTemplate(P_PORTAL.Object);
	}

	Effect->SetRelativeLocation(FVector(0.0f, 0.0f, -30.0f));

	Trigger->SetCollisionProfileName(TEXT("ItemBox"));

	LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	UE_LOG(LogTemp, Error, TEXT("%s"), *LevelName);
}

// Called when the game starts or when spawned
void ARPPortal::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARPPortal::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ARPPortal::OnCharacterOverlap);
}

void ARPPortal::OnCharacterOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	RPLOG_S(Warning);

	auto RPCharacter = Cast<ARPCharacter>(OtherActor);
	RPCHECK(nullptr != RPCharacter);

	if (nullptr != RPCharacter)
	{
		if (LevelName == "GamePlay")
		{
			UGameplayStatics::OpenLevel(this, TEXT("Test"));
		}
		else
		{
			UGameplayStatics::OpenLevel(this, TEXT("GamePlay"));
		}
	}
}
