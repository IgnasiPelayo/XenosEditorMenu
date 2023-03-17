// Copyright (c) Ignasi Pelayo, 2023. All Rights Reserved.

#include "XenosEditorMenuModule.h"

#include "ToolMenus.h"
#include "ISettingsModule.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

#include "XenosEditorMenuSettings.h"

IMPLEMENT_MODULE(FXenosEditorMenuModule, XenosEditorMenu)

#define LOCTEXT_NAMESPACE "FXenosEditorMenuModule"

#define XENOS_IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)

TSharedPtr<FWorkspaceItem> FXenosEditorMenu::GetCategoryByDisplayName(const FString& CategoryName)
{
	for (int i = 0; i < XenosEditorMenuCategories.Num(); ++i)
	{
		if (XenosEditorMenuCategories[i]->GetDisplayName().ToString() == CategoryName)
		{
			return XenosEditorMenuCategories[i];
		}
	}

	return nullptr;
}

void FXenosEditorMenuModule::StartupModule()
{
	InitializeStyleSet();

	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule != nullptr)
	{
		SettingsModule->RegisterSettings("Project", "Editor", NAME_None,
			FText::FromString("Xenos Editor Menu"),FText::FromString("Configure Xenos Editor Menus"),
			GetMutableDefault<UXenosEditorMenuSettings>());
	}

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this,
		&FXenosEditorMenuModule::RegisterXenosEditorMenu));
}

void FXenosEditorMenuModule::ShutdownModule()
{
	ReleaseStyleSet();
}

FXenosEditorMenuModule& FXenosEditorMenuModule::GetModule()
{
	return FModuleManager::LoadModuleChecked<FXenosEditorMenuModule>("XenosEditorMenu");
}

void FXenosEditorMenuModule::RegisterXenosEditorMenu()
{
	const UXenosEditorMenuSettings* XenosEditorMenuSettings = GetDefault<UXenosEditorMenuSettings>();
	if (XenosEditorMenuSettings != nullptr)
	{
		XenosEditorMenu = RegisterXenosEditorMenu(XenosEditorMenuSettings->XenosEditorMenu);

		AdditionalXenosEditorMenus.Empty();
		for (int i = 0; i < XenosEditorMenuSettings->AdditionalXenosEditorMenus.Num(); ++i)
		{
			AdditionalXenosEditorMenus.Push(RegisterXenosEditorMenu(XenosEditorMenuSettings->AdditionalXenosEditorMenus[i]));
		}
	}
}

FXenosEditorMenu FXenosEditorMenuModule::RegisterXenosEditorMenu(const FXenosEditorMenuConfig& XenosEditorMenuData)
{
	UToolMenu* MainMenu = UToolMenus::Get()->ExtendMenu("MainFrame.MainMenu");

	FXenosEditorMenu XenosMenu;
	XenosMenu.MenuName = XenosEditorMenuData.MenuName;
	
	XenosMenu.Menu = MainMenu->AddSubMenu("MainMenu", NAME_None, XenosEditorMenuData.MenuName,
		FText::FromName(XenosEditorMenuData.MenuName),
		FText::FromString(FString::Printf(TEXT("Open the %s menu"), *XenosEditorMenuData.MenuTooltipName.ToString())));

	XenosMenu.XenosEditorMenuCategories = GetXenosEditorMenuTabStructure(XenosEditorMenuData);
	
	const FNewToolMenuDelegateLegacy NewToolMenuDelegate = FNewToolMenuDelegateLegacy::CreateLambda(
		[=](FMenuBuilder& InBuilder, UToolMenu* InData)
	{
		const USlateTabManagerContext* TabManagerContext = InData->FindContext<USlateTabManagerContext>();
		if (TabManagerContext != nullptr)
		{
			const TSharedPtr<FTabManager> TabManager = TabManagerContext->TabManager.Pin();
			if (TabManager.IsValid())
			{
				TabManager->PopulateTabSpawnerMenu(InBuilder, XenosMenu.XenosEditorMenuCategories[0].ToSharedRef());
			}
		}
	});
		
	XenosMenu.Menu->AddDynamicSection("MainSection", NewToolMenuDelegate);
	
	return XenosMenu;
}

TArray<TSharedPtr<FWorkspaceItem>> FXenosEditorMenuModule::GetXenosEditorMenuTabStructure(const FXenosEditorMenuConfig& XenosEditorMenuConfig)
{
	TArray<TSharedPtr<FWorkspaceItem>> MenuStructure;
	MenuStructure.Add(FWorkspaceItem::NewGroup(FText::FromString("Main")));
	
	for (int i = 0; i < XenosEditorMenuConfig.Categories.Num(); ++i)
	{
		TSharedPtr<FWorkspaceItem> Category = MenuStructure[0]->AddGroup(FText::FromName(XenosEditorMenuConfig.Categories[i].CategoryName));
		MenuStructure.Add(Category);
		for (int j = 0; j < XenosEditorMenuConfig.Categories[i].Subcategories.Num(); ++j)
		{
			FSlateIcon SubcategoryIcon = FSlateIcon();

			if (XenosEditorMenuConfig.Categories[i].Subcategories[j].bHasIcon)
			{
				const FName IconName = XenosEditorMenuConfig.Categories[i].Subcategories[j].IconName;

				if (XenosEditorMenuConfig.Categories[i].Subcategories[j].bHasCustomIcon)
				{
					const FString IconFullPath = FPaths::ProjectDir() / XenosEditorMenuConfig.Categories[i].Subcategories[j].CustomIconPath;
					const FString IconPath = FPaths::GetPath(IconFullPath);
					const FString IconFilename = FPaths::GetBaseFilename(IconFullPath);

					AddStyle(IconName, IconPath, IconFilename, XenosEditorMenuConfig.Categories[i].Subcategories[j].CustomIconDimensions);

					SubcategoryIcon = FSlateIcon(GetStyleSetName(), IconName);
				}
				else
				{
					SubcategoryIcon = FSlateIcon(FAppStyle::GetAppStyleSetName(), IconName);
				}
			}
			
			MenuStructure.Add(Category->AddGroup(FText::FromName(XenosEditorMenuConfig.Categories[i].Subcategories[j].SubcategoryName),
				SubcategoryIcon));
		}
	}

	return MenuStructure;
}

FXenosEditorMenu FXenosEditorMenuModule::GetXenosEditorMenu(const FName& XenosEditorMenuName) const
{
	if (XenosEditorMenuName == NAME_None || XenosEditorMenu.MenuName == XenosEditorMenuName)
	{
		return XenosEditorMenu;
	}

	for (int i = 0; i < AdditionalXenosEditorMenus.Num(); ++i)
	{
		if (AdditionalXenosEditorMenus[i].MenuName == XenosEditorMenuName)
		{
			return AdditionalXenosEditorMenus[i];
		}
	}

	return FXenosEditorMenu();
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
