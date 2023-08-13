﻿// OpenPF2 for UE Game Logic, Copyright 2022-2023, Guy Elsmore-Paddock. All Rights Reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <GameFramework/Info.h>

#include "GameplayAbilitySpec.h"
#include "PF2CharacterCommandInterface.h"
#include "PF2CharacterInterface.h"

#include "Abilities/PF2GameplayAbilityInterface.h"

#include "Commands/PF2CommandExecuteImmediatelyResult.h"
#include "Commands/PF2CommandExecuteOrQueueResult.h"

#include "Utilities/PF2InterfaceUtilities.h"

#include "PF2CharacterCommand.generated.h"

// =====================================================================================================================
// Forward Declarations (to minimize header dependencies)
// =====================================================================================================================
class IPF2AbilitySystemInterface;

// =====================================================================================================================
// Normal Declarations
// =====================================================================================================================
/**
 * A command that wraps a character ability that can be executed at any time in the future (including immediately).
 *
 * (The inspiration for this approach -- of wrapping Gameplay Abilities in a command that can be invoked at any time --
 * came from "ue4-orders-abilities" by Daedalic Entertainment,
 * https://github.com/DaedalicEntertainment/ue4-orders-abilities).
 */
UCLASS(BlueprintType)
// ReSharper disable once CppClassCanBeFinal
class OPENPF2CORE_API APF2CharacterCommand : public AInfo, public IPF2CharacterCommandInterface
{
	GENERATED_BODY()

protected:
	// =================================================================================================================
	// Protected Fields
	// =================================================================================================================
	/**
	 * The character who would be issued this command.
	 */
	UPROPERTY(Replicated)
	AActor* TargetCharacter;

	/**
	 * The handle of the ability that this command will trigger when it is executed.
	 */
	UPROPERTY(Replicated)
	FGameplayAbilitySpecHandle AbilitySpecHandle;

	/**
	 * The payload to provide when invoking the ability.
	 *
	 * Not all abilities use the payload; this is only useful for those that do.
	 */
	UPROPERTY(Replicated)
	FGameplayEventData AbilityPayload;

	/**
	 * The cached ability for this command.
	 *
	 * This is memoized by APF2CharacterCommand::GetAbility().
	 */
	UPROPERTY()
	mutable UGameplayAbility* CachedAbility;

public:
	// =================================================================================================================
	// Public Static Methods
	// =================================================================================================================
	/**
	 * Creates a new APF2CharacterCommand for the given character and ability specification.
	 *
	 * @param InCharacter
	 *	The character who would be issued the command.
	 * @param InAbilitySpecHandle
	 *	The handle of the ability that the command will trigger when it is executed.
	 * @param InAbilityPayload
	 *	An optional payload to pass to the ability.
	 *
	 * @return
	 *	The new command.
	 */
	UFUNCTION(BlueprintPure, Category="OpenPF2|Character Commands", meta=(DisplayName="Create Character Command"))
	static TScriptInterface<IPF2CharacterCommandInterface> Create(
		const TScriptInterface<IPF2CharacterInterface> InCharacter,
		const FGameplayAbilitySpecHandle               InAbilitySpecHandle,
		// ReSharper disable once CppPassValueParameterByConstReference
		const FGameplayEventData                       InAbilityPayload=FGameplayEventData())
	{
		return PF2InterfaceUtilities::ToScriptInterface(
			Create(InCharacter->ToActor(), InAbilitySpecHandle, InAbilityPayload)
		);
	}

	/**
	 * Creates a new APF2CharacterCommand for the given character and ability handle.
	 *
	 * @param Character
	 *	The character who would be issued the command.
	 * @param AbilitySpecHandle
	 *	The handle of the ability that the command will trigger when it is executed.
	 *
	 * @return
	 *	The new command.
	 */
	FORCEINLINE static IPF2CharacterCommandInterface* Create(IPF2CharacterInterface*          Character,
	                                                         const FGameplayAbilitySpecHandle AbilitySpecHandle)
	{
		return Create(Character->ToActor(), AbilitySpecHandle);
	}

	/**
	 * Creates a new APF2CharacterCommand for the given character, ability handle, and ability payload.
	 *
	 * @param Character
	 *	The character who would be issued the command.
	 * @param AbilitySpecHandle
	 *	The handle of the ability that the command will trigger when it is executed.
	 * @param AbilityPayload
	 *	The payload to provide when invoking the ability.
	 *
	 * @return
	 *	The new command.
	 */
	FORCEINLINE static IPF2CharacterCommandInterface* Create(IPF2CharacterInterface*          Character,
	                                                         const FGameplayAbilitySpecHandle AbilitySpecHandle,
	                                                         const FGameplayEventData&        AbilityPayload)
	{
		return Create(Character->ToActor(), AbilitySpecHandle, AbilityPayload);
	}

	/**
	 * Creates a new APF2CharacterCommand for the given character actor and ability specification.
	 *
	 * The given actor must implement IPF2CharacterInterface.
	 *
	 * @param CharacterActor
	 *	The character (as an actor) who would be issued the command.
	 * @param AbilitySpecHandle
	 *	The handle of the ability that the command will trigger when it is executed.
	 * @param AbilityPayload
	 *	The payload to provide when invoking the ability. This can be omitted when invoking abilities that do not accept
	 *	a payload.
	 *
	 * @return
	 *	The new command.
	 */
	static IPF2CharacterCommandInterface* Create(
		AActor*                          CharacterActor,
		const FGameplayAbilitySpecHandle AbilitySpecHandle,
		const FGameplayEventData&        AbilityPayload = FGameplayEventData());

protected:
	// =================================================================================================================
	// Protected Constructors
	// =================================================================================================================
	explicit APF2CharacterCommand() : TargetCharacter(nullptr)
	{
		// Replicate commands to ensure that, when characters are controlled by AI during encounters, both the server
		// and the client who is issuing the command can observe its details (icon, description, and callback).
		this->bReplicates     = true;
		this->bAlwaysRelevant = true;
	}

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// =================================================================================================================
	// Public Methods - IPF2CharacterCommandInterface Implementation
	// =================================================================================================================
	virtual TScriptInterface<IPF2CharacterInterface> GetTargetCharacter() const override;

	virtual UTexture2D* GetCommandIcon() const override;

	virtual FText GetCommandLabel() const override;

	virtual FText GetCommandDescription() const override;

	virtual EPF2CommandExecuteOrQueueResult AttemptExecuteOrQueue() override;

	virtual EPF2CommandExecuteImmediatelyResult AttemptExecuteImmediately() override;

	virtual void AttemptCancel() override;

	virtual AInfo* ToActor() override;

	// =================================================================================================================
	// Public Methods - IPF2LogIdentifiableInterface Implementation
	// =================================================================================================================
	virtual FString GetIdForLogs() const override;

protected:
	// =================================================================================================================
	// Protected Methods
	// =================================================================================================================
	/**
	 * Gets the handle of the ability that this command will trigger when it is executed.
	 *
	 * @return
	 *	The ability handle.
	 */
	FORCEINLINE FGameplayAbilitySpecHandle GetAbilitySpecHandle() const
	{
		return this->AbilitySpecHandle;
	}

	/**
	 * Gets the payload to provide when invoking the ability.
	 *
	 * Not all abilities use the payload; this is only useful for those that do.
	 *
	 * @return
	 *	The payload to pass to the ability when it is activated.
	 */
	FORCEINLINE FGameplayEventData GetAbilityPayload() const
	{
		return this->AbilityPayload;
	}

	/**
	 * Gets the specification for the ability that this command will trigger when it is executed.
	 *
	 * @return
	 *	The ability specification; or nullptr if either the character lacks an ASC (unlikely) or the character no longer
	 *	has an ability that matches this command (more likely).
	 */
	FGameplayAbilitySpec* GetAbilitySpec() const;

	/**
	 * Gets the Ability System Component (ASC) of the character for which this command will be executed.
	 *
	 * @return
	 *	The ASC of the target character; or NULL if the character is somehow missing an Ability System Component.
	 */
	IPF2AbilitySystemInterface* GetAbilitySystemComponent() const;

	/**
	 * Gets the CDO for the ability that this command will trigger when it is executed.
	 *
	 * @return
	 *	The gameplay ability; or nullptr if the character no longer has an ability that corresponds to the specification
	 *	of this command.
	 */
	FORCEINLINE UGameplayAbility* GetAbility() const;

	/**
	 * Gets the OpenPF2 interface to the CDO of the ability that this command will trigger when it is executed.
	 *
	 * @return
	 *	The gameplay ability, as an OpenPF2 interface; or nullptr if the character no longer has an ability that
	 *	corresponds to the specification of this command.
	 */
	FORCEINLINE IPF2GameplayAbilityInterface* GetAbilityIntf() const
	{
		IPF2GameplayAbilityInterface* Ability = Cast<IPF2GameplayAbilityInterface>(this->GetAbility());

		return Ability;
	}

	/**
	 * Sets the ability that this command will execute and the character upon which the ability will be executed.
	 *
	 * @param InTargetCharacter
	 *	The character who would be issued this command.
	 * @param InAbilitySpecHandle
	 *	The handle of the ability that this command will trigger when it is executed.
	 * @param InAbilityPayload
	 *	The payload (if applicable) for the ability.
	 */
	void SetTargetCharacterAndAbility(AActor*                          InTargetCharacter,
	                                  const FGameplayAbilitySpecHandle InAbilitySpecHandle,
	                                  const FGameplayEventData&        InAbilityPayload);

	/**
	 * Attempts to cancel this command on the remote server by routing the request through the local player controller.
	 */
	void Cancel_WithRemoteServer();

	/**
	 * Attempts to cancel this command on the local server by notifying the game mode, which typically notifies a MoPRS.
	 */
	void Cancel_WithLocalServer();
};
