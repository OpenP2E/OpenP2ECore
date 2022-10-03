﻿// OpenPF2 for UE Game Logic, Copyright 2022, Guy Elsmore-Paddock. All Rights Reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "PF2CharacterQueueComponent.h"

#include <Net/UnrealNetwork.h>

#include "PF2CharacterInterface.h"

#include "Utilities/PF2InterfaceUtilities.h"
#include "Utilities/PF2LogUtilities.h"

UPF2CharacterQueueComponent::UPF2CharacterQueueComponent(): ActiveCharacterIndex(0)
{
	this->SetIsReplicatedByDefault(true);
}

void UPF2CharacterQueueComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPF2CharacterQueueComponent, Queue);
	DOREPLIFETIME(UPF2CharacterQueueComponent, ActiveCharacterIndex);
}

TScriptInterface<IPF2CharacterInterface> UPF2CharacterQueueComponent::GetActiveCharacter() const
{
	TScriptInterface<IPF2CharacterInterface> ActiveCharacter;

	if (this->Count() == 0)
	{
		// No active character.
		ActiveCharacter = TScriptInterface<IPF2CharacterInterface>();
	}
	else
	{
		AActor*                 ActiveCharacterActor;
		IPF2CharacterInterface* ActiveCharacterIntf;

		check(this->ActiveCharacterIndex < this->Count());

		ActiveCharacterActor = this->Queue[this->ActiveCharacterIndex];
		ActiveCharacterIntf  = Cast<IPF2CharacterInterface>(ActiveCharacterActor);

		check(ActiveCharacterIntf != nullptr);

		ActiveCharacter = PF2InterfaceUtilities::ToScriptInterface(ActiveCharacterIntf);
	}

	return ActiveCharacter;
}

void UPF2CharacterQueueComponent::Add(const TScriptInterface<IPF2CharacterInterface> Character)
{
	UE_LOG(
		LogPf2Core,
		Verbose,
		TEXT("[%s] Character ('%s') has been added to character queue ('%s')."),
		*(PF2LogUtilities::GetHostNetId(this->GetWorld())),
		*(Character->GetIdForLogs()),
		*(this->GetIdForLogs())
	);

	check(this->Count() <= (static_cast<uint32>(UINT8_MAX) + 1));

	this->Queue.AddUnique(Character->ToActor());

	this->Native_OnCharacterAdded(Character);
	this->Native_OnCharactersChanged();
}

bool UPF2CharacterQueueComponent::Remove(const TScriptInterface<IPF2CharacterInterface> Character)
{
	AActor*     CharacterActor       = Character->ToActor();
	const int32 RemovedIndex         = this->Queue.Find(CharacterActor);
	const int32 CountOfRemoved       = this->Queue.Remove(CharacterActor);
	const bool  bWasCharacterRemoved = (CountOfRemoved > 0);

	if (bWasCharacterRemoved)
	{
		check(RemovedIndex != INDEX_NONE);

		this->Native_OnCharacterRemoved(Character, RemovedIndex);
		this->Native_OnCharactersChanged();
	}

	return bWasCharacterRemoved;
}

int UPF2CharacterQueueComponent::Count() const
{
	return this->Queue.Num();
}

void UPF2CharacterQueueComponent::Clear()
{
	TArray<AActor*> OldQueue = this->Queue;

	this->Queue.Empty();

	for (int CharacterIndex = 0; CharacterIndex < OldQueue.Num(); ++CharacterIndex)
	{
		AActor*                 Character     = OldQueue[CharacterIndex];
		IPF2CharacterInterface* CharacterIntf = Cast<IPF2CharacterInterface>(Character);

		check(CharacterIntf != nullptr);

		this->Native_OnCharacterRemoved(
			PF2InterfaceUtilities::ToScriptInterface(CharacterIntf),
			CharacterIndex
		);
	}

	this->Native_OnCharactersChanged();
}

TScriptInterface<IPF2CharacterInterface> UPF2CharacterQueueComponent::PreviousCharacter()
{
	// Only rewind if we have elements.
	if (this->Count() != 0)
	{
		uint8 NewCharacterIndex;

		if (this->ActiveCharacterIndex == 0)
		{
			// We're already at the start, so restart with the character at the end of the queue.
			NewCharacterIndex = this->GetMaxIndex();
		}
		else
		{
			// We're not yet at the start, so just jump back one character in the queue.
			NewCharacterIndex = this->ActiveCharacterIndex - 1;
		}

		this->SetActiveCharacterIndex(NewCharacterIndex);
	}

	return this->GetActiveCharacter();
}

TScriptInterface<IPF2CharacterInterface> UPF2CharacterQueueComponent::NextCharacter()
{
	// Only advance if we have elements.
	if (this->Count() != 0)
	{
		uint8 NewCharacterIndex;

		if (this->ActiveCharacterIndex == this->GetMaxIndex())
		{
			// We're already at the end, so restart with the character at the start of the queue.
			NewCharacterIndex = 0;
		}
		else
		{
			// We're not yet at the end, so just jump forward one character in the queue.
			NewCharacterIndex = this->ActiveCharacterIndex + 1;
		}

		this->SetActiveCharacterIndex(NewCharacterIndex);
	}

	return this->GetActiveCharacter();
}

TArray<TScriptInterface<IPF2CharacterInterface>> UPF2CharacterQueueComponent::ToArray() const
{
	return PF2ArrayUtilities::Reduce<TArray<TScriptInterface<IPF2CharacterInterface>>>(
		this->Queue,
		TArray<TScriptInterface<IPF2CharacterInterface>>(),
		[](TArray<TScriptInterface<IPF2CharacterInterface>> Characters,
		   const TWeakInterfacePtr<IPF2CharacterInterface>  CurrentCharacter)
		{
			if (CurrentCharacter.IsValid())
			{
				Characters.Add(PF2InterfaceUtilities::ToScriptInterface(CurrentCharacter.Get()));
			}

			return Characters;
		});
}

UActorComponent* UPF2CharacterQueueComponent::ToActorComponent()
{
	return this;
}

FString UPF2CharacterQueueComponent::GetIdForLogs() const
{
	// ReSharper disable CppRedundantParentheses
	return FString::Format(
		TEXT("{0}.{1}"),
		{
			*(GetNameSafe(this->GetOwner())),
			*(this->GetName())
		}
	);
}

void UPF2CharacterQueueComponent::SetActiveCharacterIndex(const uint8 NewActiveCharacterIndex)
{
	const TScriptInterface<IPF2CharacterInterface> OldCharacter = this->GetActiveCharacter();
	TScriptInterface<IPF2CharacterInterface>       NewCharacter;

	check(NewActiveCharacterIndex < this->Count());

	this->ActiveCharacterIndex = NewActiveCharacterIndex;
	NewCharacter               = this->GetActiveCharacter();

	if (OldCharacter != NewCharacter)
	{
		this->Native_OnActiveCharacterChanged(NewCharacter);
	}
}

void UPF2CharacterQueueComponent::OnRep_CharacterQueue(const TArray<AActor*> OldCharacters)
{
	TArray<IPF2CharacterInterface*> RemovedCharacters,
									AddedCharacters;

	// BUGBUG: By the time we're here, this should definitely be an OpenPF2 character, but UE will sometimes replicate
	// entries in this->ControllableCharacters as NULL.
	PF2ArrayUtilities::CaptureDeltasWithCast(OldCharacters, this->Queue, RemovedCharacters, AddedCharacters);

	for (IPF2CharacterInterface* const& RemovedCharacter : RemovedCharacters)
	{
		const int32 RemovedIndex = OldCharacters.Find(RemovedCharacter->ToActor());

		check(RemovedIndex != INDEX_NONE);

		this->Native_OnCharacterRemoved(PF2InterfaceUtilities::ToScriptInterface(RemovedCharacter), RemovedIndex);
	}

	for (IPF2CharacterInterface* const& AddedCharacter : AddedCharacters)
	{
		this->Native_OnCharacterAdded(PF2InterfaceUtilities::ToScriptInterface(AddedCharacter));
	}

	if ((RemovedCharacters.Num() != 0) || (AddedCharacters.Num() != 0))
	{
		this->Native_OnCharactersChanged();
	}
}

void UPF2CharacterQueueComponent::OnRep_ActiveCharacterIndex()
{
	this->Native_OnActiveCharacterChanged(this->GetActiveCharacter());
}

void UPF2CharacterQueueComponent::Native_OnCharactersChanged()
{
	TArray<TScriptInterface<IPF2CharacterInterface>> NewCharacters;

	UE_LOG(
		LogPf2Core,
		Verbose,
		TEXT("[%s] Characters in queue ('%s') have changed."),
		*(PF2LogUtilities::GetHostNetId(this->GetWorld())),
		*(this->GetIdForLogs())
	);

	for (AActor* Character : this->Queue)
	{
		// BUGBUG: By the time we're here, this should definitely be an OpenPF2 character, but UE will sometimes
		// replicate entries in this->Queue as NULL.
		if (Character != nullptr)
		{
			NewCharacters.Add(
				PF2InterfaceUtilities::ToScriptInterface<IPF2CharacterInterface>(
					Cast<IPF2CharacterInterface>(Character)
				)
			);
		}
	}

	UE_LOG(
		LogPf2CoreAbilities,
		VeryVerbose,
		TEXT("[%s] Character queue changed ('%s') - %d elements."),
		*(PF2LogUtilities::GetHostNetId(this->GetWorld())),
		*(this->GetIdForLogs()),
		NewCharacters.Num()
	);

	this->OnCharactersChanged.Broadcast(NewCharacters);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UPF2CharacterQueueComponent::Native_OnCharacterAdded(
	const TScriptInterface<IPF2CharacterInterface>& AddedCharacter)
{
	UE_LOG(
		LogPf2Core,
		Verbose,
		TEXT("[%s] Character ('%s') added to queue ('%s')."),
		*(PF2LogUtilities::GetHostNetId(this->GetWorld())),
		*(AddedCharacter->GetIdForLogs()),
		*(this->GetIdForLogs())
	);

	this->OnCharacterAdded.Broadcast(AddedCharacter);
}

void UPF2CharacterQueueComponent::Native_OnCharacterRemoved(
	const TScriptInterface<IPF2CharacterInterface>& RemovedCharacter,
	const uint8 RemovedIndex)
{
	UE_LOG(
		LogPf2Core,
		Verbose,
		TEXT("[%s] Character ('%s') removed from queue ('%s')."),
		*(PF2LogUtilities::GetHostNetId(this->GetWorld())),
		*(RemovedCharacter->GetIdForLogs()),
		*(this->GetIdForLogs())
	);

	// Ensure that we keep the active character cursor in bounds. If the character that was removed was the active
	// character or a character prior to it, we move the active character index accordingly.
	if (this->ActiveCharacterIndex >= RemovedIndex)
	{
		if (this->ActiveCharacterIndex != 0)
		{
			// We handle this ourselves here instead of using SetActiveCharacterIndex() since its detection of the
			// previous active character could be out of bounds at this point.
			this->ActiveCharacterIndex -= 1;
		}

		// Manually invoke active character change callback.
		this->Native_OnActiveCharacterChanged(this->GetActiveCharacter());
	}

	this->OnCharacterRemoved.Broadcast(RemovedCharacter);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UPF2CharacterQueueComponent::Native_OnActiveCharacterChanged(
	const TScriptInterface<IPF2CharacterInterface>& NewCharacter)
{
	UE_LOG(
		LogPf2Core,
		Verbose,
		TEXT("[%s] Active character in queue ('%s') has changed (now '%s')."),
		*(PF2LogUtilities::GetHostNetId(this->GetWorld())),
		*(this->GetIdForLogs()),
		*(NewCharacter->GetIdForLogs())
	);

	this->OnActiveCharacterChanged.Broadcast(NewCharacter);
}
