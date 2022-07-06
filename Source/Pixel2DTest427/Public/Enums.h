#pragma once

UENUM(BlueprintType)
enum class EAmmoType: uint8
{
	Arrow,
	Bolt,
	Bullet,
	Dart,
	EAT_MAX
};

UENUM(BlueprintType)
enum class EAttributes: uint8
{
	Health,
	Energy,
	Stamina,
	Score,
	Gold,
	EA_MAX
};

UENUM(BlueprintType)
enum class EDirection: uint8
{
	Left,
	Right
};

UENUM(BlueprintType)
enum class EFaction: uint8
{
	Player,
	Enemy,
	NPC
};

UENUM(BlueprintType)
enum class EFloatingText: uint8
{
	Damage,
	Health,
	Gold,
	Item,
	Experience
};

UENUM(BlueprintType)
enum class EItemCategories: uint8
{
	Consumable,
	Weapon,
	Ability,
	Ranged,
	Armor,
	QuestItem,
	Readable,
	Junk,
	Ammunition,
	Accessory
};

UENUM(BlueprintType)
enum class EMovementAxis: uint8
{
	Horizontal,
	Vertical
};

UENUM(BlueprintType)
enum class EOrientationAxis: uint8
{
	Horizontal,
	Vertical
};

UENUM(BlueprintType)
enum class ERotationDirection: uint8
{
	Clockwise,
	CounterClockwise
};

UENUM(BlueprintType)
enum class EStartStop: uint8
{
	Start,
	Stop
};

UENUM(BlueprintType)
enum class EStateChange: uint8
{
	Increased,
	Decreased
};

UENUM(BlueprintType)
enum class EStatusEffects: uint8
{
	Stunned,
	DamageOverTime,
	Slowed,
	Vulnerable
};

UENUM(BlueprintType)
enum class EViewMode: uint8
{
	SideView,
	TopDownView
};

UENUM(BlueprintType)
enum class EUIWidget: uint8
{
	Inventory,
	Equipment,
	ActionBar
};
