// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#include "SlateGamesLib.h"

#include "SlateGamesGameInstance.h"
#include "SlateGamesModule.h"
#include "SMainWindow.h"

FSlateGamesGameInstance* SlateGamesLib::GetGameInstance()
{
	return FSlateGamesModule::Get()->GetGameInstance();
}

FSlateGamesGameMode* SlateGamesLib::GetGameMode()
{
	return FSlateGamesModule::Get()->GetGameInstance()->GetGameMode();
}

void SlateGamesLib::Restart()
{
	auto GameMode = SlateGamesLib::GetGameMode();
	auto MainWindow = SlateGamesLib::GetGameInstance()->MainWindow;

	check(MainWindow.IsValid() && GameMode)
	GameMode->Restart();
	MainWindow.Pin()->ResetWidgets();
}

void SlateGamesLib::ShowResultScreen()
{
	auto MainWindow = SlateGamesLib::GetGameInstance()->MainWindow;

	check(MainWindow.IsValid())
	MainWindow.Pin()->ShowResultScreen(true);
}
