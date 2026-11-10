// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#include "SlateGamesModule.h"

#include "SlateGamesStyle.h"
#include "SlateGamesCommands.h"
#include "SlateGamesNodeFactory.h"
#include "SMainWindow.h"
#include "EdGraphUtilities.h"
#include "SlateGamesGameInstance.h"
#include "SlateGamesLib.h"
#include "Math/UnrealMathUtility.h"
#include "ToolMenus.h"
#include "NumberFactory/NumberFactoryPinFactory.h"

static const FName SlateGamesTabName("SlateGames");

#define LOCTEXT_NAMESPACE "FSlateGamesModule"

void FSlateGamesModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FSlateGamesStyle::Initialize();
	FSlateGamesStyle::ReloadTextures();
	FSlateGamesCommands::Register();

	NodeFactory = MakeShared<FSlateGamesNodeFactory>();
	FEdGraphUtilities::RegisterVisualNodeFactory(NodeFactory);

	PinFactory = MakeShared<FNumberFactoryPinFactory>();
	FEdGraphUtilities::RegisterVisualPinFactory(PinFactory);

	PluginCommands = MakeShareable(new FUICommandList);
	PluginCommands->MapAction(
		FSlateGamesCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FSlateGamesModule::PluginButtonClicked),
		FCanExecuteAction::CreateRaw(this, &FSlateGamesModule::IsTabAllowedInCurrentEditorState),
		FIsActionChecked(),
		FIsActionButtonVisible::CreateRaw(this, &FSlateGamesModule::IsTabAllowedInCurrentEditorState));

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FSlateGamesModule::RegisterMenus));

	FTabSpawnerEntry& TabSpawnerEntry = FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		SlateGamesTabName,
		FOnSpawnTab::CreateRaw(this, &FSlateGamesModule::OnSpawnPluginTab),
		FCanSpawnTab::CreateRaw(this, &FSlateGamesModule::CanSpawnTab));

	TabSpawnerEntry.SetDisplayName(LOCTEXT("SlateGamesTabTitle", "Slate Games"));
	TabSpawnerEntry.SetMenuType(ETabSpawnerMenuType::Hidden);
	TabSpawnerEntry.SetIcon(FSlateIcon(FAppStyle::Get().GetStyleSetName(), "PlayWorld.RepeatLastLaunch"));
}

void FSlateGamesModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FSlateGamesStyle::Shutdown();

	FSlateGamesCommands::Unregister();

	if (NodeFactory.IsValid())
	{
		FEdGraphUtilities::UnregisterVisualNodeFactory(NodeFactory);
		NodeFactory.Reset();
	}

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(SlateGamesTabName);
}

TSharedRef<SDockTab> FSlateGamesModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	// Init GameInstance
	GameInstance = MakeUnique<FSlateGamesGameInstance>();

	// Create main widget
	TSharedRef<SMainWindow> MainWindow = SNew(SMainWindow)
		.TabRole(ETabRole::NomadTab);

	GameInstance->MainWindow = MainWindow;

	SlateGamesLib::Restart();

	return MainWindow;
}

void FSlateGamesModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(SlateGamesTabName);
}

bool FSlateGamesModule::CanSpawnTab(const FSpawnTabArgs& Args)
{
	return IsTabAllowedInCurrentEditorState();
}

bool FSlateGamesModule::IsTabAllowedInCurrentEditorState()
{
	return GEditor->PlayWorld == nullptr;
}

void FSlateGamesModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	// Menu button
	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Tools");
		FToolMenuSection& Section = Menu->FindOrAddSection("Tools");
		Section.AddMenuEntryWithCommandList(FSlateGamesCommands::Get().OpenPluginWindow, PluginCommands);
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSlateGamesModule, SlateGames)
