// Copyright Epic Games, Inc. All Rights Reserved.

#include "GCGameplayTagStack.h"

#include "UObject/Stack.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GCGameplayTagStack)

//////////////////////////////////////////////////////////////////////
// FGCGameplayTagStack

FString FGCGameplayTagStack::GetDebugString() const
{
	return FString::Printf(TEXT("%sx%d"), *Tag.ToString(), StackCount);
}

void FGCGameplayTagStack::PostReplicatedChange(const struct FGCGameplayTagStackContainer& InArraySerializer)
{
	//OnChanged.Broadcast();
}

FGameplayTag FGCGameplayTagStack::GetGameplayTag() const
{
	return Tag;
}

float FGCGameplayTagStack::GetStackCount() const
{
	return StackCount;
}

//////////////////////////////////////////////////////////////////////
// FGCGameplayTagStackContainer

void FGCGameplayTagStackContainer::AddStack(FGameplayTag Tag, float StackCount)
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to AddStack"), ELogVerbosity::Warning);
		return;
	}

	if (StackCount > 0)
	{
		for (FGCGameplayTagStack& Stack : Stacks)
		{
			if (Stack.Tag == Tag)
			{
				const float NewCount = Stack.StackCount + StackCount;
				Stack.StackCount = NewCount;

				if (TagToCountMap.Contains(Tag))
				{
					TagToCountMap[Tag] = NewCount;
				}
				else
				{
					TagToCountMap.Add(Tag, NewCount);
				}
				Stack.OnChanged.Broadcast();
				MarkItemDirty(Stack);
				return;
			}
		}

		FGCGameplayTagStack& NewStack = Stacks.Emplace_GetRef(Tag, StackCount);
		MarkItemDirty(NewStack);
		TagToCountMap.Add(Tag, StackCount);
		OnStackItemAdded.Broadcast(Tag);
		NewStack.OnChanged.Broadcast();
	}
}

void FGCGameplayTagStackContainer::RemoveStack(FGameplayTag Tag, float StackCount)
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to RemoveStack"), ELogVerbosity::Warning);
		return;
	}

	//@TODO: Should we error if you try to remove a stack that doesn't exist or has a smaller count?
	if (StackCount > 0)
	{
		for (auto It = Stacks.CreateIterator(); It; ++It)
		{
			FGCGameplayTagStack& Stack = *It;
			if (Stack.Tag == Tag)
			{
				if (Stack.StackCount <= StackCount)
				{
					It.RemoveCurrent();
					TagToCountMap.Remove(Tag);
					MarkArrayDirty();
				}
				else
				{
					const float NewCount = Stack.StackCount - StackCount;
					Stack.StackCount = NewCount;
					TagToCountMap[Tag] = NewCount;
					MarkItemDirty(Stack);
				}
				Stack.OnChanged.Broadcast();
				return;
			}
		}
	}
}

void FGCGameplayTagStackContainer::ClearStack()
{
	for (FGCGameplayTagStack& Stack : Stacks)
	{
		RemoveStack(Stack.Tag, Stack.StackCount);
	}
}

TArray<FGCGameplayTagStack> FGCGameplayTagStackContainer::GetGameplayTagStackList() const
{
	return Stacks;
}

void FGCGameplayTagStackContainer::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		const FGameplayTag Tag = Stacks[Index].Tag;
		TagToCountMap.Remove(Tag);
		Stacks[Index].OnChanged.Broadcast();
	}
}

void FGCGameplayTagStackContainer::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		const FGCGameplayTagStack& Stack = Stacks[Index];
		TagToCountMap.Add(Stack.Tag, Stack.StackCount);
		OnStackItemAdded.Broadcast(Stack.Tag);
	}
}

void FGCGameplayTagStackContainer::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (int32 Index : ChangedIndices)
	{
		if (Stacks.IsValidIndex(Index))
		{
			const FGCGameplayTagStack& Stack = Stacks[Index];
			TagToCountMap[Stack.Tag] = Stack.StackCount;
			Stack.OnChanged.Broadcast();
		}
	}
}

FGCGameplayTagStack* FGCGameplayTagStackContainer::GetTagStackItem(const FGameplayTag& tag)
{
	for (auto It = Stacks.CreateIterator(); It; ++It)
	{
		FGCGameplayTagStack& Stack = *It;
		if (Stack.Tag == tag)
		{
			return &Stack;
		}
	}
	return nullptr;
}

void FGCGameplayTagStackContainer::BindDelegateToStackReplicated(const FGameplayTag& tag, const FDynamicOnStackItemReplicated& onStackItemReplicated, const UObject* ownerUObject)
{
	auto tagStack = GetTagStackItem(tag);
	if (tagStack)
	{
		tagStack->OnChanged.AddWeakLambda(ownerUObject,
			[onStackItemReplicated]()
			{
				onStackItemReplicated.ExecuteIfBound();
			});
	}
	else
	{
		OnStackItemAdded.AddWeakLambda(ownerUObject,
			[onStackItemReplicated, tag, ownerUObject, this](const FGameplayTag& broadCastTag)
			{
				if (tag == broadCastTag)
				{
					if (auto tagStackItem = this->GetTagStackItem(tag))
					{
						tagStackItem->OnChanged.AddWeakLambda(ownerUObject,
							[onStackItemReplicated]()
							{
								onStackItemReplicated.ExecuteIfBound();
							});
						tagStackItem->OnChanged.Broadcast();
					}
				}
			});
	}
}