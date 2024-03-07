// Fill out your copyright notice in the Description page of Project Settings.


#include "GCInventoryMappingDataAsset.h"
#include "Modules/GCInventorySystem.h"

UDataTable* UGCInventoryMappingDataAsset::FindItemsDataTable(const FGameplayTag& categoryTag) const
{
	if (const auto table = ItemsCategoryMap.Find(categoryTag))
	{
		return (*table);
	}

	UE_LOG(LogInventorySystem, Warning, TEXT("[%s] Could not find Items table for category: %s"), ANSI_TO_TCHAR(__FUNCTION__), *categoryTag.ToString());
	return nullptr;
}

FItemRecipeInfo UGCInventoryMappingDataAsset::FindRecipesCategory(const FGameplayTag& categoryTag) const
{
	if (auto recipes = ItemsCategoryCraftingRecipes.Find(categoryTag))
	{
		return *recipes;
	}

	UE_LOG(LogInventorySystem, Warning, TEXT("[%s] Could not find Items table for category: %s"), ANSI_TO_TCHAR(__FUNCTION__), *categoryTag.ToString());
	return FItemRecipeInfo();
}
