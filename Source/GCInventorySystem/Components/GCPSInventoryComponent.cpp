// Fill out your copyright notice in the Description page of Project Settings.

#include "GCPSInventoryComponent.h"
#include "Subsystems/GCInventoryGISSubsystems.h"
#include <GameFramework/PlayerState.h>
#include <Net/UnrealNetwork.h>

UGCPSInventoryComponent::UGCPSInventoryComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	HeldItemTags = FGameplayTagStackContainer();
	StartUpItems.Empty();

	SetIsReplicatedByDefault(true);
}

void UGCPSInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (StartUpItems.Num() > 0)
	{
		for (const auto currentItem : StartUpItems)
		{
			AddItemToInventory(currentItem.Key, currentItem.Value);
		}
	}
}

void UGCPSInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, HeldItemTags);
}

void UGCPSInventoryComponent::AddItemToInventory(FGameplayTag itemTag, float itemStack)
{
	const auto ownerPlayer = GetPlayerState<APlayerState>();

	HeldItemTags.AddStack(itemTag, itemStack);

	if (auto inventorySubsystem = UGCInventoryGISSubsystems::Get(ownerPlayer))
	{
		inventorySubsystem->ItemAddedToInventory(itemTag, itemStack, ownerPlayer);
	}

	OnItemGranted.Broadcast(itemTag, ownerPlayer);
}

void UGCPSInventoryComponent::UseItemFromInventory(FGameplayTag itemTag, float itemStack)
{
	const auto ownerPlayer = GetPlayerState<APlayerState>();

	if (IsItemInInventory(itemTag))
	{
		if (auto inventorySubsystem = UGCInventoryGISSubsystems::Get(ownerPlayer))
		{
			inventorySubsystem->ItemUsedFromInventory(itemTag, itemStack, ownerPlayer);
		}

		OnItemUsed.Broadcast(itemTag, ownerPlayer);
	}
}

void UGCPSInventoryComponent::DropItemFromInventory(FGameplayTag itemTag, float itemStack)
{
	const auto ownerPlayer = GetPlayerState<APlayerState>();

	if (IsItemInInventory(itemTag))
	{
		HeldItemTags.RemoveStack(itemTag, itemStack);

		if (auto inventorySubsystem = UGCInventoryGISSubsystems::Get(ownerPlayer))
		{
			inventorySubsystem->ItemDroppedFromInventory(itemTag, itemStack, ownerPlayer);
		}

		OnItemRemoved.Broadcast(itemTag, ownerPlayer);
	}
}

void UGCPSInventoryComponent::DropAllItemsFromInventory()
{
	const auto heldItems = HeldItemTags.GetGameplayTagStackList();

	if (heldItems.Num() > 0)
	{
		for (const auto itemStack : heldItems)
		{
			DropItemFromInventory(itemStack.GetGameplayTag(), itemStack.GetStackCount());
		}
	}
}

void UGCPSInventoryComponent::RemoveItemFromInventory(FGameplayTag itemTag, float itemStack)
{
	const auto ownerPlayer = GetPlayerState<APlayerState>();

	if (IsItemInInventory(itemTag))
	{
		HeldItemTags.RemoveStack(itemTag, itemStack);

		if (auto inventorySubsystem = UGCInventoryGISSubsystems::Get(ownerPlayer))
		{
			inventorySubsystem->ItemRemovedFromInventory(itemTag, itemStack, ownerPlayer);
		}

		OnItemRemoved.Broadcast(itemTag, ownerPlayer);
	}
}

void UGCPSInventoryComponent::RemoveAllItemsFromInventory()
{
	const auto heldItems = HeldItemTags.GetGameplayTagStackList();

	if (heldItems.Num() > 0)
	{
		for (const auto itemStack : heldItems)
		{
			RemoveItemFromInventory(itemStack.GetGameplayTag(), itemStack.GetStackCount());
		}
	}
}

void UGCPSInventoryComponent::ClearInventory()
{
	HeldItemTags.ClearStack();
}

bool UGCPSInventoryComponent::IsItemInInventory(FGameplayTag itemTag) const
{
	return HeldItemTags.ContainsTag(itemTag);
}

float UGCPSInventoryComponent::GetItemStack(FGameplayTag itemTag) const
{
	return HeldItemTags.GetStackCount(itemTag);
}