#pragma once

//언리얼 에디터에 로그를 띄웁니다.
#define LOG_SCREEN(Color, Text, ...)			\
	GEngine->AddOnScreenDebugMessage(-1, 3.f, Color, FString::Printf(Text, __VA_ARGS__));

//데이터 테이블을 로드합니다.
#define LOAD_DATA_TABLE(TableName)	\
	LoadObject<UDataTable>(nullptr, *UtilPath::GetDataTablePath(TEXT(#TableName)))