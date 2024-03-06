// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/PlayerStateComponent.h"
#include "Types/InventoryTypes.h"

#include "GCPSInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemGranted, FGameplayTag, itemGranted, APlayerState*, playerReference);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemUsed, FGameplayTag, itemGranted, APlayerState*, playerReference);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemDroped, FGameplayTag, itemGranted, APlayerState*, playerReference);

/**
 *  Inventory component used to manage the inventory of players during the game.
 *  It covers the basic functions for items and requires that the player implements the GCInventoryInterface,
 *  to apply the desired effects of the Items.
 */
UCLASS()
class GCINVENTORYSYSTEM_API UGCPSInventoryComponent : public UPlayerStateComponent
{
	GENERATED_BODY()

public:

	UGCPSInventoryComponent(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "InventoryComponent")
	void AddItemToInventory(FGameplayTag itemTag);

	UFUNCTION(BlueprintCallable, Category = "InventoryComponent")
	void UseItemFromInventory(FGameplayTag itemTag);

	UFUNCTION(BlueprintCallable, Category = "InventoryComponent")
	void RemoveItemFromInventory(FGameplayTag itemTag);

	UFUNCTION(BlueprintCallable, Category = "InventoryComponent")
	void DropItemFromInventory(FGameplayTag itemTag);

public:

	UPROPERTY(BlueprintCallable)
	FOnItemGranted OnItemGranted;

	UPROPERTY(BlueprintCallable)
	FOnItemUsed OnItemUsed;

	UPROPERTY(BlueprintCallable)
	FOnItemDroped OnItemDroped;

protected:

	// Gameplay tags of the items that the player holds
	UPROPERTY(Replicated)
	FGameplayTagContainer HeldItemTags;
};
