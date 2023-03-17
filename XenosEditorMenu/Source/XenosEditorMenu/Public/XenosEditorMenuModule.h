// Copyright (c) Ignasi Pelayo, 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

struct FXenosEditorMenuConfig;

struct XENOSEDITORMENU_API FXenosEditorMenu
{
	FName MenuName;
	
	UToolMenu* Menu = nullptr;

	TArray<TSharedPtr<FWorkspaceItem>> XenosEditorMenuCategories;

	TSharedPtr<FWorkspaceItem> GetCategoryByDisplayName(const FString& CategoryName = "Main");
};

class XENOSEDITORMENU_API FXenosEditorMenuModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	
	virtual void ShutdownModule() override;

	static FXenosEditorMenuModule& GetModule();

	void RegisterXenosEditorMenu();

	FXenosEditorMenu RegisterXenosEditorMenu(const FXenosEditorMenuConfig& XenosEditorMenuData);

	TArray<TSharedPtr<FWorkspaceItem>> GetXenosEditorMenuTabStructure(const FXenosEditorMenuConfig& XenosEditorMenuConfig);

	FXenosEditorMenu GetXenosEditorMenu(const FName& XenosEditorMenuName = NAME_None) const;

	static FName GetStyleSetName();

	void AddStyle(const FName& PropertyName, const FString& Path, const FString& LName, const float& Dimensions);

	const FSlateBrush* GetBrush(const FName& PropertyName) const;

protected:
	void InitializeStyleSet();

	void ReleaseStyleSet();
	
protected:
	FXenosEditorMenu XenosEditorMenu;

	TArray<FXenosEditorMenu> AdditionalXenosEditorMenus = TArray<FXenosEditorMenu>();

	TSharedPtr<FSlateStyleSet> CustomEditorMenuStyle = nullptr;
};
