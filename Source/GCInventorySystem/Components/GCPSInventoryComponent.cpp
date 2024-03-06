// Fill out your copyright notice in the Description page of Project Settings.

#include "GCPSInventoryComponent.h"
#include "Subsystems/GCInventoryGISSubsystems.h"
#include <GameFramework/PlayerState.h>
#include <Net/UnrealNetwork.h>

UGCPSInventoryComponent::UGCPSInventoryComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	HeldItemTags.Reset();

	SetIsReplicatedByDefault(true);
}

void UGCPSInventoryComponent::BeginPlay()
{
}

void UGCPSInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, HeldItemTags);
}

void UGCPSInventoryComponent::AddItemToInventory(FGameplayTag itemTag)
{
	const auto ownerPlayer = GetPlayerState<APlayerState>();

	HeldItemTags.AddTag(itemTag);
	OnItemGranted.Broadcast(itemTag, ownerPlayer);

	if (auto inventorySubsystem = UGCInventoryGISSubsystems::Get(ownerPlayer))
	{
		inventorySubsystem->ItemAddedToInventory(itemTag, ownerPlayer);
	}
}

void UGCPSInventoryComponent::UseItemFromInventory(FGameplayTag itemTag)
{
}

void UGCPSInventoryComponent::RemoveItemFromInventory(FGameplayTag itemTag)
{
}

void UGCPSInventoryComponent::DropItemFromInventory(FGameplayTag itemTag)
{
}