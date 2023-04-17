﻿// OpenPF2 for UE Game Logic, Copyright 2022-2023, Guy Elsmore-Paddock. All Rights Reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <UObject/Object.h>

#include "PF2AbilityExecutionFilterInterface.h"
#include "PF2AbilityExecutionFilterBase.generated.h"

// =====================================================================================================================
// Forward Declarations (to minimize header dependencies)
// =====================================================================================================================
class IPF2GameStateInterface;

// =====================================================================================================================
// Normal Declarations
// =====================================================================================================================
/**
 * Abstract base class for OpenPF2 ability execution filters.
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
// ReSharper disable once CppClassCanBeFinal
class OPENPF2CORE_API UPF2AbilityExecutionFilterBase :
	public UObject,
	public IPF2AbilityExecutionFilterInterface
{
	GENERATED_BODY()

public:
	// =================================================================================================================
	// Public Methods - UObject Overrides
	// =================================================================================================================
	virtual UWorld* GetWorld() const override;

	// =================================================================================================================
	// Public Methods - IPF2LogIdentifiableInterface Implementation
	// =================================================================================================================
	UFUNCTION(BlueprintCallable)
	virtual FString GetIdForLogs() const override;

protected:
	// =================================================================================================================
	// Protected Methods
	// =================================================================================================================
	/**
	 * Gets a reference to the world that contains the character who is activating the ability.
	 *
	 * @return
	 *	The world containing the character on which the ability is being activated.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="OpenPF2|Gameplay Ability Execution Filters")
	static UWorld* GetWorld(const FPF2AbilityExecutionFilterContext ExecutionContext);

	/**
	 * Gets a reference to the current game state.
	 *
	 * @return
	 *	The state of the game.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="OpenPF2|Gameplay Ability Execution Filters")
	static TScriptInterface<IPF2GameStateInterface> GetGameState(
		const FPF2AbilityExecutionFilterContext ExecutionContext);
};
