// Fill out your copyright notice in the Description page of Project Settings.


#include "RPSection.h"
#include "RPCharacter.h"
#include "RPItemBox.h"
#include "RPPlayerController.h"
#include "RPGameMode.h"

// Sets default values
ARPSection::ARPSection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	RootComponent = Mesh;

	FString AssetPath = TEXT("/Game/My/Mesh/SM_SQUARE.SM_SQUARE");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SQUARE(*AssetPath);
	if (SM_SQUARE.Succeeded())
	{
		Mesh->SetStaticMesh(SM_SQUARE.Object);
	}
	else
	{
		RPLOG(Error, TEXT("StaticMesh Loading Failed : %s"), *AssetPath);
	}

	Mesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 2.0f));

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Trigger->SetBoxExtent(FVector(775.0f, 775.0f, 250.0f));
	Trigger->SetupAttachment(RootComponent);
	Trigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	Trigger->SetCollisionProfileName(TEXT("RPTrigger"));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ARPSection::OnTriggerBeginOverlap);

	FString GateAssetPath = TEXT("/Game/My/Mesh/SM_GATE.SM_GATE");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_GATE(*GateAssetPath);
	if (!SM_GATE.Succeeded())
	{
		RPLOG(Error, TEXT("StaticMesh Loading Failed : %s"), *GateAssetPath);
	}

	static FName GateSockets[] = {
		{TEXT("+XGate")},
		{TEXT("-XGate")},
		{TEXT("+YGate")},
		{TEXT("-YGate")}
	};

	for (FName GateSocket : GateSockets)
	{
		RPCHECK(Mesh->DoesSocketExist(GateSocket));
		UStaticMeshComponent* NewGate = CreateDefaultSubobject<UStaticMeshComponent>(*GateSocket.ToString());
		NewGate->SetStaticMesh(SM_GATE.Object);
		NewGate->SetupAttachment(RootComponent, GateSocket);
		NewGate->SetRelativeLocation(FVector(0.0f, -80.5f, 0.0f));
		GateMeshes.Add(NewGate);

		UBoxComponent* NewGateTrigger = CreateDefaultSubobject<UBoxComponent>(*GateSocket.ToString()
			.Append(TEXT("Trigger")));
		NewGateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
		NewGateTrigger->SetupAttachment(RootComponent, GateSocket);
		NewGateTrigger->SetRelativeLocation(FVector(70.0f, 0.0f, 250.0f));
		NewGateTrigger->SetCollisionProfileName(TEXT("RPTrigger"));
		GateTriggers.Add(NewGateTrigger);

		NewGateTrigger->OnComponentBeginOverlap.AddDynamic(this, &ARPSection::OnGateTriggerBeginOverlap);
		NewGateTrigger->ComponentTags.Add(GateSocket);
	}

	bNoBattle = false;

	EnemySpawnTime = 3.0f;
	//ItemBoxSpawnTime = 1.0f;
}

void ARPSection::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);
	SetState(bNoBattle ? ESectionState::COMPLETE : ESectionState::READY);
}

// Called when the game starts or when spawned
void ARPSection::BeginPlay()
{
	Super::BeginPlay();
	
	SetState(bNoBattle ? ESectionState::COMPLETE : ESectionState::READY);
}

void ARPSection::SetState(ESectionState NewState)
{
	switch (NewState)
	{
	case ESectionState::READY:
		Trigger->SetCollisionProfileName(TEXT("RPTrigger"));
		for (UBoxComponent* GateTrigger : GateTriggers)
		{
			GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
		}
		OperateGates(true);
		break;

	case ESectionState::BATTLE:
		Trigger->SetCollisionProfileName(TEXT("NoCollision"));
		for (UBoxComponent* GateTrigger : GateTriggers)
		{
			GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
		}
		OperateGates(false);

		GetWorld()->GetTimerManager().SetTimer(
			SpawnNPCTimerHandle,
			FTimerDelegate::CreateUObject(this, &ARPSection::OnNPCSpawn),
			EnemySpawnTime, 
			false
		);

		//GetWorld()->GetTimerManager().SetTimer(
		//	SpawnItemBoxTimerHandle,
		//	FTimerDelegate::CreateLambda([this]() -> void {
		//		FVector2D RandXY = FMath::RandPointInCircle(600.0f);
		//		GetWorld()->SpawnActor<ARPItemBox>(GetActorLocation() + FVector(RandXY, 30.0f), FRotator::ZeroRotator);
		//	}),
		//	ItemBoxSpawnTime,
		//	false
		//);

		break;
	case ESectionState::COMPLETE:
		Trigger->SetCollisionProfileName(TEXT("NoCollision"));
		for (UBoxComponent* GateTrigger : GateTriggers)
		{
			GateTrigger->SetCollisionProfileName(TEXT("RPTrigger"));
		}
		OperateGates(true);
		break;
	default:
		break;
	}
}

void ARPSection::OperateGates(bool bOpen)
{
	for (UStaticMeshComponent* Gate : GateMeshes)
	{
		Gate->SetRelativeRotation(bOpen ? FRotator(0.0f, -90.0f, 0.0f) : FRotator::ZeroRotator);
	}
}

void ARPSection::OnTriggerBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (CurrentState == ESectionState::READY)
	{
		SetState(ESectionState::BATTLE);
	}
}

void ARPSection::OnGateTriggerBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	RPCHECK(OverlappedComponent->ComponentTags.Num() == 1);
	FName ComponentTag = OverlappedComponent->ComponentTags[0];
	FName SocketName = FName(*ComponentTag.ToString().Left(2));
	if (!Mesh->DoesSocketExist(SocketName)) return;

	FVector NewLocation = Mesh->GetSocketLocation(SocketName);

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);
	FCollisionObjectQueryParams ObjectQueryParam(FCollisionObjectQueryParams::InitType::AllObjects);

	bool bResult = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		NewLocation,
		FQuat::Identity,
		ObjectQueryParam,
		FCollisionShape::MakeSphere(775.0f),
		CollisionQueryParam
	);

	if (!bResult) { auto NewSection = GetWorld()->SpawnActor<ARPSection>(NewLocation, FRotator::ZeroRotator); }
	else { RPLOG(Warning, TEXT("NewSection is not empty")); }
}

void ARPSection::OnNPCSpawn()
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnNPCTimerHandle);
	auto KeyNPC = GetWorld()->SpawnActor<ARPCharacter>(GetActorLocation() + FVector::UpVector*88.0f, FRotator::ZeroRotator);
	if (nullptr != KeyNPC)
	{
		KeyNPC->OnDestroyed.AddDynamic(this, &ARPSection::OnKeyNPCDestroyed);
	}
}

void ARPSection::OnKeyNPCDestroyed(AActor * DestroyedActor)
{
	auto RPCharacter = Cast<ARPCharacter>(DestroyedActor);
	RPCHECK(nullptr != RPCharacter);

	auto RPPlayerController = Cast<ARPPlayerController>(RPCharacter->LastHitBy);
	RPCHECK(nullptr != RPPlayerController);

	auto RPGameMode = Cast<ARPGameMode>(GetWorld()->GetAuthGameMode());
	RPCHECK(nullptr != RPGameMode);
	RPGameMode->AddScore(RPPlayerController);

	SetState(ESectionState::COMPLETE);
}

