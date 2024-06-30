﻿// OpenPF2 Game Framework for Unreal Engine, Copyright 2021-2024, Guy Elsmore-Paddock. All Rights Reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <GameplayEffect.h>

#include "PF2ClassGameplayEffectBase.generated.h"

/**
 * Parent class for Gameplay Effects that represent how a character's class impacts them.
 */
UCLASS(Abstract)
class OPENPF2GAMEFRAMEWORK_API UPF2ClassGameplayEffectBase : public UGameplayEffect
{
	GENERATED_BODY()
};
