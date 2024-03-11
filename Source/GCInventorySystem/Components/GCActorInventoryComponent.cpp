// Fill out your copyright notice in the Description page of Project Settings.

#include "GCActorInventoryComponent.h"
#include "Interfaces/GCInventoryInterface.h"
#include "Subsystems/GCInventoryGISSubsystems.h"
#include <Net/UnrealNetwork.h>

UGCActorInventoryComponent::UGCActorInventoryComponent(const FObjectInitializer& ObjectInitializer)
{
	HeldItemTags = FGCGameplayTagStackContainer();
	StartUpItems.Empty();

	SetIsReplicatedByDefault(true);
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UGCActorInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (StartUpItems.Num() > 0)
	{
		for (const auto& currentItem : StartUpItems)
		{
			AddItemToInventory(currentItem.Key, currentItem.Value);
		}
	}
}

void UGCActorInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, HeldItemTags);
}

void UGCActorInventoryComponent::AddItemToInventory(FGameplayTag itemTag, float itemStack)
{
	const auto ownerActor = GetOwner();

	if (ownerActor && ownerActor->GetClass()->ImplementsInterface(UGCInventoryInterface::StaticClass()))
	{
		HeldItemTags.AddStack(itemTag, itemStack);

		IGCInventoryInterface::Execute_ItemGranted(ownerActor, itemTag, itemStack);

		OnItemGranted.Broadcast(itemTag, itemStack, ownerActor);
	}
}

void UGCActorInventoryComponent::UseItemFromInventory(FGameplayTag itemTag, float itemStack)
{
	const auto ownerActor = GetOwner();

	if (ownerActor && ownerActor->GetClass()->ImplementsInterface(UGCInventoryInterface::StaticClass()) && IsItemInInventory(itemTag))
	{
		IGCInventoryInterface::Execute_ItemUsed(ownerActor, itemTag, itemStack);

		OnItemUsed.Broadcast(itemTag, itemStack, ownerActor);
	}
}

void UGCActorInventoryComponent::DropItemFromInventory(FGameplayTag itemTag, float itemStack)
{
	const auto ownerActor = GetOwner();

	if (ownerActor && ownerActor->GetClass()->ImplementsInterface(UGCInventoryInterface::StaticClass()) && IsItemInInventory(itemTag))
	{
		HeldItemTags.RemoveStack(itemTag, itemStack);

		IGCInventoryInterface::Execute_ItemDropped(ownerActor, itemTag, itemStack);

		OnItemRemoved.Broadcast(itemTag, itemStack, ownerActor);
	}
}

void UGCActorInventoryComponent::DropAllItemsFromInventory()
{
	const auto heldItems = HeldItemTags.GetGameplayTagStackList();

	if (heldItems.Num() > 0)
	{
		for (const auto& itemStack : heldItems)
		{
			DropItemFromInventory(itemStack.GetGameplayTag(), itemStack.GetStackCount());
		}
	}
}

void UGCActorInventoryComponent::RemoveItemFromInventory(FGameplayTag itemTag, float itemStack)
{
	const auto ownerActor = GetOwner();

	if (ownerActor && ownerActor->GetClass()->ImplementsInterface(UGCInventoryInterface::StaticClass()) && IsItemInInventory(itemTag))
	{
		HeldItemTags.RemoveStack(itemTag, itemStack);

		IGCInventoryInterface::Execute_ItemRemoved(ownerActor, itemTag, itemStack);

		OnItemRemoved.Broadcast(itemTag, itemStack, ownerActor);
	}
}

void UGCActorInventoryComponent::RemoveAllItemsFromInventory()
{
	const auto heldItems = HeldItemTags.GetGameplayTagStackList();

	if (heldItems.Num() > 0)
	{
		for (const auto& itemStack : heldItems)
		{
			RemoveItemFromInventory(itemStack.GetGameplayTag(), itemStack.GetStackCount());
		}
	}
}

void UGCActorInventoryComponent::ClearInventory()
{
	HeldItemTags.ClearStack();
}

bool UGCActorInventoryComponent::IsItemInInventory(FGameplayTag itemTag) const
{
	return HeldItemTags.ContainsTag(itemTag);
}

float UGCActorInventoryComponent::GetItemStack(FGameplayTag itemTag) const
{
	return HeldItemTags.GetStackCount(itemTag);
}

TMap<FGameplayTag, float> UGCActorInventoryComponent::GetAllItemsOnInventory() const
{
	TMap<FGameplayTag, float> currentItemsMap;

	const auto currentItemsArray = HeldItemTags.GetGameplayTagStackList();

	if (currentItemsArray.Num() > 0)
	{
		for (const auto& itemStack : currentItemsArray)
		{
			currentItemsMap.Add(itemStack.GetGameplayTag(), itemStack.GetStackCount());
		}
	}

	return currentItemsMap;
}

void UGCActorInventoryComponent::CraftItem(FGameplayTag itemTag)
{
	const auto ownerActor = GetOwner();

	if (auto inventorySubsystem = UGCInventoryGISSubsystems::Get(ownerActor))
	{
		const auto itemRecipe = inventorySubsystem->GetItemRecipe(itemTag);

		if (IsItemCraftable(itemRecipe))
		{
			for (const auto& recipeElement : itemRecipe.RecipeElements)
			{
				RemoveItemFromInventory(recipeElement.Key, recipeElement.Value);
			}

			AddItemToInventory(itemTag, 1.f);
		}
	}
}

bool UGCActorInventoryComponent::CanItemBeCrafted(FGameplayTag itemTag)
{
	const auto ownerActor = GetOwner();

	if (auto inventorySubsystem = UGCInventoryGISSubsystems::Get(ownerActor))
	{
		const auto itemRecipe = inventorySubsystem->GetItemRecipe(itemTag);

		return IsItemCraftable(itemRecipe);
	}

	return false;
}

bool UGCActorInventoryComponent::IsItemCraftable(FItemRecipeElements recipe)
{
	bool bHasMaterials = true;

	if (recipe.RecipeElements.Num() > 0)
	{
		for (const auto& recipeElement : recipe.RecipeElements)
		{
			if (GetItemStack(recipeElement.Key) < recipeElement.Value)
			{
				bHasMaterials = false;
				break;
			}
		}
	}
	else
	{
		bHasMaterials = false;
	}

	return bHasMaterials;
}