﻿// OpenPF2 for UE Game Logic, Copyright 2021, Guy Elsmore-Paddock. All Rights Reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "PF2PlayerControllerBase.h"

#include "PF2CharacterInterface.h"

void APF2PlayerControllerBase::HandleModeOfPlayChanged(const EPF2ModeOfPlayType NewMode)
{
	this->OnModeOfPlayChanged(NewMode);
}

void APF2PlayerControllerBase::HandleActionQueued_Implementation(const TScriptInterface<IPF2QueuedActionInterface>& Action)
{
	this->OnActionQueued(Action);
}

void APF2PlayerControllerBase::HandleActionDequeued_Implementation(const TScriptInterface<IPF2QueuedActionInterface>& Action)
{
	this->OnActionDequeued(Action);
}

TScriptInterface<IPF2CharacterInterface> APF2PlayerControllerBase::GetControlledCharacter()
{
	return this->GetPawn();
}

APlayerController* APF2PlayerControllerBase::ToPlayerController()
{
	return this;
}
