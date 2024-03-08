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
	Q	UMETA(ToolTip = "백분율"),
	W	UMETA(ToolTip = "상수"),
	E	UMETA(ToolTip = "상수"),
	R	UMETA(ToolTip = "상수"),
	None	UMETA(Hidden),
};
ENUM_RANGE_BY_COUNT(ESkillInputKey, ESkillInputKey::None);