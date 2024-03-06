// Fill out your copyright notice in the Description page of Project Settings.


#include "GCFragmentItemDataAsset.h"

bool UGCFragmentItemDataAsset::IsStructTypeSupported(FGameplayTag structTag)
{
	if (TagStructMap.Find(structTag))
	{
		return true;
	}

	return false;
}

UScriptStruct* UGCFragmentItemDataAsset::GetStructType(FGameplayTag structTag)
{
	if (const auto structType = TagStructMap.Find(structTag))
	{
		return structType->LoadSynchronous();
	}

	return nullptr;
}
