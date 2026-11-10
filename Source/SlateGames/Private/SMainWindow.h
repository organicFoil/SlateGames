// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#pragma once

#include "Widgets/Docking/SDockTab.h"

struct FGameModeDifficulty;
struct FGameModeTypeAndName;

namespace SlateGames
{
	enum class EGameMode : uint8;
}

class SMainWindow : public SDockTab
{
public:
	SLATE_BEGIN_ARGS(SMainWindow) {}
		SLATE_ARGUMENT(ETabRole, TabRole)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	void ResetWidgets();
	void PostLoadGame();
	void ShowResultScreen(bool IsWin);

private:
	FReply OnStartGameClicked();
	FText ConstructTextForStartButton(bool bRestart) const;
	void OnGameModeTypeChanged(TSharedPtr<FGameModeTypeAndName> Item, ESelectInfo::Type SelectType);
	void OnGameModeDifficultyChanged(TSharedPtr<FGameModeDifficulty> Item, ESelectInfo::Type SelectType);
public:
	void SetDifficultyList(SlateGames::EGameMode GameMode, int32 DifficultyId);

	TSharedPtr<SOverlay> ViewportOverlay;
	TSharedPtr<SBox> GameContainer;
	TSharedPtr<STextBlock> StartTextBlock;
	TSharedPtr<SComboBox<TSharedPtr<FGameModeTypeAndName>>> GameModeComboBox;
	TArray<TSharedPtr<FGameModeTypeAndName>> GameModeNames;
	TSharedPtr<SComboBox<TSharedPtr<FGameModeDifficulty>>> DifficultyComboBox;
	TArray<TSharedPtr<FGameModeDifficulty>> DifficultyList;
};
