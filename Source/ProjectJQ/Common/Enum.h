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