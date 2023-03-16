// Copyright (c) Ignasi Pelayo, 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ToolMenuDelegates.h"
#include "Modules/ModuleManager.h"

class XENOSEDITORMENU_API FXenosEditorMenuModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	
	virtual void ShutdownModule() override;

	static FXenosEditorMenuModule& GetModule();

	void RegisterCustomEditorMenu(const FNewToolMenuDelegateLegacy& MainSectionNewToolMenuDelegate,
		const FName& EditorMenuName = "Xenos", const FName& EditorMenuTooltipName = "Xenos");

	UToolMenu* GetMenu() const;

	static FName GetStyleSetName();

	void AddStyle(const FName& PropertyName, const FString& Path, const FString& LName, const float& Dimensions);

	const FSlateBrush* GetBrush(const FName& PropertyName) const;

protected:
	void InitializeStyleSet();

	void ReleaseStyleSet();
	
protected:
	UToolMenu* CustomEditorMenu = nullptr;

	TSharedPtr<FSlateStyleSet> CustomEditorMenuStyle = nullptr;
};
