﻿// OpenPF2 Game Framework for Unreal Engine, Copyright 2021-2024, Guy Elsmore-Paddock. All Rights Reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "CharacterStats/AbilityModifiers/PF2SimpleTemlModifierCalculationBase.h"

#include "PF2SavingThrowModifierCalculationBase.generated.h"

/**
 * Base class for Saving Throw modifier MMCs.
 *
 * There is expected to be one blueprint sub-class of this base class for each type of Saving Throw (Fortitude, Reflex,
 * and Will). The ability upon which the saving throw modifier is based *must* be the one and ONLY attribute being
 * captured.
 */
UCLASS(Abstract)
class OPENPF2GAMEFRAMEWORK_API UPF2SavingThrowModifierCalculationBase : public UPF2SimpleTemlModifierCalculationBase
{
	GENERATED_BODY()
};
