// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "System/GameplayTagStack.h"
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

USTRUCT(BlueprintType)
struct FItemRecipeElements
{
	GENERATED_BODY()

	FItemRecipeElements() {}

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TMap<FGameplayTag, float> RecipeElements;
};

USTRUCT(BlueprintType)
struct FItemRecipeInfo
{
	GENERATED_BODY()

	FItemRecipeInfo() {}

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TMap<FGameplayTag, FItemRecipeElements> ItemRecipes;
};