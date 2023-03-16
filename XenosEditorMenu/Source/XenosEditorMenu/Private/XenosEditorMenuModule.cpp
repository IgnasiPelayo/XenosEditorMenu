// Copyright (c) Ignasi Pelayo, 2023. All Rights Reserved.

#include "XenosEditorMenuModule.h"

#include "ToolMenus.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

IMPLEMENT_MODULE(FXenosEditorMenuModule, XenosEditorMenu)

#define LOCTEXT_NAMESPACE "FXenosEditorMenuModule"

#define XENOS_IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)

void FXenosEditorMenuModule::StartupModule()
{
	InitializeStyleSet();
}

void FXenosEditorMenuModule::ShutdownModule()
{
	ReleaseStyleSet();
}

FXenosEditorMenuModule& FXenosEditorMenuModule::GetModule()
{
	return FModuleManager::LoadModuleChecked<FXenosEditorMenuModule>("XenosEditorMenu");
}

void FXenosEditorMenuModule::RegisterCustomEditorMenu(const FNewToolMenuDelegateLegacy& MainSectionNewToolMenuDelegate,
	const FName& EditorMenuName, const FName& EditorMenuTooltipName)
{
	UToolMenu* MainMenu = UToolMenus::Get()->ExtendMenu("MainFrame.MainMenu");

	CustomEditorMenu = MainMenu->AddSubMenu("MainMenu", NAME_None, EditorMenuName,
		FText::FromName(EditorMenuName),
		FText::FromString(FString::Printf(TEXT("Open the %s menu"), *EditorMenuTooltipName.ToString())));

	CustomEditorMenu->AddDynamicSection("MainSection", MainSectionNewToolMenuDelegate);
}

UToolMenu* FXenosEditorMenuModule::GetMenu() const
{
	return CustomEditorMenu;
}

FName FXenosEditorMenuModule::GetStyleSetName()
{
	return "XenosEditorMenuStyle";
}

void FXenosEditorMenuModule::AddStyle(const FName& PropertyName, const FString& Path, const FString& LName, const float& Dimensions)
{
	const TSharedRef<FSlateStyleSet>& StyleSet = CustomEditorMenuStyle.ToSharedRef();
	StyleSet->SetContentRoot(Path);
	StyleSet->SetCoreContentRoot(Path);

	StyleSet->Set(PropertyName, new XENOS_IMAGE_BRUSH(TEXT(Name), FVector2D(Dimensions)));

	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet);
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);

	CustomEditorMenuStyle = StyleSet;
}

const FSlateBrush* FXenosEditorMenuModule::GetBrush(const FName& PropertyName) const
{
	return CustomEditorMenuStyle->GetBrush(PropertyName);
}

void FXenosEditorMenuModule::InitializeStyleSet()
{
	if (CustomEditorMenuStyle.IsValid())
	{
		return;
	}

	CustomEditorMenuStyle = MakeShareable(new FSlateStyleSet(GetStyleSetName()));
	FSlateStyleRegistry::RegisterSlateStyle(*CustomEditorMenuStyle);
}

void FXenosEditorMenuModule::ReleaseStyleSet()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*CustomEditorMenuStyle);

	ensure(CustomEditorMenuStyle.IsUnique());
	CustomEditorMenuStyle.Reset();
}

#undef LOCTEXT_NAMESPACE
