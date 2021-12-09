﻿// OpenPF2 for UE Game Logic, Copyright 2021, Guy Elsmore-Paddock. All Rights Reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "Tests/PF2SpecBase.h"

#include "PF2GameplayAbilityUtilities.h"
#include "Abilities/PF2AttributeSet.h"

template <typename BlueprintType>
TSubclassOf<BlueprintType> FPF2SpecBase::LoadBlueprint(const FString FolderPath, const FString BlueprintName)
{
	const FString ObjectPath =
		FString::Printf(TEXT("BlueprintGeneratedClass'%s/%s.%s_C'"), *FolderPath, *BlueprintName, *BlueprintName);

	const TSoftClassPtr<BlueprintType> ObjectClass =
		TSoftClassPtr<BlueprintType>(FSoftObjectPath(ObjectPath));

	return ObjectClass.LoadSynchronous();
}

FAttributeCapture FPF2SpecBase::CaptureAttributes(const UPF2AttributeSet* AttributeSet)
{
	FAttributeCapture Capture =
	{
		{ TEXT("Experience"),           const_cast<FGameplayAttributeData *>(&AttributeSet->Experience)           },
		{ TEXT("AbBoostCount"),         const_cast<FGameplayAttributeData *>(&AttributeSet->AbBoostCount)         },
		{ TEXT("AbBoostLimit"),         const_cast<FGameplayAttributeData *>(&AttributeSet->AbBoostLimit)         },
		{ TEXT("ClassDifficultyClass"), const_cast<FGameplayAttributeData *>(&AttributeSet->ClassDifficultyClass) },
		{ TEXT("Speed"),                const_cast<FGameplayAttributeData *>(&AttributeSet->Speed)                },
		{ TEXT("MaxSpeed"),             const_cast<FGameplayAttributeData *>(&AttributeSet->MaxSpeed)             },
		{ TEXT("ArmorClass"),           const_cast<FGameplayAttributeData *>(&AttributeSet->ArmorClass)           },
		{ TEXT("HitPoints"),            const_cast<FGameplayAttributeData *>(&AttributeSet->HitPoints)            },
		{ TEXT("MaxHitPoints"),         const_cast<FGameplayAttributeData *>(&AttributeSet->MaxHitPoints)         },
		{ TEXT("PerceptionModifier"),   const_cast<FGameplayAttributeData *>(&AttributeSet->PerceptionModifier)   },
		{ TEXT("FeAncestryFeatCount"),  const_cast<FGameplayAttributeData *>(&AttributeSet->FeAncestryFeatCount)  },
		{ TEXT("FeAncestryFeatLimit"),  const_cast<FGameplayAttributeData *>(&AttributeSet->FeAncestryFeatLimit)  },
		{ TEXT("TmpDamageIncoming"),    const_cast<FGameplayAttributeData *>(&AttributeSet->TmpDamageIncoming)    },
	};

	Capture.Append(CaptureAbilityAttributes(AttributeSet));
	Capture.Append(CaptureAbilityModifierAttributes(AttributeSet));
	Capture.Append(CaptureSavingThrowAttributes(AttributeSet));
	Capture.Append(CaptureSkillModifierAttributes(AttributeSet));
	Capture.Append(CaptureSpellAttributes(AttributeSet));

	return Capture;
}

FAttributeCapture FPF2SpecBase::CaptureAbilityAttributes(const UPF2AttributeSet* AttributeSet)
{
	FAttributeCapture Capture =
	{
		{ TEXT("AbCharisma"),     const_cast<FGameplayAttributeData *>(&AttributeSet->AbCharisma) },
		{ TEXT("AbConstitution"), const_cast<FGameplayAttributeData *>(&AttributeSet->AbConstitution) },
		{ TEXT("AbDexterity"),    const_cast<FGameplayAttributeData *>(&AttributeSet->AbDexterity) },
		{ TEXT("AbIntelligence"), const_cast<FGameplayAttributeData *>(&AttributeSet->AbIntelligence) },
		{ TEXT("AbStrength"),     const_cast<FGameplayAttributeData *>(&AttributeSet->AbStrength) },
		{ TEXT("AbWisdom"),       const_cast<FGameplayAttributeData *>(&AttributeSet->AbWisdom) },
	};

	return Capture;
}

FAttributeCapture FPF2SpecBase::CaptureAbilityModifierAttributes(const UPF2AttributeSet* AttributeSet)
{
	FAttributeCapture Capture =
	{
		{ TEXT("AbCharismaModifier"),     const_cast<FGameplayAttributeData *>(&AttributeSet->AbCharismaModifier) },
		{ TEXT("AbConstitutionModifier"), const_cast<FGameplayAttributeData *>(&AttributeSet->AbConstitutionModifier) },
		{ TEXT("AbDexterityModifier"),    const_cast<FGameplayAttributeData *>(&AttributeSet->AbDexterityModifier) },
		{ TEXT("AbIntelligenceModifier"), const_cast<FGameplayAttributeData *>(&AttributeSet->AbIntelligenceModifier) },
		{ TEXT("AbStrengthModifier"),     const_cast<FGameplayAttributeData *>(&AttributeSet->AbStrengthModifier) },
		{ TEXT("AbWisdomModifier"),       const_cast<FGameplayAttributeData *>(&AttributeSet->AbWisdomModifier) },
	};
	return Capture;
}

FAttributeCapture FPF2SpecBase::CaptureSavingThrowAttributes(const UPF2AttributeSet* AttributeSet)
{
	FAttributeCapture Capture =
	{
		{ TEXT("StFortitudeModifier"), const_cast<FGameplayAttributeData *>(&AttributeSet->StFortitudeModifier) },
		{ TEXT("StReflexModifier"),    const_cast<FGameplayAttributeData *>(&AttributeSet->StReflexModifier) },
		{ TEXT("StWillModifier"),      const_cast<FGameplayAttributeData *>(&AttributeSet->StWillModifier) },
	};

	return Capture;
}

FAttributeCapture FPF2SpecBase::CaptureSkillModifierAttributes(const UPF2AttributeSet* AttributeSet)
{
	FAttributeCapture Capture =
	{
		{ TEXT("SkAcrobaticsModifier"),   const_cast<FGameplayAttributeData *>(&AttributeSet->SkAcrobaticsModifier) },
		{ TEXT("SkArcanaModifier"),       const_cast<FGameplayAttributeData *>(&AttributeSet->SkArcanaModifier) },
		{ TEXT("SkAthleticsModifier"),    const_cast<FGameplayAttributeData *>(&AttributeSet->SkAthleticsModifier) },
		{ TEXT("SkCraftingModifier"),     const_cast<FGameplayAttributeData *>(&AttributeSet->SkCraftingModifier) },
		{ TEXT("SkDeceptionModifier"),    const_cast<FGameplayAttributeData *>(&AttributeSet->SkDeceptionModifier) },
		{ TEXT("SkDiplomacyModifier"),    const_cast<FGameplayAttributeData *>(&AttributeSet->SkDiplomacyModifier) },
		{ TEXT("SkIntimidationModifier"), const_cast<FGameplayAttributeData *>(&AttributeSet->SkIntimidationModifier) },
		{ TEXT("SkLore1Modifier"),        const_cast<FGameplayAttributeData *>(&AttributeSet->SkLore1Modifier) },
		{ TEXT("SkLore2Modifier"),        const_cast<FGameplayAttributeData *>(&AttributeSet->SkLore2Modifier) },
		{ TEXT("SkMedicineModifier"),     const_cast<FGameplayAttributeData *>(&AttributeSet->SkMedicineModifier) },
		{ TEXT("SkNatureModifier"),       const_cast<FGameplayAttributeData *>(&AttributeSet->SkNatureModifier) },
		{ TEXT("SkOccultismModifier"),    const_cast<FGameplayAttributeData *>(&AttributeSet->SkOccultismModifier) },
		{ TEXT("SkPerformanceModifier"),  const_cast<FGameplayAttributeData *>(&AttributeSet->SkPerformanceModifier) },
		{ TEXT("SkReligionModifier"),     const_cast<FGameplayAttributeData *>(&AttributeSet->SkReligionModifier) },
		{ TEXT("SkSocietyModifier"),      const_cast<FGameplayAttributeData *>(&AttributeSet->SkSocietyModifier) },
		{ TEXT("SkStealthModifier"),      const_cast<FGameplayAttributeData *>(&AttributeSet->SkStealthModifier) },
		{ TEXT("SkSurvivalModifier"),     const_cast<FGameplayAttributeData *>(&AttributeSet->SkSurvivalModifier) },
		{ TEXT("SkThieveryModifier"),     const_cast<FGameplayAttributeData *>(&AttributeSet->SkThieveryModifier) },
	};

	return Capture;
}

FAttributeCapture FPF2SpecBase::CaptureSpellAttributes(const UPF2AttributeSet* AttributeSet)
{
	FAttributeCapture Capture =
	{
		{ TEXT("SpellAttackRoll"),      const_cast<FGameplayAttributeData *>(&AttributeSet->SpellAttackRoll) },
		{ TEXT("SpellDifficultyClass"), const_cast<FGameplayAttributeData *>(&AttributeSet->SpellDifficultyClass) },
	};

	return Capture;
}

void FPF2SpecBase::SetupWorld()
{
	FWorldContext& WorldContext = GEngine->CreateNewWorldContext(EWorldType::Game);

	this->World = UWorld::CreateWorld(EWorldType::Game, false);

	WorldContext.SetCurrentWorld(this->World);
}

void FPF2SpecBase::BeginPlay() const
{
	const FURL CommandLineURL;

	this->World->InitializeActorsForPlay(CommandLineURL);
	this->World->BeginPlay();
}

void FPF2SpecBase::DestroyWorld() const
{
	GEngine->DestroyWorldContext(this->World);

	this->World->DestroyWorld(false);
}

void FPF2SpecBase::SetupPawn()
{
	this->TestPawn             = this->World->SpawnActor<APF2TestPawn>();
	this->PawnAbilityComponent = this->TestPawn->GetAbilitySystemComponent();
}

void FPF2SpecBase::DestroyPawn()
{
	if (this->TestPawn)
	{
		this->World->EditorDestroyActor(this->TestPawn, false);
	}

	this->TestPawn = nullptr;
}

FActiveGameplayEffectHandle FPF2SpecBase::ApplyGameEffect(FGameplayAttributeData&             Attribute,
                                                          const float                         StartingValue,
                                                          const TSubclassOf<UGameplayEffect>& EffectBP,
                                                          const float                         CharacterLevel) const
{
	UGameplayEffect* GameplayEffect = EffectBP->GetDefaultObject<UGameplayEffect>();

	Attribute = StartingValue;

	const FActiveGameplayEffectHandle EffectHandle = this->PawnAbilityComponent->ApplyGameplayEffectToTarget(
		GameplayEffect,
		this->PawnAbilityComponent,
		CharacterLevel
	);

	return EffectHandle;
}

void FPF2SpecBase::ApplyUnreplicatedTag(const FString TagName, const float CharacterLevel) const
{
	this->PawnAbilityComponent->AddLooseGameplayTag(PF2GameplayAbilityUtilities::GetTag(TagName));
}

void FPF2SpecBase::RemoveUnreplicatedTag(const FString TagName) const
{
	this->PawnAbilityComponent->RemoveLooseGameplayTag(PF2GameplayAbilityUtilities::GetTag(TagName));
}
