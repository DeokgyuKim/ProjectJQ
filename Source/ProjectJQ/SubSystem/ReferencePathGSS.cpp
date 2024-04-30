// Fill out your copyright notice in the Description page of Project Settings.


#include "ReferencePathGSS.h"

#include <AssetRegistry/AssetRegistryModule.h>
#include <AssetRegistry/AssetData.h>
#include <HAL/FileManagerGeneric.h>


void UReferencePathGSS::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	FindReferencePathInDirectory(TEXT("ProjectJQ/Blueprints/Character"));
}

void UReferencePathGSS::Deinitialize()
{
	Super::Deinitialize();
}

FString UReferencePathGSS::GetReferencePath(FString InAssetName)
{
	FString* findStrPtr = ReferencePath.Find(InAssetName);
	if(findStrPtr == nullptr)
		return TEXT("");

	return *findStrPtr;
}

void UReferencePathGSS::FindReferencePathInDirectory(FString InPath)
{
	//폴더 내 모든 에셋 탐색
	FAssetRegistryModule& assetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));

	TArray<FAssetData> assetList;
	FARFilter Filter;
	FString assetPath = TEXT("/Game/") + InPath;
	Filter.PackagePaths.Add(*assetPath);

	assetRegistryModule.Get().GetAssets(Filter, assetList);

	for(const FAssetData& data : assetList)
	{
		FString assetName = data.AssetName.ToString();
		
		FString classPath = data.GetObjectPathString();
		if(data.AssetClassPath.ToString().Contains(TEXT("Blueprint")))
			classPath = data.AssetClassPath.ToString() + TEXT("'") + classPath + TEXT("_C'");
		else
			classPath = data.AssetClassPath.ToString() + TEXT("'") + classPath + TEXT("'");

		ReferencePath.Add(assetName, classPath);
	}

	//폴더 내 다른 폴더 탐색
	FString folderPath = FPaths::ProjectContentDir() + InPath + TEXT("/*");
	TArray<FString> files;
	IFileManager::Get().FindFiles(files, *folderPath, true, true);
	for(FString& file : files)
	{
		if(!file.Contains(TEXT(".")))
			FindReferencePathInDirectory(InPath + TEXT("/") + file);
	}
}
