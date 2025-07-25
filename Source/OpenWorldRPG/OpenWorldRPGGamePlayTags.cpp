// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenWorldRPGGamePlayTags.h"

namespace LyraGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_DataAvailable, "InitState.DataAvailable", "2: All required data has been loaded/replicated and is ready for initialization");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_DataInitialized, "InitState.DataInitialized", "3: The available data has been initialized for this actor/component, but it is not ready for full gameplay");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(DAMAGE_DAMAGE_OVER_TIME, "Damage.DamageOverTime", "applies damage every 5 seconds");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(STATUS_PoisonBuildUp, "GE.STATUS.PoisonBuildUp", "Poison build up status effect");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(STATUS_PoisonDecay, "GE.STATUS.PoisonDecay", "Poison status effect decay");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(STATUS_Poisoned, "GE.STATUS.Poisoned", "Poisoned status effect");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(STATUS_FrostBuildUp, "GE.STATUS.FrostBuildUp", "Frost build up status effect");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(STATUS_FrostDecay, "GE.STATUS.FrostDecay", "Frost Status effect decay");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(STATUS_Frostbite, "GE.STATUS.Frostbite", "Frostbite status effect");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ABILITY_Backstep, "Ability.Movement.Backstep","");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Backstep, "State.Movement.Backstep","");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ABILITY_Movement, "Ability.Movement","");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Invulnerable, "State.Invulnerable","");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Data_Fragment_Amount, "Data.Fragment.Amount","");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Character_Death, "Event.Character.Death","");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Use_HealingHerb,"Ability.Use.HealingHerb","");
}
