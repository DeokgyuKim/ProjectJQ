// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManagementGSS.h"
#include "ProjectJQ/UI/UserWidgetBase.h"

#include <AssetRegistry/AssetRegistryModule.h>
#include <AssetRegistry/AssetData.h>
#include <HAL/FileManagerGeneric.h>

#include "ProjectJQ/Manager/FstreamManager.h"


void UUIManagementGSS::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UIIdGenerator.Initialize(0, 10000);

	World = GetWorld();

#if WITH_EDITOR
	if(GIsEditor)
	{
		TArray<FString> outData;
		FindUIDirectroy(TEXT("ProjectJQ/UI"), outData);
		FFstreamManager::WriteDataBinary(TEXT("UIReferencePath.bin"), outData, TEXT(":"));
	}
#endif

	TArray<FString> inData;
	FFstreamManager::ReadDataBinary(TEXT("UIReferencePath.bin"), inData, TEXT(":"));
	for(int32 i = 0; i < inData.Num(); i += 2)
	{
		UIReferencePath.Add(inData[i], inData[i + 1]);
	}
}

void UUIManagementGSS::Deinitialize()
{
	Super::Deinitialize();
}

int32 UUIManagementGSS::CreateWidgetBase(FString InBPName, FString InWidgetName, AActor* InOwner, UUserWidgetBase* InOwningObject)
{
	FString* referencePathPtr = UIReferencePath.Find(InBPName);
	if(!referencePathPtr)
		return INVALID_UIID;
	
	return CreateWidgetBase(LoadClass<UUserWidgetBase>(nullptr, **referencePathPtr), InWidgetName, InOwner, InOwningObject);
}

int32 UUIManagementGSS::CreateWidgetBase(UClass* InUClass, FString InWidgetName, AActor* InOwner, UUserWidgetBase* InOwningObject)
{
	for(TMap<int32, TWeakObjectPtr<UUserWidgetBase>>::TIterator iter = ManagementTargets.CreateIterator(); iter; ++iter)
	{
		if(!iter->Value.IsValid())
			iter.RemoveCurrent();
	}
	
	World = GetWorld();

	UUserWidgetBase* widgetBase= nullptr;

	if(InOwningObject == nullptr)
		widgetBase = Cast<UUserWidgetBase>(CreateWidget(Controller.Get(), InUClass, *InWidgetName));
	else
		widgetBase = Cast<UUserWidgetBase>(CreateWidget(InOwningObject, InUClass, *InWidgetName));
	
	if(widgetBase == nullptr)
		return INVALID_UIID;

	int32 uiId = UIIdGenerator.GenerateID(); 
	widgetBase->SetUserWidgetId(uiId);
	widgetBase->SetOwnerActor(InOwner);
	widgetBase->OnCreated();

	ManagementTargets.Add(uiId, widgetBase);

	return uiId;
}

void UUIManagementGSS::DestroyWidgetBase(int32 InUserWidgetId)
{
	TWeakObjectPtr<UUserWidgetBase>* findWidget = ManagementTargets.Find(InUserWidgetId);
	if(!findWidget || !findWidget->IsValid())
		return;

	(*findWidget)->RemoveFromParent();
	(*findWidget)->Destruct();

	ManagementTargets.Remove(InUserWidgetId);
}

UUserWidgetBase* UUIManagementGSS::FindWidgetBase(int32 InUserWidgetId)
{
	TWeakObjectPtr<UUserWidgetBase>* findWidget = ManagementTargets.Find(InUserWidgetId);
	if(findWidget && findWidget->IsValid())
		return findWidget->Get();
	
	return nullptr;
}

UClass* UUIManagementGSS::GetUClassWidgetBP(FString InBPName)
{
	FString* referencePathPtr = UIReferencePath.Find(InBPName);
	if(!referencePathPtr)
		return nullptr;

	return LoadClass<UUserWidgetBase>(nullptr, **referencePathPtr);
}

void UUIManagementGSS::FindUIDirectroy(FString InPath, TArray<FString>& OutData)
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
		OutData.Add(assetName);
		
		FString classPath = data.GetObjectPathString();
		classPath = TEXT("/Script/UMGEditor.WidgetBlueprint'") + classPath + TEXT("_C'");
		OutData.Add(classPath);
	}

	//폴더 내 다른 폴더 탐색
	FString folderPath = FPaths::ProjectContentDir() + InPath + TEXT("/*");
	TArray<FString> files;
	IFileManager::Get().FindFiles(files, *folderPath, true, true);
	for(FString& file : files)
	{
		if(!file.Contains(TEXT(".")))
			FindUIDirectroy(InPath + TEXT("/") + file, OutData);
	}
}
