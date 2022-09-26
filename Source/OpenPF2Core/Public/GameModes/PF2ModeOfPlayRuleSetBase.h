﻿// OpenPF2 for UE Game Logic, Copyright 2021-2022, Guy Elsmore-Paddock. All Rights Reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "PF2GameModeInterface.h"
#include "PF2ModeOfPlayRuleSetInterface.h"
#include "PF2PlayerControllerInterface.h"

#include "PF2ModeOfPlayRuleSetBase.generated.h"

/**
 * Default base class for OpenPF2 Mode of Play Rule Sets (MoPRS).
 *
 * @see IPF2GameModeInterface
 */
UCLASS(Abstract, Blueprintable)
// ReSharper disable once CppClassCanBeFinal
class OPENPF2CORE_API APF2ModeOfPlayRuleSetBase : public AActor, public IPF2ModeOfPlayRuleSetInterface
{
	GENERATED_BODY()

public:
	// =================================================================================================================
	// Public Methods - IPF2ModeOfPlayRuleSetInterface Implementation
	// =================================================================================================================
	virtual EPF2CommandExecuteOrQueueResult AttemptToExecuteOrQueueCommand_Implementation(
		const TScriptInterface<IPF2CharacterCommandInterface>& Command) override;

	virtual void AttemptToCancelCommand_Implementation(
		const TScriptInterface<IPF2CharacterCommandInterface>& Command) override;

protected:
	// =================================================================================================================
	// Protected Methods
	// =================================================================================================================
	/**
	 * Gets the game mode.
	 *
	 * @return
	 *	OpenPF2-specific game mode.
	 */
	UFUNCTION(BlueprintCallable, Category="OpenPF2|Mode of Play Rule Sets")
	TScriptInterface<IPF2GameModeInterface> GetGameMode() const;

	/**
	 * Gets controllers for all the characters in the world.
	 *
	 * @return
	 *	OpenPF2-specific controllers for all players in the world.
	 */
	UFUNCTION(BlueprintCallable, Category="OpenPF2|Mode of Play Rule Sets|Characters")
	TArray<TScriptInterface<IPF2PlayerControllerInterface>> GetPlayerControllers() const;

	/**
	 * Gets all of the player-controlled OpenPF2 characters in the world.
	 *
	 * @return
	 *	OpenPF2-specific controllers for all players in the world.
	 */
	UFUNCTION(BlueprintCallable, Category="OpenPF2|Mode of Play Rule Sets|Characters")
	TArray<TScriptInterface<IPF2CharacterInterface>> GetPlayerControlledCharacters() const;
};
