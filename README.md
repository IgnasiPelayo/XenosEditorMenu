# Xenos Editor Menu for UE5

Creating an editor menu is pretty simple with this plugin. It also allows adding tabs dynamically to specified categories and custom Slate styles, such as custom icons.

## Installation Guide

- Since this is an independent plugin, add the downloaded files to the "Plugins" folder inside your UE5 project.

> **NOTE:** The plugin can be placed in subdirectories inside the "Plugins" directory for better organization.
> For example: ../YourProjectName/Plugins/Editor/

- Right-click on the .uproject file and click on "Generate Visual Project Files."
- Compile the project.

## First steps

- Once the Unreal Engine 5 editor is open, you can see the "Xenos" menu in the top area, next to the "Help" menu.
- To configure the menu according to the needs of your project, go to Edit > Project Settings > Editor > Xenos Editor Menu.
- In this section, you can change the menu name and the tooltip menu name. You can also add categories, although we will explain this topic a little later.
- You can also add additional menus and rename them as you wish by adding a field in "Additional Xenos Editor Menus".
- Once you have made the changes, you will need to restart the engine for the changes to take effect.

## Adding custom tabs

Let's add our first tab to one of our menus. 

- To get started, we need to add a dependency to the XenosEditorMenu in any editor module in our project.
- Then, in the StartupModule of that module, we have to register the nomad tab spawner. Here's one way to do it:

```c++
FXenosEditorMenuModule XenosEditorMenuModule = FXenosEditorMenuModule::GetModule();

const FName XenosEditorMenuName = NAME_None;
const FString CategoryName = "Main";
const TSharedPtr<FWorkspaceItem> MainSection = XenosEditorMenuModule.GetXenosEditorMenu(XenosEditorMenuName).GetCategoryByDisplayName(CategoryName);
if (MainSection.IsValid())
{
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner("ExampleTab", FOnSpawnTab::CreateLambda([](const FSpawnTabArgs& Args) -> TSharedRef<SDockTab>
	{
	  return SNew(SDockTab)
	  .TabRole(ETabRole::NomadTab)
	  [
		  SNew(STextBlock)
		  .Text(FText::FromString("This is a tab example"))
	  ];
	}))
	.SetDisplayName(FText::FromString("ExampleTab"))
	.SetGroup(MainSection.ToSharedRef());
}
```
- Additionally, don't forget to unregister the nomad tab spawner in the ShutdownModule function:
```c++
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner("ExampleTab");
```

## Creating Custom Groups and Sections

Let's take the default "Tools" menu as an example to explain this topic. The "Tools" structure has a main category (just like every Xenos editor menu), but it is divided into eight groups: PROGRAMMING, TOOLS, INSTRUMENTATION, SOURCE CONTROL, UNREAL INSIGHTS, DATA VALIDATION, WORLD PARTITION, and PYTHON. Additionally, some items have additional subcategories. Tools > INSTRUMENTATION > Debug is an example. Now let's create groups in our Xenos menu.

- Go to Edit > Project Settings > Editor > Xenos Editor Menu and add a new category to your menu. Change the "Category Name" variable to something descriptive. For this tutorial, we will use "Tools" as the name of the category.

> **NOTE:** Even if you restart the Unreal Engine, this category or group won't be visible as it doesn't hold any tab..

- Change the "CategoryName" variable of the code snippet above to your new category's "Category Name," and compile again.

```c++
const FString CategoryName = "Tools";
```

We can also create subcategories and add icons.

- To create a subcategory, go to Edit > Project Settings > Editor > Xenos Editor Menu and add a subcategory to the "Tools" category. Change the name of the newly created subcategory. For this tutorial, we will use "Debug" as the name of the subcategory.

- Now, let's enable the "Has Icon" option. We will add a Unreal Engine 5 icon, for example, we will use the same icon as the Tools > INSTRUMENTATION > Debug subcategory. Update the "Icon Name" field of the subcategory with "Debug".

> Remember, we have to update the "CategoryName" value in our .cpp file to the current subcategory name to spawn the tab in that subcategory.

Now, let's add a custom icon. For this example, we will use the "Icon_Xenos_40x.png" file located at "XenosEditorMenu/Resources".

- First, enable the "Has Custom Icon" option. Some additional options will appear.
- In the "Icon Name" field, write the name of the file. In this case, "Icon_Xenos_40x".
- In the "Custom Icon Path" field, write the relative path of the directory that contains the icon. In my case, "Plugins/XenosEditorMenu/Resources".
- Finally, in the "Custom Icon Dimensions" field, determine the dimensions in pixels of that icon. In this case, 40.

> Recompile the project to see if the icon was successfully loaded. If a white box appears, something went wrong.

## Adding custom icons to tab spawners via C++

In this part of the tutorial, we will add the "Icon_Xenos_40x" icon to the "ExampleTab" tab via C++.
To do this, we will modify the code snippet above as follows:

```c++
FXenosEditorMenuModule XenosEditorMenuModule = FXenosEditorMenuModule::GetModule();

XenosEditorMenuModule.AddStyle("XenosIcon", FPaths::ProjectDir() / "Plugins/XenosEditorMenu/Resources",
	"Icon_Xenos_40x", 40);

[...]
    .SetGroup(MainSection.ToSharedRef())
	.SetIcon(FSlateIcon(XenosEditorMenuModule.GetStyleSetName(), "XenosIcon"));
[...]
```
