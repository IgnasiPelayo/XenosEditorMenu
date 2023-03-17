// Copyright (c) Ignasi Pelayo, 2023. All Rights Reserved.

#pragma once

#include "XenosEditorMenuSettings.generated.h"

USTRUCT(BlueprintType)
struct FXenosEditorMenuCategory
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Config)
	FName CategoryName;

	UPROPERTY(EditAnywhere, Config)
	TArray<FName> SubcategoriesNames;
};

USTRUCT(BlueprintType)
struct FXenosEditorMenuConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Config)
	FName MenuName = "Xenos";

	UPROPERTY(EditAnywhere, Config)
	FName MenuTooltipName = "Xenos";

	UPROPERTY(EditAnywhere, Config)
	TArray<FXenosEditorMenuCategory> Categories;
};

UCLASS(Config = Engine, DefaultConfig)
class UXenosEditorMenuSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Config)
	FXenosEditorMenuConfig XenosEditorMenu;

	UPROPERTY(EditAnywhere, Config, AdvancedDisplay)
	TArray<FXenosEditorMenuConfig> AdditionalXenosEditorMenus;
};