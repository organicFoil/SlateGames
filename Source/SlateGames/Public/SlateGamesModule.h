// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FSlateGamesGameInstance;
struct FGraphPanelNodeFactory;
struct FGraphPanelPinFactory;

class FSlateGamesModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static FSlateGamesModule* Get()
	{
		return &FModuleManager::LoadModuleChecked<FSlateGamesModule>("SlateGames");
	}

	FSlateGamesGameInstance* GetGameInstance() { return GameInstance.Get(); }
	void PluginButtonClicked();

private:
	bool CanSpawnTab(const FSpawnTabArgs& Args);
	bool IsTabAllowedInCurrentEditorState();
	void RegisterMenus();
	TSharedRef<SDockTab> OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs);

	TSharedPtr<FUICommandList> PluginCommands;
	TSharedPtr<FGraphPanelNodeFactory> NodeFactory;
	TSharedPtr<FGraphPanelPinFactory> PinFactory;
	TUniquePtr<FSlateGamesGameInstance> GameInstance;
};
