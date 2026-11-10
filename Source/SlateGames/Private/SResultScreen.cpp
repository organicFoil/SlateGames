// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#include "SResultScreen.h"

#include "SlateGamesLib.h"
#include "Styling/SlateStyleMacros.h"

#define LOCTEXT_NAMESPACE "SResultScreen"

void SResultScreen::Construct(const FArguments& InArgs)
{
	SBorder::Construct(SBorder::FArguments()
	.BorderImage(FAppStyle::GetBrush("WhiteBrush"))
	.BorderBackgroundColor(FLinearColor(FColor(134,134,134, 250)))
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.Padding(0.f,0.f, 0.f, 40.f)
		[
			SNew(STextBlock)
			.Text(InArgs._Type == EResultScreenType::Win ? LOCTEXT("WinTitle", "Win") : LOCTEXT("DefeatTitle", "Defeat"))
			.ColorAndOpacity(FSlateColor(FLinearColor::White))
			.Justification(ETextJustify::Center)
			.Font(DEFAULT_FONT("Bold", 42))
		]
		+ SVerticalBox::Slot()
		[
			SNew(SButton)
			.ContentPadding(FMargin(30.f, 15.f))
			.OnClicked(FOnClicked::CreateSP(this, &SResultScreen::OnRestartClicked))
			[
				SNew(STextBlock)
				.Text(LOCTEXT("RestartButton", "Restart"))
				.Font(DEFAULT_FONT("Regular", 16))
			]
		]
	]);
}

FReply SResultScreen::OnRestartClicked()
{
	SlateGamesLib::Restart();
	return FReply::Unhandled();
}

#undef LOCTEXT_NAMESPACE