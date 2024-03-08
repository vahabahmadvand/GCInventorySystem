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
UCLASS(config = Engine, defaultconfig)
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
	FItemKeyInfo GetItemInformationFromTag(const FGameplayTag& itemTag);

	UFUNCTION(BlueprintCallable, Category = InventorySubsystem)
	void ItemAddedToInventory(const FGameplayTag& itemTag, const float itemStack, AActor* ownerReference);

	UFUNCTION(BlueprintCallable, Category = InventorySubsystem)
	void ItemUsedFromInventory(const FGameplayTag& itemTag, const float itemStack, AActor* ownerReference);

	UFUNCTION(BlueprintCallable, Category = InventorySubsystem)
	void ItemDroppedFromInventory(const FGameplayTag& itemTag, const float itemStack, AActor* ownerReference);

	UFUNCTION(BlueprintCallable, Category = InventorySubsystem)
	void ItemRemovedFromInventory(const FGameplayTag& itemTag, const float itemStack, AActor* ownerReference);

	//~ Crafting related methods
	UFUNCTION(BlueprintCallable, Category = InventorySubsystem)
	FItemRecipeElements GetItemRecipe(const FGameplayTag& itemTag);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = InventorySubsystem)
	UGCInventoryMappingDataAsset* GetItemDataAsset();

protected:

	// Function in charge of filling the information for the AllItemsInventory map
	void InitializeItemsInformation();

public:

	/*A data asset which link the fragment type (which is a gameplay tag) with a UScriptStruct.*/
	UPROPERTY(EditAnywhere, config, Category = Settings)
	TSoftObjectPtr<UGCInventoryMappingDataAsset> ItemsDataAsset;

private:

	// Map with all the existing items in the game (defined in the items data asset) with their key info
	TMap<FGameplayTag, FItemKeyInfo> AllItemsInventory;
};
