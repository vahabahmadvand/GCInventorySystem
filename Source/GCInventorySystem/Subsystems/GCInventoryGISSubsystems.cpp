// Fill out your copyright notice in the Description page of Project Settings.

#include "GCInventoryGISSubsystems.h"
#include "Modules/GCInventorySystem.h"
#include <GameFramework/PlayerState.h>
#include <InstancedStruct.h>
#include <Kismet/DataTableFunctionLibrary.h>
#include <Engine/DataTable.h>

UGCInventoryGISSubsystems::UGCInventoryGISSubsystems()
{
	ItemsDataAsset = nullptr;
	AllItemsInventory.Empty();
}

UGCInventoryGISSubsystems* UGCInventoryGISSubsystems::Get(const UObject* worldContextObject)
{
	UGCInventoryGISSubsystems* InventorySubsystem = nullptr;

	if (const auto world = GEngine->GetWorldFromContextObject(worldContextObject, EGetWorldErrorMode::Assert))
	{
		InventorySubsystem = UGameInstance::GetSubsystem<UGCInventoryGISSubsystems>(world->GetGameInstance());
	}

	check(InventorySubsystem);

	return InventorySubsystem;
}

void UGCInventoryGISSubsystems::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);

	InitializeItemsInformation();
}

void UGCInventoryGISSubsystems::Deinitialize()
{
	Super::Deinitialize();
}

bool UGCInventoryGISSubsystems::K2_GetItemStrcutFromTag(const FGameplayTag& itemTag, FTableRowBase& itemData)
{
	// We should never hit this!  stubs to avoid NoExport on the class.
	check(0);
	return false;
}

DEFINE_FUNCTION(UGCInventoryGISSubsystems::execK2_GetItemStrcutFromTag)
{
	P_GET_STRUCT(FGameplayTag, itemTag);

	Stack.StepCompiledIn<FStructProperty>(NULL);
	void* itemData = Stack.MostRecentPropertyAddress;

	P_FINISH;
	bool bSuccess = false;

	FStructProperty* StructProp = CastField<FStructProperty>(Stack.MostRecentProperty);

	const auto usedItemInfo = P_THIS->GetItemKeyInformationFromTag(itemTag);

	if (const auto dataAsset = P_THIS->ItemsDataAsset.LoadSynchronous())
	{
		if (const auto itemCategory = dataAsset->FindItemsDataTable(usedItemInfo.ItemCategoryTag))
		{
			if (StructProp && itemData)
			{
				UScriptStruct* OutputType = StructProp->Struct;
				const UScriptStruct* TableType = itemCategory->GetRowStruct();

				const bool bCompatible = (OutputType == TableType) ||
					(OutputType->IsChildOf(TableType) && FStructUtils::TheSameLayout(OutputType, TableType));
				if (bCompatible)
				{
					P_NATIVE_BEGIN;
					bSuccess = P_THIS->Generic_GetDataTableRowFromName(itemCategory, FName(*itemTag.ToString()), itemData);
					P_NATIVE_END;
				}
				*(bool*)RESULT_PARAM = bSuccess;
			}
		}
	}
}

FItemRecipeElements UGCInventoryGISSubsystems::GetItemRecipe(const FGameplayTag& itemTag)
{
	if (const auto dataAsset = ItemsDataAsset.LoadSynchronous())
	{
		const auto usedItemInfo = GetItemKeyInformationFromTag(itemTag);

		const auto itemCategory = dataAsset->FindRecipesCategory(usedItemInfo.ItemCategoryTag);

		return itemCategory.ItemRecipes[itemTag];
	}
	else
	{
		UE_LOG(LogInventorySystem, Error, TEXT("[%s] Failed to find ItemsDataAsset. Cannot fill the item information."), ANSI_TO_TCHAR(__FUNCTION__));
	}

	return FItemRecipeElements();
}

FItemKeyInfo UGCInventoryGISSubsystems::GetItemKeyInformationFromTag(const FGameplayTag& itemTag) const
{
	if (AllItemsInventory.Contains(itemTag))
	{
		return AllItemsInventory[itemTag];
	}

	UE_LOG(LogInventorySystem, Warning, TEXT("[%s] Could not find the item with the tag: %s"), ANSI_TO_TCHAR(__FUNCTION__), *itemTag.ToString());

	return FItemKeyInfo();
}

void UGCInventoryGISSubsystems::InitializeItemsInformation()
{
	if (ensureMsgf(ItemsDataAsset.IsValid(), TEXT("Items data asset is not valid, without this file the system won't work. Please Fix it")))
	{
		if (const auto dataAsset = ItemsDataAsset.LoadSynchronous())
		{
			TArray<FGameplayTag> itemCategories;
			dataAsset->ItemsCategoryMap.GetKeys(itemCategories);

			if (itemCategories.Num() > 0)
			{
				for (const auto& categoryTag : itemCategories)
				{
					if (const auto itemCategory = dataAsset->FindItemsDataTable(categoryTag))
					{
						TArray<FName> tableRowNames;
						UDataTableFunctionLibrary::GetDataTableRowNames(itemCategory, tableRowNames);

						if (tableRowNames.Num() > 0)
						{
							for (const auto& itemNameTag : tableRowNames)
							{
								const auto itemTag = FGameplayTag::RequestGameplayTag(itemNameTag);
								FItemKeyInfo newItemInfo;
								newItemInfo.ItemTag = itemTag;
								newItemInfo.ItemCategoryTag = categoryTag;
								AllItemsInventory.Add(itemTag, newItemInfo);
							}
						}
						else
						{
							UE_LOG(LogInventorySystem, Warning, TEXT("[%s] Item Category info is empty. Please fill it with information."), ANSI_TO_TCHAR(__FUNCTION__));
						}
					}
					else
					{
						UE_LOG(LogInventorySystem, Error, TEXT("[%s] Failed to find the desired item category"), ANSI_TO_TCHAR(__FUNCTION__));
					}
				}
			}
			else
			{
				UE_LOG(LogInventorySystem, Warning, TEXT("[%s] Item Data Asset is empty. Please fill it with information."), ANSI_TO_TCHAR(__FUNCTION__));
			}
		}
		else
		{
			UE_LOG(LogInventorySystem, Error, TEXT("[%s] Failed to find ItemsDataAsset. Cannot fill the item information."), ANSI_TO_TCHAR(__FUNCTION__));
		}
	}
	else
	{
		UE_LOG(LogInventorySystem, Error, TEXT("[%s] Item Data asset reference is not valid"), ANSI_TO_TCHAR(__FUNCTION__));
	}
}

bool UGCInventoryGISSubsystems::Generic_GetDataTableRowFromName(const UDataTable* Table, FName RowName, void* OutRowPtr)
{
	bool bFoundRow = false;

	if (OutRowPtr && Table)
	{
		void* RowPtr = Table->FindRowUnchecked(RowName);

		if (RowPtr != nullptr)
		{
			const UScriptStruct* StructType = Table->GetRowStruct();

			if (StructType != nullptr)
			{
				StructType->CopyScriptStruct(OutRowPtr, RowPtr);
				bFoundRow = true;
			}
		}
	}

	return bFoundRow;
}