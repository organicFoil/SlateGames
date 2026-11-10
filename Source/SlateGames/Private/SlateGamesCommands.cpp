// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#include "SlateGamesCommands.h"

#define LOCTEXT_NAMESPACE "FSlateGamesModule"

void FSlateGamesCommands::RegisterCommands()
{
	const TSharedRef<FBindingContext> SharedThis = AsShared();

	FUICommandInfo::MakeCommandInfo(
		SharedThis
		, OpenPluginWindow
		, TEXT("SlateGamesButton")
		, LOCTEXT("SlateGamesButton_Label", "Slate Games")
		, LOCTEXT("SlateGamesButton_ToolTip", "Run SlateGames")
		, FSlateIcon(FAppStyle::Get().GetStyleSetName(), "PlayWorld.RepeatLastLaunch")
		, EUserInterfaceActionType::Button
		, FInputChord());
}

#undef LOCTEXT_NAMESPACE
