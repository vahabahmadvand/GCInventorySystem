// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/GCInventoryMappingDataAsset.h"
#include "Types/InventoryTypes.h"

#include "GCInventoryGISSubsystems.generated.h"

class APlayerState;

/**
 *
 */
UCLASS(config = Game, defaultconfig)
class GCINVENTORYSYSTEM_API UGCInventoryGISSubsystems : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UGCInventoryGISSubsystems();

	static UGCInventoryGISSubsystems* Get(const UObject* worldContextObject);

	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual void Deinitialize() override;

	/** Function to find the row of a table given its name. */
	template <class T>
	T* GetItemFromTag(const FGameplayTag& itemTag) const
	{
		const auto usedItemInfo = GetItemKeyInformationFromTag(itemTag);

		if (const auto dataAsset = ItemsDataAsset.LoadSynchronous())
		{
			if (const auto itemCategory = dataAsset->FindItemsDataTable(usedItemInfo.ItemCategoryTag))
			{
				return itemCategory->FindRow<T>(FName(*itemTag.ToString()), "");
			}
		}

		return nullptr;
	}

	UFUNCTION(BlueprintCallable, Category = InventorySubsystem, meta = (AutoCreateRefTerm = "itemTag"))
	FItemKeyInfo GetItemKeyInformationFromTag(const FGameplayTag& itemTag) const;

	UFUNCTION(BlueprintCallable, CustomThunk, Category = "InventorySubsystem", meta = (CustomStructureParam = "itemData", AutoCreateRefTerm = "itemTag", DisplayName = "Get Item Struct From Tag"))
	bool K2_GetItemStrcutFromTag(const FGameplayTag& itemTag, FTableRowBase& itemData);
	DECLARE_FUNCTION(execK2_GetItemStrcutFromTag);

	//~ Crafting related methods
	UFUNCTION(BlueprintCallable, Category = InventorySubsystem, meta = (AutoCreateRefTerm = "itemTag"))
	FItemRecipeElements GetItemRecipe(const FGameplayTag& itemTag);

protected:

	// Function in charge of filling the information for the AllItemsInventory map
	void InitializeItemsInformation();

	bool Generic_GetDataTableRowFromName(const UDataTable* Table, FName RowName, void* OutRowPtr);

	/*A data asset which link the fragment type (which is a gameplay tag) with a UScriptStruct.*/
	UPROPERTY(EditAnywhere, config, Category = Settings)
	TSoftObjectPtr<UGCInventoryMappingDataAsset> ItemsDataAsset;

private:

	// Map with all the existing items in the game (defined in the items data asset) with their key info
	TMap<FGameplayTag, FItemKeyInfo> AllItemsInventory;
};
