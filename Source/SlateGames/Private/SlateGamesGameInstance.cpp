// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#include "SlateGamesGameInstance.h"

#include "NumberFactory/NumberFactoryGameMode.h"
#include "Pairs/PairsGameMode.h"

using SlateGames::EGameMode;

#define LOCTEXT_NAMESPACE "FGameInstanceDefaultArgs"

int32 FGameInstanceDefaultArgs::GameModeId = 0;		// Pairs
int32 FGameInstanceDefaultArgs::DifficultyId= 1;	// Normal

void FGameInstanceDefaultArgs::GetGameModeNames(TArray<TSharedPtr<FGameModeTypeAndName>>& GameModes)
{
	GameModes.Empty();
	GameModes.Add(MakeShared<FGameModeTypeAndName>(LOCTEXT("PairsMode", "Pairs"), SlateGames::EGameMode::Pairs));
	GameModes.Add(MakeShared<FGameModeTypeAndName>(LOCTEXT("NumberFactoryMode", "Number Factory"), SlateGames::EGameMode::NumberFactory));
}

void FGameInstanceDefaultArgs::GetDifficultyList(TArray<TSharedPtr<FGameModeDifficulty>>& DiffuciltyList)
{
	DiffuciltyList = FPairsGameMode::GetDifficultyList();
}

TUniquePtr<FSlateGamesGameMode> FGameInstanceDefaultArgs::CreateGameMode()
{
	return MakeUnique<FPairsGameMode>(DifficultyId);
}

#undef LOCTEXT_NAMESPACE

FSlateGamesGameInstance::FSlateGamesGameInstance()
{
	GameMode = FGameInstanceDefaultArgs::CreateGameMode();
}

void FSlateGamesGameInstance::SetGameMode(EGameMode InGameMode, int32 InDifficultyId)
{
	switch (InGameMode)
	{
	case EGameMode::Pairs:
		GameMode = MakeUnique<FPairsGameMode>(InDifficultyId);
		break;
	case EGameMode::NumberFactory:
		GameMode = MakeUnique<FNumberFactoryGameMode>(InDifficultyId);
		break;
	default: break;
	}
}

FSlateGamesGameMode* FSlateGamesGameInstance::GetGameMode()
{
	return GameMode.Get();
}
