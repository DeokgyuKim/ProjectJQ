#pragma once

#include <Misc/EnumRange.h>

// ObjectManagementTargetType입니다.
UENUM(BlueprintType)
enum class EObjectManagementTargetType : uint8
{
	Player			UMETA( ToolTip = "플레이어"),
	Monster			UMETA( ToolTip = "몬스터"),
	None			UMETA( ToolTip = "지정되지 않음"),
};
ENUM_RANGE_BY_COUNT(EObjectManagementTargetType, EObjectManagementTargetType::None);