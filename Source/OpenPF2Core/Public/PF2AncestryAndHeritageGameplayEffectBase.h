﻿// OpenPF2 for UE Game Logic, Copyright 2021, Guy Elsmore-Paddock. All Rights Reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "GameplayEffect.h"

#include "PF2AncestryAndHeritageGameplayEffectBase.generated.h"

/**
 * Parent class for Gameplay Effects that represent how a character's ancestry impacts them.
 */
UCLASS(Abstract)
class OPENPF2CORE_API UPF2AncestryAndHeritageGameplayEffectBase : public UGameplayEffect
{
	GENERATED_BODY()
};
