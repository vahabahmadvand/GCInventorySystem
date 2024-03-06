// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTags.h"
#include "GCFragmentItemDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GCINVENTORYSYSTEM_API UGCFragmentItemDataAsset : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, TSoftObjectPtr<UScriptStruct>> TagStructMap;

public:

	bool IsStructTypeSupported(FGameplayTag structTag);

	UScriptStruct* GetStructType(FGameplayTag structTag);
	
};
