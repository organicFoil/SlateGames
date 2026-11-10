// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"
#include "SlateGamesStyle.h"

class FSlateGamesCommands : public TCommands<FSlateGamesCommands>
{
public:
	FSlateGamesCommands()
		: TCommands<FSlateGamesCommands>(TEXT("SlateGames"),
			NSLOCTEXT("Contexts", "SlateGames", "SlateGames Plugin"),
			NAME_None,
			FSlateGamesStyle::GetStyleSetName()) {}

	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};