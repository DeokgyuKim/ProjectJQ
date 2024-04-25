#pragma once

#include <Misc/EnumRange.h>

// 오브젝트 관리 대상의 타입입니다.
UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	Player			UMETA(ToolTip = "플레이어"),
	Monster			UMETA(ToolTip = "몬스터"),
	None			UMETA(ToolTip = "지정되지 않음"),
};
ENUM_RANGE_BY_COUNT(ECharacterType, ECharacterType::None);

// 스탯 타입입니다.
UENUM(BlueprintType)
enum class EStatControlType : uint8
{
	Hp			UMETA(ToolTip = "체력"),
	HpMax		UMETA(ToolTip = "최대 체력"),
	Attack		UMETA(ToolTip = "공격력"),
	DetectRange UMETA(ToolTip = "탐지 범위"),
	MoveSpeed	UMETA(ToolTip = "이동 속도"),
	Count		UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EStatControlType, EStatControlType::Count);

// 이펙트 재생 타입
UENUM( BlueprintType )
enum class EEffectPlayType : uint8
{
	Spawn	UMETA(ToolTip = "스폰 시"),
	Death	UMETA(ToolTip = "사망 시"),
	Walk	UMETA(ToolTip = "걸을 때"),
	None	UMETA(Hidden),
};

// 버프 밸류 타입
UENUM( BlueprintType )
enum class EBuffValueType : uint8
{
	Percentage	UMETA(ToolTip = "백분율"),
	Constant	UMETA(ToolTip = "상수"),
};

// 버프 밸류 타입
UENUM( BlueprintType )
enum class ESkillInputKey : uint8
{
	None = 0,
	Q			UMETA(ToolTip = "스킬"),
	W			UMETA(ToolTip = "스킬"),
	E			UMETA(ToolTip = "스킬"),
	R			UMETA(ToolTip = "스킬"),
	BasicAttack	UMETA(ToolTip = "기본공격"),
	End
};
ENUM_RANGE_BY_COUNT(ESkillInputKey, ESkillInputKey::End);

// 버프 밸류 타입
UENUM( BlueprintType )
enum class EGemStoneType : uint8
{
	None = 0,
	Ruby		UMETA(ToolTip = "루비: 공격 및 해당 스킬 강화/딜러"),
	Emerald		UMETA(ToolTip = "에메랄드: 체력, 방어력 및 해당 스킬 강화/탱커"),
	Obsidian	UMETA(ToolTip = "흑요석: 이동, 신속 및 해당 스킬 강화/이동기"),
	Pearl		UMETA(ToolTip = "진주: 회복, 유틸 및 해당 스킬 강화/힐러"),
	Sapphire	UMETA(ToolTip = "사파이어: 마나, 주문력 및 마법 스킬 강화/법사"),
};

//공격 스킬 범위 타입
UENUM(BlueprintType)
enum class EAttackRangeType : uint8
{
	None = 0,
	Box			UMETA(ToolTip = "박스 형태 공격"),
	Sphere		UMETA(ToolTip = "구 형태 공격"),
	Projectile	UMETA(ToolTip = "투사체 공격"),
	Arc			UMETA(TooTip = "호 형태 공격")
};
