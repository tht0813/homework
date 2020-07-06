// Fill out your copyright notice in the Description page of Project Settings.


#include "RPWeapon.h"

// Sets default values
ARPWeapon::ARPWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	RootComponent = Weapon;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight'"));
	if (SK_WEAPON.Succeeded())
	{
		Weapon->SetSkeletalMesh(SK_WEAPON.Object);
	}

	Weapon->SetCollisionProfileName(TEXT("NoCollision"));

	AttackRange = 150.0f;

	AttackDamageMin = 2.5f;
	AttackDamageMax = 10.0f;
	AttackModifierMin = 1.0;
	AttackModifierMax = 1.25f;
}

// Called when the game starts or when spawned
void ARPWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	AttackDamage = FMath::RandRange(AttackDamageMin, AttackDamageMax);
	AttackModifier = FMath::RandRange(AttackModifierMin, AttackModifierMax);
}

float ARPWeapon::GetAttackRange() const
{
	return AttackRange;
}

float ARPWeapon::GetAttackDamage() const
{
	return AttackDamage;
}

float ARPWeapon::GetAttackModifier() const
{
	return AttackModifier;
}
