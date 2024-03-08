// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "System/GCGameplayTagStack.h"
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

USTRUCT(BlueprintType, Blueprintable)
struct FTestItemEntry : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemName = FText();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description = FText();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> ItemIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemClass = nullptr;
};