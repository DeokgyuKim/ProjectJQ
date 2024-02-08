#pragma once

//물리 경로를 최소화하기 위한 파일 경로 관련 함수입니다.

/*
 *	Example!
 *	
 *	inline FString GetCharacterBlueprintPath(const TCHAR* InCharacterBlueprintName)
 *	{
 *		return FString::Printf(TEXT("/Script/Engine.Blueprint'/Game/DieselPunk/Blueprints/Characters/%s.%s_C'"), InCharacterBlueprintName, InCharacterBlueprintName);
 *	}
 */

namespace UtilPath
{
	template<typename T>
	FString EnumToString( T InValue )
	{
		UEnum* enumClass = StaticEnum<T>();
		if (!enumClass) return FString();

		return enumClass->GetNameStringByValue((int64)InValue);
	}

	template<typename T>
	T StringToEnum( const FString& InString, T InDefault = T::Max )
	{
		UEnum* enumClass = StaticEnum<T>();
		if (!enumClass)
			return InDefault;

		int32 index = enumClass->GetValueByName(FName(InString));
		return (index != INDEX_NONE) ? (T)index : InDefault;
	}
	
	//데이터 테이블 Path를 생성합니다.
	inline FString GetDataTablePath(const TCHAR* InTableName)
	{
		return FString::Printf(TEXT("/Script/Engine.DataTable'/Game/ProjectJQ/Data/DataTable/DT_%s.DT_%s'"), InTableName, InTableName);
	}
}