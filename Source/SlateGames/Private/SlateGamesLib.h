// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#pragma once

class FSlateGamesGameMode;
class FSlateGamesGameInstance;

namespace SlateGamesLib
{
	FSlateGamesGameInstance* GetGameInstance();
	FSlateGamesGameMode* GetGameMode();
	void Restart();
	void ShowResultScreen();
};
