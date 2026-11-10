// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#include "SPairsCard.h"

#include "PairsGameMode.h"
#include "SlateGamesLib.h"
#include "Styling/SlateStyleMacros.h"

#define DEFAULT_COLOR FLinearColor(FColor(67, 164, 101))

void SPairsCard::Construct(const FArguments& InArgs)
{
	Parameters = InArgs._Parameters;

	SButton::Construct(SButton::FArguments()
		.ButtonColorAndOpacity(FSlateColor(DEFAULT_COLOR))
		.OnClicked(FOnClicked::CreateSP(this, &SPairsCard::OnClicked)));

	ChildSlot
	[
		SNew(SBox)
		.HeightOverride(80.f)
		.WidthOverride(80.f)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SAssignNew(TextBlock, STextBlock)
			.Text(FText::AsNumber(InArgs._Parameters.Value))
			.Font(DEFAULT_FONT("Regular", 30))
			.Visibility(EVisibility::Hidden)
		]
	];
}

FReply SPairsCard::OnClicked()
{
	if (FPairsGameMode* GameMode = SlateGamesLib::GetGameMode()->As<FPairsGameMode>())
	{
		SetOpenState(GameMode->OnCardClicked(SharedThis(this)));
	}
	return FReply::Unhandled();
}

void SPairsCard::SetOpenState(bool bOpen)
{
	if (bOpen)
	{
		SetBorderBackgroundColor(Parameters.Color);
		TextBlock->SetVisibility(EVisibility::SelfHitTestInvisible);
	}
	else
	{
		if (bPairFound)	return;
		SetBorderBackgroundColor(DEFAULT_COLOR);
		TextBlock->SetVisibility(EVisibility::Hidden);
	}
}

bool SPairsCard::IsOpen() const
{
	return TextBlock->GetVisibility() == EVisibility::SelfHitTestInvisible;
}

bool SPairsCard::IsPairFor(const TSharedRef<SPairsCard>& TestCard) const
{
	return (this != &TestCard.Get()) && (Parameters.Value == TestCard->Parameters.Value);
}
