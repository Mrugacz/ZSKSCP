// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SCPAmmoComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
USCPAmmoComponent::USCPAmmoComponent()
{
	AmmoCount = 0.0f;
	AmmoInClip = 0.0f;

	SetIsReplicated(true);
}


// Called when the game starts
void USCPAmmoComponent::BeginPlay()
{
	Super::BeginPlay();

	//@TODO: Begin game with full clip
}

void USCPAmmoComponent::OnRep_Ammo()
{
	OnAmmoChanged.Broadcast(this, AmmoCount, AmmoInClip);
}

void USCPAmmoComponent::HandleAmmoChanges(AActor* ChangedAmmoActor, float OwnerAmmoCount, float OwnerAmmoInClip, float MaxAmmo)
{
	// Update ammo clamped
	OwnerAmmoCount = FMath::Clamp(OwnerAmmoCount, 0.0f, MaxAmmo);

	UE_LOG(LogTemp, Log, TEXT("Ammo changed %s, %s in clip"), *FString::SanitizeFloat(OwnerAmmoCount), *FString::SanitizeFloat(OwnerAmmoInClip));

	OnAmmoChanged.Broadcast(this, OwnerAmmoCount, OwnerAmmoInClip);
}

bool USCPAmmoComponent::Reload(float AmmoPerClip)
{
	if (AmmoCount <= 0.0f)
	{
		return false;
	}

	AmmoCount = FMath::Max(AmmoCount - AmmoPerClip, 0.0f);
	AmmoInClip = AmmoPerClip;

	UE_LOG(LogTemp, Log, TEXT("Ammo changed %s (-%s)"), *FString::SanitizeFloat(AmmoCount), *FString::SanitizeFloat(AmmoPerClip));

	OnAmmoChanged.Broadcast(this, AmmoCount, AmmoInClip);

	return true;
}

void USCPAmmoComponent::AddAmmo(float AmmoToAdd, float MaxAmmo)
{
	if (AmmoToAdd <= 0.0f)
	{
		return;
	}

	AmmoCount = FMath::Clamp(AmmoCount + AmmoToAdd, 0.0f, MaxAmmo);

	UE_LOG(LogTemp, Log, TEXT("Ammo changed %s (+%s)"), *FString::SanitizeFloat(AmmoCount), *FString::SanitizeFloat(AmmoToAdd));

	OnAmmoChanged.Broadcast(this, AmmoCount, AmmoInClip);
}

void USCPAmmoComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USCPAmmoComponent, AmmoCount);
	DOREPLIFETIME(USCPAmmoComponent, AmmoInClip);
}