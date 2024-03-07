// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "GCGameplayTagStack.generated.h"

struct FGCGameplayTagStackContainer;
struct FNetDeltaSerializeInfo;

DECLARE_MULTICAST_DELEGATE(FOnStackItemReplicated);
DECLARE_DYNAMIC_DELEGATE(FDynamicOnStackItemReplicated);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStackItemAdded, const FGameplayTag& tag);

/**
 * Represents one stack of a gameplay tag (tag + count)
 */
USTRUCT(BlueprintType)
struct FGCGameplayTagStack : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FGCGameplayTagStack() {}

	FGCGameplayTagStack(FGameplayTag InTag, float InStackCount) : Tag(InTag), StackCount(InStackCount) {}

	FString GetDebugString() const;

	void PostReplicatedChange(const struct FGCGameplayTagStackContainer& InArraySerializer);

	FGameplayTag GetGameplayTag() const;

	float GetStackCount() const;

	FOnStackItemReplicated OnChanged;

private:

	friend FGCGameplayTagStackContainer;

	UPROPERTY()
	FGameplayTag Tag;

	UPROPERTY()
	float StackCount = 0.0f;
};

/** Container of gameplay tag stacks */
USTRUCT(BlueprintType)
struct FGCGameplayTagStackContainer : public FFastArraySerializer
{
	GENERATED_BODY()

	FGCGameplayTagStackContainer()
		//	: Owner(nullptr)
	{
	}

public:

	// Adds a specified number of stacks to the tag (does nothing if StackCount is below 1)
	void AddStack(FGameplayTag Tag, float StackCount);

	// Removes a specified number of stacks from the tag (does nothing if StackCount is below 1)
	void RemoveStack(FGameplayTag Tag, float StackCount);

	// Removes all the elements in the stack
	void ClearStack();

	TArray<FGCGameplayTagStack> GetGameplayTagStackList() const;

	// Returns the stack count of the specified tag (or 0 if the tag is not present)
	float GetStackCount(FGameplayTag Tag) const
	{
		return TagToCountMap.FindRef(Tag);
	}

	// Returns true if there is at least one stack of the specified tag
	bool ContainsTag(FGameplayTag Tag) const
	{
		return TagToCountMap.Contains(Tag);
	}

	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FGCGameplayTagStack, FGCGameplayTagStackContainer>(Stacks, DeltaParms, *this);
	}

	FGCGameplayTagStack* GetTagStackItem(const FGameplayTag& tag);

	void BindDelegateToStackReplicated(const FGameplayTag& tag, const FDynamicOnStackItemReplicated& onStackItemReplicated, const UObject* ownerUObject);

	FOnStackItemAdded OnStackItemAdded;

private:

	// Replicated list of gameplay tag stacks
	UPROPERTY()
	TArray<FGCGameplayTagStack> Stacks;

	// Accelerated list of tag stacks for queries
	TMap<FGameplayTag, float> TagToCountMap;
};

template<>
struct TStructOpsTypeTraits<FGCGameplayTagStackContainer> : public TStructOpsTypeTraitsBase2<FGCGameplayTagStackContainer>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};
