// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"
#include "GCInventoryInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGCInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class GCINVENTORYSYSTEM_API IGCInventoryInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// Logic that the owner will execute when the item is granted in the inventory
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ItemGranted(const FTableRowBase& itemRowStruct, float itemStack);

	// Logic that the owner will execute when the item is used from the inventory
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ItemUsed(const FTableRowBase& itemRowStruct, float itemStack);

	// Logic that the owner will execute when the item is removed from the inventory
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ItemRemoved(const FTableRowBase& itemRowStruct, float itemStack);

	// Logic that the owner will execute when an item is dropped from the inventory (Does not call Item Removed)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ItemDropped(const FTableRowBase& itemRowStruct, float itemStack);
};
