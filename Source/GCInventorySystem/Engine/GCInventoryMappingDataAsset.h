// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Types/InventoryTypes.h"
#include <Engine/DataAsset.h>

#include "GCInventoryMappingDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class GCINVENTORYSYSTEM_API UGCInventoryMappingDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UDataTable* FindItemsDataTable(const FGameplayTag& categoryTag) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FItemRecipeInfo FindRecipesCategory(const FGameplayTag& categoryTag) const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FGameplayTag, UDataTable*> ItemsCategoryMap;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FGameplayTag, FItemRecipeInfo> ItemsCategoryCraftingRecipes;
	
};
