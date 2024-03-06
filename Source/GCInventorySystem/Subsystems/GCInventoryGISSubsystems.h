// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "Types/InventoryTypes.h"

#include "GCInventoryGISSubsystems.generated.h"

class UGCInventoryMappingDataAsset;
class APlayerState;

/**
 *
 */
UCLASS(Config = Game)
class GCINVENTORYSYSTEM_API UGCInventoryGISSubsystems : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UGCInventoryGISSubsystems();

	static UGCInventoryGISSubsystems* Get(const UObject* worldContextObject);

	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = InventorySubsystem)
	FItemKeyInfo GetItemInformationFromTag(FGameplayTag itemTag);

	UFUNCTION(BlueprintCallable, Category = InventorySubsystem)
	void ItemAddedToInventory(FGameplayTag itemTag, float itemStack, APlayerState* playerReference);

	UFUNCTION(BlueprintCallable, Category = InventorySubsystem)
	void ItemUsedFromInventory(FGameplayTag itemTag, float itemStack, APlayerState* playerReference);

	UFUNCTION(BlueprintCallable, Category = InventorySubsystem)
	void ItemDroppedFromInventory(FGameplayTag itemTag, float itemStack, APlayerState* playerReference);

	UFUNCTION(BlueprintCallable, Category = InventorySubsystem)
	void ItemRemovedFromInventory(FGameplayTag itemTag, float itemStack, APlayerState* playerReference);

protected:

	// Function in charge of filling the information for the AllItemsInventory map
	void InitializeItemsInformation();

public:

	/*A data asset which link the fragment type (which is a gameplay tag) with a UScriptStruct.*/
	UPROPERTY(Config)
	TSoftObjectPtr<UGCInventoryMappingDataAsset> ItemsDataAsset;

private:

	// Map with all the existing items in the game (defined in the items data asset) with their key info
	TMap<FGameplayTag, FItemKeyInfo> AllItemsInventory;
};
