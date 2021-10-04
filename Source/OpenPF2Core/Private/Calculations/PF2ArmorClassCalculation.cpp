﻿// OpenPF2 for UE Game Logic, Copyright 2021, Guy Elsmore-Paddock. All Rights Reserved.
//
// Content from Pathfinder 2nd Edition is licensed under the Open Game License (OGL) v1.0a, subject to the following:
//   - Open Game License v 1.0a, Copyright 2000, Wizards of the Coast, Inc.
//   - System Reference Document, Copyright 2000, Wizards of the Coast, Inc.
//   - Pathfinder Core Rulebook (Second Edition), Copyright 2019, Paizo Inc.
// Except for material designated as Product Identity, the game mechanics and logic in this file are Open Game Content,
// as defined in the Open Game License version 1.0a, Section 1(d) (see accompanying LICENSE.TXT). No portion of this
// file other than the material designated as Open Game Content may be reproduced in any form without written
// permission.

#include "Calculations/PF2ArmorClassCalculation.h"

#include "OpenPF2Core.h"
#include "GameplayAbilityUtils.h"
#include "TemlCalculationUtils.h"

#include "Abilities/PF2AbilityAttributes.h"
#include "Abilities/PF2AttributeSet.h"

UPF2ArmorClassCalculation::UPF2ArmorClassCalculation() :
	DexterityModifierCaptureDefinition(FPF2AbilityAttributes::GetInstance().AbDexterityModifierDef)
{
	this->RelevantAttributesToCapture.Add(this->DexterityModifierCaptureDefinition);
}

float UPF2ArmorClassCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// From Pathfinder 2E Core Rulebook, page 274, "Armor Class".
	// "Armor Class = 10 + Dexterity modifier (up to your armor’s Dex Cap) + proficiency bonus
	// + armor's item bonus to AC + other bonuses + penalties"
	//
	// "Use your proficiency bonus for the category (light, medium, or heavy) or the specific type of armor you're
	// wearing. If you're not wearing armor, use your proficiency in unarmored defense."
	//
	// TODO: Implement armor Dex Cap.
	const float DexterityModifier         = GetDexterityModifier(Spec),
	            ArmorTypeProficiencyBonus = CalculateArmorTypeProficiencyBonus(Spec),
	            AbilityScore              = 10.0f + DexterityModifier + ArmorTypeProficiencyBonus;

	UE_LOG(
		LogPf2Core,
		VeryVerbose,
		TEXT("Calculated armor class score: 10 + %f + %f = %f"),
		DexterityModifier,
		ArmorTypeProficiencyBonus,
		AbilityScore
	);

	return AbilityScore;
}

float UPF2ArmorClassCalculation::GetDexterityModifier(const FGameplayEffectSpec& Spec) const
{
	float                         DexterityModifier     = 0.0f;
	const FGameplayTagContainer   *SourceTags           = Spec.CapturedSourceTags.GetAggregatedTags(),
	                              *TargetTags           = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;

	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	this->GetCapturedAttributeMagnitude(
		this->DexterityModifierCaptureDefinition,
		Spec,
		EvaluationParameters,
		DexterityModifier
	);

	return DexterityModifier;
}

float UPF2ArmorClassCalculation::CalculateArmorTypeProficiencyBonus(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags                 = Spec.CapturedSourceTags.GetAggregatedTags();
	const FString                ArmorType                  = DetermineArmorType(SourceTags),
	                             ArmorTypeProficiencyPrefix = "Armor.Category." + ArmorType;

	const float ProficiencyBonus = TemlCalculationUtils::CalculateProficiencyBonus(ArmorTypeProficiencyPrefix, Spec);

	UE_LOG(
		LogPf2Core,
		VeryVerbose,
		TEXT("Calculated armor proficiency bonus ('%s'): %f"),
		*(ArmorType),
		ProficiencyBonus
	);

	return ProficiencyBonus;
}

FString UPF2ArmorClassCalculation::DetermineArmorType(const FGameplayTagContainer* SourceTags) const
{
	// Default to no armor.
	FString ArmorType = "Unarmored";

	// Bypass additional checks if the character has no armor equipped, to avoid checking every armor type.
	if (GameplayAbilityUtils::HasTag(SourceTags, FName("Armor.Equipped")))
	{
		if (GameplayAbilityUtils::HasTag(SourceTags, FName("Armor.Equipped.Heavy")))
		{
			ArmorType = "Heavy";
		}
		else if (GameplayAbilityUtils::HasTag(SourceTags, FName("Armor.Equipped.Medium")))
		{
			ArmorType = "Medium";
		}
		else if (GameplayAbilityUtils::HasTag(SourceTags, FName("Armor.Equipped.Light")))
		{
			ArmorType = "Light";
		}
	}

	return ArmorType;
}
