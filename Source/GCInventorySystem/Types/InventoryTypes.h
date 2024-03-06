// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameplayTagContainer.h>
#include "InventoryTypes.generated.h"

USTRUCT(BlueprintType)
struct FItemKeyInfo
{
	GENERATED_BODY()

	FItemKeyInfo() {}

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FName ItemName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FGameplayTag ItemTag;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FGameplayTag ItemCategoryTag;
};