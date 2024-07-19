﻿// OpenPF2 Game Framework for Unreal Engine, Copyright 2021-2024, Guy Elsmore-Paddock. All Rights Reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "CharacterStats/AbilityBoosts/PF2AbilityBoostBase.h"

#include "CharacterStats/PF2CharacterAbilitySystemInterface.h"
#include "CharacterStats/PF2CharacterAttributeSet.h"
#include "CharacterStats/AbilityBoosts/PF2AbilityBoostRuleOptionValidator.h"
#include "CharacterStats/AbilityBoosts/PF2GameplayAbilityTargetData_BoostAbility.h"

#include "GameplayTags/Encounters/AbilityTriggers.h"
#include "GameplayTags/GameplayAbilities/AbilityTypes.h"

UPF2AbilityBoostBase::UPF2AbilityBoostBase()
{
	FAbilityTriggerData TriggerData;

	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	TriggerData.TriggerTag    = Pf2TagTriggerTagCategoryAnytimeAbilityBoost;

	this->AbilityTriggers.Add(TriggerData);

	// We don't maintain any local state.
	this->InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;

	this->AbilityTags.AddTag(Pf2TagGameplayAbilityTypeAbilityBoost);
}

bool UPF2AbilityBoostBase::CheckCost(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	FGameplayTagContainer*           OptionalRelevantTags) const
{
	if (Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags))
	{
		const UPF2CharacterAttributeSet* AttributeSet  = PF2GameplayAbilityUtilities::GetCharacterAttributeSet(ActorInfo);
		const float                      BoostsToApply = this->BoostRuleOptions.Num(),
		                                 BoostsApplied = AttributeSet->GetAbBoostCount(),
		                                 BoostLimit    = AttributeSet->GetAbBoostLimit();

		// We can apply this set of ability boosts if the difference between how many boosts are applied and the boost
		// limit is large enough to accomodate all of the boost options on this ability.
		return ((BoostsApplied + BoostsToApply) <= BoostLimit);
	}
	else
	{
		return false;
	}
}

void UPF2AbilityBoostBase::ActivateAbility(
	const FGameplayAbilitySpecHandle     Handle,
	const FGameplayAbilityActorInfo*     ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData*            TriggerEventData)
{
	if (CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		const TSet<EPF2CharacterAbilityScoreType> BoostSelections = this->GetBoostSelections(TriggerEventData);

		IPF2CharacterAbilitySystemInterface* CharacterAsc =
			PF2GameplayAbilityUtilities::GetCharacterAbilitySystemComponent(ActorInfo);

		UPF2AbilityBoostRuleOptionValidator* Validator = NewObject<UPF2AbilityBoostRuleOptionValidator>();

		Validator->AppendRuleOptions(this->BoostRuleOptions);

		for (auto& Selection : BoostSelections)
		{
			Validator->ApplyAbilityBoost(Selection);
		}

		checkf(
			!Validator->HasRemainingBoosts(),
			TEXT("There must be a selection for every rule option.")
		);

		for (auto& Selection : BoostSelections)
		{
			CharacterAsc->ApplyAbilityBoost(Selection);
		}

		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

FText UPF2AbilityBoostBase::GetDescription() const
{
	return this->Description;
}

TArray<FPF2AbilityBoostRuleOption> UPF2AbilityBoostBase::GetBoostRuleOptions() const
{
	return this->BoostRuleOptions;
}

UGameplayAbility* UPF2AbilityBoostBase::ToGameplayAbility()
{
	return this;
}

TSet<EPF2CharacterAbilityScoreType> UPF2AbilityBoostBase::GetBoostSelections(
	const FGameplayEventData* TriggerEventData) const
{
	const FGameplayAbilityTargetDataHandle GameplayAbilityTargetDataHandle = TriggerEventData->TargetData;
	const FGameplayAbilityTargetData*      AbilityTargetData               = GameplayAbilityTargetDataHandle.Get(0);

	const FPF2GameplayAbilityTargetData_BoostAbility* BoostTargetData;

	check(AbilityTargetData != nullptr);

	BoostTargetData = StaticCast<const FPF2GameplayAbilityTargetData_BoostAbility*>(AbilityTargetData);
	check(BoostTargetData != nullptr);

	return BoostTargetData->SelectedAbilities;
}
