// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Engine/DataAsset.h>
#include <GameplayTagContainer.h>

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FGameplayTag, UDataTable*> ItemsCategoryMap;
	
};
