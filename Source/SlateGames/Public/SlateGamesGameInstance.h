// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#pragma once

#include "SlateGamesGameMode.h"

struct FGameInstanceDefaultArgs
{
	static void GetGameModeNames(TArray<TSharedPtr<FGameModeTypeAndName>>& GameModes);
	static void GetDifficultyList(TArray<TSharedPtr<FGameModeDifficulty>>& DiffuciltyList);
	static TUniquePtr<FSlateGamesGameMode> CreateGameMode();

	static int32 GameModeId;
	static int32 DifficultyId;
};

class FSlateGamesGameInstance
{
public:
	FSlateGamesGameInstance();

	void SetGameMode(SlateGames::EGameMode InGameMode, int32 InDifficultyId);
	FSlateGamesGameMode* GetGameMode();

	TWeakPtr<SMainWindow> MainWindow;

private:
	TUniquePtr<FSlateGamesGameMode> GameMode;
};
