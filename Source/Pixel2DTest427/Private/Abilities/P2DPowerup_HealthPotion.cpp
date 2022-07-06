// Fill out your copyright notice in the Description page of Project Settings.


#define LOCTEXT_NAMESPACE "InteractableActors"
#include "Abilities/P2DPowerup_HealthPotion.h"

#include "Abilities/P2DAttributeComponent.h"
#include "Items/P2DPotionItem.h"
#include "Items/P2DRPGItem.h"


AP2DPowerup_HealthPotion::AP2DPowerup_HealthPotion()
{
}


void AP2DPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	UP2DAttributeComponent* AttributeComp = UP2DAttributeComponent::GetAttributes(InstigatorPawn);
	// Check if not already at max health
	if (ensure(AttributeComp) && ensure(ItemData) && !AttributeComp->IsFullHealth())
	{
		if(const UP2DPotionItem* PotionItem = Cast<UP2DPotionItem>(ItemData))
		{
			if (AttributeComp->ApplyHealthChange(this, PotionItem->Value))
			{
				// Only activate if healed successfully
				HideAndCooldownPowerup();
			}
		}
	}
}

FText AP2DPowerup_HealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	UP2DAttributeComponent* AttributeComp = UP2DAttributeComponent::GetAttributes(InstigatorPawn);
	if (AttributeComp && AttributeComp->IsFullHealth())
	{
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health.");
	}

	return LOCTEXT("HealthPotion_InteractMessage", "Restores health to maximum.");
}

#undef LOCTEXT_NAMESPACE
