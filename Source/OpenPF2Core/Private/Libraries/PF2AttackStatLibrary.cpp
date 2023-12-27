﻿// OpenPF2 for UE Game Logic, Copyright 2023, Guy Elsmore-Paddock. All Rights Reserved.
//
// Content from Pathfinder 2nd Edition is licensed under the Open Game License (OGL) v1.0a, subject to the following:
//   - Open Game License v 1.0a, Copyright 2000, Wizards of the Coast, Inc.
//   - System Reference Document, Copyright 2000, Wizards of the Coast, Inc.
//   - Pathfinder Core Rulebook (Second Edition), Copyright 2019, Paizo Inc.
//
// Except for material designated as Product Identity, the game mechanics and logic in this file are Open Game Content,
// as defined in the Open Game License version 1.0a, Section 1(d) (see accompanying LICENSE.TXT). No portion of this
// file other than the material designated as Open Game Content may be reproduced in any form without written
// permission.

#include "Libraries/PF2AttackStatLibrary.h"

#include "Calculations/PF2TemlCalculation.h"

#include "Libraries/PF2DiceLibrary.h"

#include "Utilities/PF2EnumUtilities.h"

EPF2CheckResult UPF2AttackStatLibrary::PerformAttackRoll(const int32                  CharacterLevel,
                                                         const FGameplayTagContainer& CharacterTags,
                                                         const float                  AttackAbilityModifier,
                                                         const float                  MultipleAttackPenalty,
                                                         const FGameplayTagContainer& ProficiencyTagPrefixes,
                                                         const float                  TargetArmorClass)
{
	EPF2CheckResult Result;

	// "When making an attack roll, determine the result by rolling 1d20 and adding your attack modifier for the weapon
	// or unarmed attack you’re using."
	//
	// Source: Pathfinder 2E Core Rulebook, Chapter 6, page 278, "Attack Rolls".
	const int32 DiceRoll               = UPF2DiceLibrary::RollSum(1, 20);
	const bool  bIsNatural20           = (DiceRoll == 20);
	float       WeaponProficiencyBonus = 0,
	            AttackRoll;

	if (MultipleAttackPenalty > 0)
	{
		UE_LOG(
			LogPf2CoreAbilities,
			Error,
			TEXT("PerformAttackRoll(): The Multiple Attack Penalty should be negative or zero (was given '%f')."),
			MultipleAttackPenalty
		);
	}

	// "When attempting a check that involves something you have some training in, you will also add your proficiency
	// bonus. This bonus depends on your proficiency rank [...] if you have multiple bonuses of the same type, you can
	// use only the highest bonus on a given roll—in other words, they don’t "stack."
	//
	// Source: Pathfinder 2E Core Rulebook, Chapter 9, page 444, "Step 1: Roll D20 and Identify The Modifiers, Bonuses,
	// and Penalties That Apply".
	for (const FGameplayTag& ProficiencyTagPrefix : ProficiencyTagPrefixes)
	{
		FPF2TemlCalculation Calculation(ProficiencyTagPrefix, &CharacterTags, CharacterLevel);

		WeaponProficiencyBonus = FMath::Max(WeaponProficiencyBonus, Calculation.GetValue());
	}

	// Melee attack modifier = Strength modifier (or optionally Dexterity for a finesse weapon) + proficiency bonus +
	// other bonuses + penalties
	//
	// Ranged attack modifier = Dexterity modifier + proficiency bonus + other bonuses + penalties
	//
	// Source: Pathfinder 2E Core Rulebook, Chapter 6, page 278, "Attack Rolls".
	AttackRoll = DiceRoll + AttackAbilityModifier + WeaponProficiencyBonus + MultipleAttackPenalty;
	Result     = CheckAgainstDifficultyClass(AttackRoll, TargetArmorClass);

	// If you rolled a 20 on the die (a “natural 20”), your result is one degree of success better than it would be by
	// numbers alone."
	if (bIsNatural20 && (Result != EPF2CheckResult::CriticalSuccess))
	{
		Result = IncreaseDegreeOfSuccess(Result);
	}

	UE_LOG(
		LogPf2CoreAbilities,
		VeryVerbose,
		TEXT("Attack Roll (%d%s) + Attack Ability Modifier (%f) + Weapon Proficiency Bonus (%f) + Multiple Attack Penalty (%f) = %f vs. AC %f: %s."),
		DiceRoll,
		bIsNatural20 ? TEXT(" [CRIT]") : TEXT(""),
		AttackAbilityModifier,
		WeaponProficiencyBonus,
		MultipleAttackPenalty,
		AttackRoll,
		TargetArmorClass,
		*(PF2EnumUtilities::ToString(Result))
	);

	return Result;
}

float UPF2AttackStatLibrary::CalculateDamageRoll(const FName DamageDie, const float DamageAbilityModifier)
{
	float       DamageRoll;
	const int32 DamageDieRoll = UPF2DiceLibrary::RollStringSum(DamageDie);

	// Melee damage roll  = damage die of weapon or unarmed attack + Strength modifier + bonuses + penalties
	// Ranged damage roll = damage die of weapon + Strength modifier for thrown weapons + bonuses + penalties
	//
	// Source: Pathfinder 2E Core Rulebook, Chapter 6, page 278, "Damage Rolls".
	DamageRoll = DamageDieRoll + DamageAbilityModifier;

	UE_LOG(
		LogPf2CoreAbilities,
		VeryVerbose,
		TEXT("Damage Die Roll (%s => %d) + Damage Ability Modifier (%f) = %f."),
		*(DamageDie.ToString()),
		DamageDieRoll,
		DamageAbilityModifier,
		DamageRoll
	);

	return DamageRoll;
}

float UPF2AttackStatLibrary::CalculateRangePenalty(const float WeaponRangeIncrementCentimeters,
                                                   const float DistanceCentimeters)
{
	// From the Pathfinder 2E Core Rulebook, page 279, "Range":
	// "Ranged and thrown weapons have a range increment. Attacks with these weapons work normally up to that distance.
	// Attack rolls beyond a weapon’s range increment take a –2 penalty for each additional multiple of that increment
	// between you and the target."
	//
	// From the Pathfinder 2E Core Rulebook, page 446, "Range":
	// "Ranged and thrown weapons each have a listed range increment, and attacks with them grow less accurate against
	// targets farther away [...] As long as your target is at or within the listed range increment, also called the
	// first range increment, you take no penalty to the attack roll. If you’re attacking beyond that range increment,
	// you take a –2 penalty for each additional increment beyond the first. You can attempt to attack with a ranged
	// weapon or thrown weapon up to six range increments away, but the farther away you are, the harder it is to hit
	// your target."
	const float RangeIncrement =
		FMath::Max(0, FMath::FloorToFloat((DistanceCentimeters - 1.0f) / WeaponRangeIncrementCentimeters));

	float Penalty = RangeIncrement * RangePenaltyPerIncrement;

	// These are both negative values, so "less than" is equivalent to "greater than" for positive values.
	if (Penalty < MaxRangePenalty)
	{
		Penalty = -UE_MAX_FLT;
	}

	return Penalty;
}

float UPF2AttackStatLibrary::CalculateMaximumRange(const float WeaponRangeIncrementCentimeters)
{
	return WeaponRangeIncrementCentimeters * MaxRangeIncrement;
}

bool UPF2AttackStatLibrary::IsWithinRange(const float WeaponRangeIncrementCentimeters, const float DistanceCentimeters)
{
	const float MaximumRange = CalculateMaximumRange(WeaponRangeIncrementCentimeters);
	const bool  bInRange     = (DistanceCentimeters <= MaximumRange);

	UE_LOG(
		LogPf2CoreAbilities,
		VeryVerbose,
		TEXT("IsWithinRange(%f,%f): %s (Max Range = %f)"),
		WeaponRangeIncrementCentimeters,
		DistanceCentimeters,
		bInRange ? TEXT("true") : TEXT("false"),
		MaximumRange
	);

	return bInRange;
}

EPF2CheckResult UPF2AttackStatLibrary::CheckAgainstDifficultyClass(const float Value, const float DifficultyClass)
{
	EPF2CheckResult Result;

	if (Value >= DifficultyClass + 10.0f)
	{
		Result = EPF2CheckResult::CriticalSuccess;
	}
	else if (Value >= DifficultyClass)
	{
		Result = EPF2CheckResult::Success;
	}
	else if (Value <= DifficultyClass - 10.0f)
	{
		Result = EPF2CheckResult::CriticalFailure;
	}
	else
	{
		Result = EPF2CheckResult::Failure;
	}

	return Result;
}
