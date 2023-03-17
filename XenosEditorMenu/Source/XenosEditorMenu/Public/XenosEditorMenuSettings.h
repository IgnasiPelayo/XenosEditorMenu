// Copyright (c) Ignasi Pelayo, 2023. All Rights Reserved.

#pragma once

#include "XenosEditorMenuSettings.generated.h"

USTRUCT()
struct FXenosEditorMenuSubcategory
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Config)
	FName SubcategoryName;
	
	UPROPERTY(EditAnywhere, Config)
	bool bHasIcon;

	UPROPERTY(EditAnywhere, Config, meta = (EditCondition = "bHasIcon==true", EditConditionHides))
	bool bHasCustomIcon;
	
	UPROPERTY(EditAnywhere, Config, meta = (EditCondition = "bHasIcon==true", EditConditionHides))
	FName IconName;

	UPROPERTY(EditAnywhere, Config, meta = (EditCondition = "bHasIcon==true&&bHasCustomIcon==true", EditConditionHides,
		ToolTip = "Relative path from the project directory"))
	FString CustomIconPath;

	UPROPERTY(EditAnywhere, Config, meta = (EditCondition = "bHasIcon==true&&bHasCustomIcon==true", EditConditionHides))
	int32 CustomIconDimensions = 30;	
};

USTRUCT(BlueprintType)
struct FXenosEditorMenuCategory
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Config)
	FName CategoryName;

	UPROPERTY(EditAnywhere, Config, meta = (TitleProperty = "{SubcategoryName}"))
	TArray<FXenosEditorMenuSubcategory> Subcategories;
};

USTRUCT(BlueprintType)
struct FXenosEditorMenuConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Config)
	FName MenuName = "Xenos";

	UPROPERTY(EditAnywhere, Config)
	FName MenuTooltipName = "Xenos";

	UPROPERTY(EditAnywhere, Config, meta = (TitleProperty = "{CategoryName}"))
	TArray<FXenosEditorMenuCategory> Categories;
};

UCLASS(Config = Editor, DefaultConfig)
class UXenosEditorMenuSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Config)
	FXenosEditorMenuConfig XenosEditorMenu;

	UPROPERTY(EditAnywhere, Config)
	TArray<FXenosEditorMenuConfig> AdditionalXenosEditorMenus;
};