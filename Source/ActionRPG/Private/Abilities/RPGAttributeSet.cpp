// Based on "Action RPG Sample", Copyright 2020, Epic Games, Inc. All Rights Reserved.
// Teen Wolf RPG game logic, Copyright 2021, Teen Wolf RPG. All Rights Reserved.
// Content from Pathfinder 2nd Edition is licensed under the Open Game License (OGL) v1.0a, subject to the following:
//	 - Open Game License v 1.0a, Copyright 2000, Wizards of the Coast, Inc.
//	 - System Reference Document, Copyright 2000, Wizards of the Coast, Inc.
//	 - Pathfinder Core Rulebook (Second Edition), Copyright 2019, Paizo Inc.
// Except for material designated as Product Identity or External Tools, the game mechanics of this Teen Wolf RPG
// product are Open Game Content, as defined in the Open Game License version 1.0a, Section 1(d) (see accompanying
// LICENSE.TXT). No portion of this work other than the material designated as Open Game Content may be reproduced in
// any form without written permission.

#include "Abilities/RPGAttributeSet.h"
#include "Abilities/RPGAbilitySystemComponent.h"
#include "RPGCharacterBase.h"

#include <GameplayEffect.h>
#include <GameplayEffectExtension.h>
#include <Kismet/KismetMathLibrary.h>

URPGAttributeSet::URPGAttributeSet()
	: Experience(0.0f)
	, HitPoints(1.0f)
	, MaxHitPoints(1.0f)
	, AbStrength(0.0f)
	, AbStrengthModifier(0.0f)
	, AbDexterity(10.0f)
	, AbDexterityModifier(0.0f)
	, AbConstitution(10.0f)
	, AbConstitutionModifier(0.0f)
	, AbIntelligence(10.0f)
	, AbIntelligenceModifier(0.0f)
	, AbWisdom(10.0f)
	, AbWisdomModifier(0.0f)
	, AbCharisma(10.0f)
	, AbCharismaModifier(0.0f)
	, CdcDifficultyClass(10.0f)
	, Speed(1.0f)
	, MaxSpeed(1.0f)
	, ArmorClass(10.0f)
	, StFortitudeModifier(0.0f)
	, StReflexModifier(0.0f)
	, StWillModifier(0.0f)
	, PcpPerceptionModifier(0.0f)
	, SkAcrobaticsProficiency(0.0f)
	, SkArcanaProficiency(0.0f)
	, SkAthleticsProficiency(0.0f)
	, SkCraftingProficiency(0.0f)
	, SkDeceptionProficiency(0.0f)
	, SkDiplomacyProficiency(0.0f)
	, SkIntimidationProficiency(0.0f)
	, SkLore1Modifier(0.0f)
	, SkLore2Modifier(0.0f)
	, SkMedicineProficiency(0.0f)
	, SkNatureProficiency(0.0f)
	, SkOccultismProficiency(0.0f)
	, SkPerformanceProficiency(0.0f)
	, SkReligionProficiency(0.0f)
	, SkSocietyProficiency(0.0f)
	, SkStealthProficiency(0.0f)
	, SkSurvivalProficiency(0.0f)
	, SkThieveryProficiency(0.0f)
	, AttackPower(1.0f)
	, DefensePower(1.0f)
	, Damage(0.0f)
{
}

void URPGAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URPGAttributeSet, Experience);
	DOREPLIFETIME(URPGAttributeSet, HitPoints);
	DOREPLIFETIME(URPGAttributeSet, MaxHitPoints);
	DOREPLIFETIME(URPGAttributeSet, AbStrength);
	DOREPLIFETIME(URPGAttributeSet, AbStrengthModifier);
	DOREPLIFETIME(URPGAttributeSet, AbDexterity);
	DOREPLIFETIME(URPGAttributeSet, AbDexterityModifier);
	DOREPLIFETIME(URPGAttributeSet, AbConstitution);
	DOREPLIFETIME(URPGAttributeSet, AbConstitutionModifier);
	DOREPLIFETIME(URPGAttributeSet, AbIntelligence);
	DOREPLIFETIME(URPGAttributeSet, AbIntelligenceModifier);
	DOREPLIFETIME(URPGAttributeSet, AbWisdom);
	DOREPLIFETIME(URPGAttributeSet, AbWisdomModifier);
	DOREPLIFETIME(URPGAttributeSet, AbCharisma);
	DOREPLIFETIME(URPGAttributeSet, AbCharismaModifier);
	DOREPLIFETIME(URPGAttributeSet, CdcDifficultyClass);
	DOREPLIFETIME(URPGAttributeSet, Speed);
	DOREPLIFETIME(URPGAttributeSet, MaxSpeed);
	DOREPLIFETIME(URPGAttributeSet, ArmorClass);
	DOREPLIFETIME(URPGAttributeSet, StFortitudeModifier);
	DOREPLIFETIME(URPGAttributeSet, StReflexModifier);
	DOREPLIFETIME(URPGAttributeSet, StWillModifier);
	DOREPLIFETIME(URPGAttributeSet, PcpPerceptionModifier);
	DOREPLIFETIME(URPGAttributeSet, SkAcrobaticsProficiency);
	DOREPLIFETIME(URPGAttributeSet, SkArcanaProficiency);
	DOREPLIFETIME(URPGAttributeSet, SkAthleticsProficiency);
	DOREPLIFETIME(URPGAttributeSet, SkCraftingProficiency);
	DOREPLIFETIME(URPGAttributeSet, SkDeceptionProficiency);
	DOREPLIFETIME(URPGAttributeSet, SkDiplomacyProficiency);
	DOREPLIFETIME(URPGAttributeSet, SkIntimidationProficiency);
	DOREPLIFETIME(URPGAttributeSet, SkLore1Modifier);
	DOREPLIFETIME(URPGAttributeSet, SkLore2Modifier);
	DOREPLIFETIME(URPGAttributeSet, SkMedicineProficiency);
	DOREPLIFETIME(URPGAttributeSet, SkNatureProficiency);
	DOREPLIFETIME(URPGAttributeSet, SkOccultismProficiency);
	DOREPLIFETIME(URPGAttributeSet, SkPerformanceProficiency);
	DOREPLIFETIME(URPGAttributeSet, SkReligionProficiency);
	DOREPLIFETIME(URPGAttributeSet, SkSocietyProficiency);
	DOREPLIFETIME(URPGAttributeSet, SkStealthProficiency);
	DOREPLIFETIME(URPGAttributeSet, SkSurvivalProficiency);
	DOREPLIFETIME(URPGAttributeSet, SkThieveryProficiency);

	DOREPLIFETIME(URPGAttributeSet, AttackPower);
	DOREPLIFETIME(URPGAttributeSet, DefensePower);
}

void URPGAttributeSet::OnRep_Experience(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Experience, OldValue);
}

void URPGAttributeSet::OnRep_HitPoints(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, HitPoints, OldValue);
}

void URPGAttributeSet::OnRep_MaxHitPoints(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MaxHitPoints, OldValue);
}

void URPGAttributeSet::OnRep_AbStrength(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, AbStrength, OldValue);
}

void URPGAttributeSet::OnRep_AbStrengthModifier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, AbStrengthModifier, OldValue);
}

void URPGAttributeSet::OnRep_AbDexterity(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, AbDexterity, OldValue);
}

void URPGAttributeSet::OnRep_AbDexterityModifier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, AbDexterityModifier, OldValue);
}

void URPGAttributeSet::OnRep_AbConstitution(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, AbConstitution, OldValue);
}

void URPGAttributeSet::OnRep_AbConstitutionModifier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, AbConstitutionModifier, OldValue);
}

void URPGAttributeSet::OnRep_AbIntelligence(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, AbIntelligence, OldValue);
}

void URPGAttributeSet::OnRep_AbIntelligenceModifier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, AbIntelligenceModifier, OldValue);
}

void URPGAttributeSet::OnRep_AbWisdom(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, AbWisdom, OldValue);
}

void URPGAttributeSet::OnRep_AbWisdomModifier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, AbWisdomModifier, OldValue);
}

void URPGAttributeSet::OnRep_AbCharisma(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, AbCharisma, OldValue);
}

void URPGAttributeSet::OnRep_AbCharismaModifier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, AbCharismaModifier, OldValue);
}

void URPGAttributeSet::OnRep_CdcDifficultyClass(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, CdcDifficultyClass, OldValue);
}

void URPGAttributeSet::OnRep_Speed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Speed, OldValue);
}

void URPGAttributeSet::OnRep_MaxSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MaxSpeed, OldValue);
}

void URPGAttributeSet::OnRep_ArmorClass(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, ArmorClass, OldValue);
}

void URPGAttributeSet::OnRep_StFortitudeModifier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, StFortitudeModifier, OldValue);
}

void URPGAttributeSet::OnRep_StReflexModifier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, StReflexModifier, OldValue);
}

void URPGAttributeSet::OnRep_StWillModifier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, StWillModifier, OldValue);
}

void URPGAttributeSet::OnRep_PcpPerceptionModifier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, PcpPerceptionModifier, OldValue);
}

void URPGAttributeSet::OnRep_SkAcrobaticsProficiency(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, SkAcrobaticsProficiency, OldValue);
}

void URPGAttributeSet::OnRep_SkArcanaProficiency(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, SkArcanaProficiency, OldValue);
}

void URPGAttributeSet::OnRep_SkAthleticsProficiency(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, SkAthleticsProficiency, OldValue);
}

void URPGAttributeSet::OnRep_SkCraftingProficiency(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, SkCraftingProficiency, OldValue);
}

void URPGAttributeSet::OnRep_SkDeceptionProficiency(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, SkDeceptionProficiency, OldValue);
}

void URPGAttributeSet::OnRep_SkDiplomacyProficiency(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, SkDiplomacyProficiency, OldValue);
}

void URPGAttributeSet::OnRep_SkIntimidationProficiency(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, SkIntimidationProficiency, OldValue);
}

void URPGAttributeSet::OnRep_SkLore1Modifier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, SkLore1Modifier, OldValue);
}

void URPGAttributeSet::OnRep_SkLore2Modifier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, SkLore2Modifier, OldValue);
}

void URPGAttributeSet::OnRep_SkMedicineProficiency(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, SkMedicineProficiency, OldValue);
}

void URPGAttributeSet::OnRep_SkNatureProficiency(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, SkNatureProficiency, OldValue);
}

void URPGAttributeSet::OnRep_SkOccultismProficiency(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, SkOccultismProficiency, OldValue);
}

void URPGAttributeSet::OnRep_SkPerformanceProficiency(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, SkPerformanceProficiency, OldValue);
}

void URPGAttributeSet::OnRep_SkReligionProficiency(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, SkReligionProficiency, OldValue);
}

void URPGAttributeSet::OnRep_SkSocietyProficiency(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, SkSocietyProficiency, OldValue);
}

void URPGAttributeSet::OnRep_SkStealthProficiency(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, SkStealthProficiency, OldValue);
}

void URPGAttributeSet::OnRep_SkSurvivalProficiency(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, SkSurvivalProficiency, OldValue);
}

void URPGAttributeSet::OnRep_SkThieveryProficiency(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, SkThieveryProficiency, OldValue);
}

void URPGAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, AttackPower, OldValue);
}

void URPGAttributeSet::OnRep_DefensePower(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, DefensePower, OldValue);
}

void URPGAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute,
												   const FGameplayAttributeData& MaxAttribute,
												   float NewMaxValue,
												   const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComp	 = GetOwningAbilitySystemComponent();
	const float				 CurrentMaxValue = MaxAttribute.GetCurrentValue();

	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		// Change current value to maintain the current Val / Max percent
		const float CurrentValue = AffectedAttribute.GetCurrentValue();

		float NewDelta =
			(CurrentMaxValue > 0.0f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

void URPGAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// This is called whenever attributes change, so for max health we want to scale the current totals to match
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHitPointsAttribute())
	{
		AdjustAttributeForMaxChange(
			HitPoints,
			MaxHitPoints,
			NewValue,
			GetHitPointsAttribute()
		);
	}
}

void URPGAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle	Context	   = Data.EffectSpec.GetContext();
	UAbilitySystemComponent*		Source	   = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer&	SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	// Compute the delta between old and new, if it is available
	float DeltaValue = 0;

	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
	{
		// If this was additive, store the raw delta value to be passed along later
		DeltaValue = Data.EvaluatedData.Magnitude;
	}

	// Get the Target actor, which should be our owner
	AActor*				TargetActor		 = nullptr;
	AController*		TargetController = nullptr;
	ARPGCharacterBase*	TargetCharacter	 = nullptr;

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor		 = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter  = Cast<ARPGCharacterBase>(TargetActor);
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Get the Source actor
		AActor*				SourceActor		 = nullptr;
		AController*		SourceController = nullptr;
		ARPGCharacterBase*	SourceCharacter  = nullptr;

		if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
		{
			SourceActor		 = Source->AbilityActorInfo->AvatarActor.Get();
			SourceController = Source->AbilityActorInfo->PlayerController.Get();

			if (SourceController == nullptr && SourceActor != nullptr)
			{
				if (APawn* Pawn = Cast<APawn>(SourceActor))
				{
					SourceController = Pawn->GetController();
				}
			}

			// Use the controller to find the source pawn
			if (SourceController)
			{
				SourceCharacter = Cast<ARPGCharacterBase>(SourceController->GetPawn());
			}
			else
			{
				SourceCharacter = Cast<ARPGCharacterBase>(SourceActor);
			}

			// Set the causer actor based on context if it's set
			if (Context.GetEffectCauser())
			{
				SourceActor = Context.GetEffectCauser();
			}
		}

		// Try to extract a hit result
		FHitResult HitResult;

		if (Context.GetHitResult())
		{
			HitResult = *Context.GetHitResult();
		}

		// Store a local copy of the amount of damage done and clear the damage attribute
		const float LocalDamageDone = GetDamage();
		SetDamage(0.0f);

		if (LocalDamageDone > 0)
		{
			// Apply the health change and then clamp it
			const float OldHitPoints = GetHitPoints();

			SetHitPoints(FMath::Clamp(OldHitPoints - LocalDamageDone, 0.0f, GetMaxHitPoints()));

			UE_LOG(
				// TODO: Change to LogTwRpgStatsDebug after moving this code into the TW RPG module.
				LogActionRPGStatsDebug,
				VeryVerbose,
				TEXT("Damage: %s - Old HitPoints: %f, Damage: %f, New HitPoints: %f"),
				*(TargetCharacter->GetName()),
				OldHitPoints,
				LocalDamageDone,
				OldHitPoints - LocalDamageDone
			);

			if (TargetCharacter)
			{
				// This is proper damage
				TargetCharacter->HandleDamage(LocalDamageDone, HitResult, SourceTags, SourceCharacter, SourceActor);

				// Call for all health changes
				TargetCharacter->HandleHitPointsChanged(-LocalDamageDone, SourceTags);
			}
		}
	}
	else if (Data.EvaluatedData.Attribute == GetHitPointsAttribute())
	{
		// Handle other health changes such as from healing or direct modifiers
		// First clamp it
		SetHitPoints(FMath::Clamp(GetHitPoints(), 0.0f, GetMaxHitPoints()));

		if (TargetCharacter)
		{
			// Call for all health changes
			TargetCharacter->HandleHitPointsChanged(DeltaValue, SourceTags);
		}
	}
	else if (Data.EvaluatedData.Attribute == GetSpeedAttribute())
	{
		if (TargetCharacter)
		{
			// Call for all speed changes
			TargetCharacter->HandleMoveSpeedChanged(DeltaValue, SourceTags);
		}
	}
}
