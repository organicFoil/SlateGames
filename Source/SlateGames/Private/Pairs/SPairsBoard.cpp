// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#include "SPairsBoard.h"

#include "PairsGameMode.h"
#include "SlateGamesLib.h"
#include "Widgets/Layout/SUniformGridPanel.h"

void SPairsBoard::Construct(const FArguments& InArgs)
{
	TSharedPtr<SUniformGridPanel> CardsContainer;

	SBorder::Construct( SBorder::FArguments()
	.BorderImage(FAppStyle::GetBrush("WhiteBrush"))
	.BorderBackgroundColor(FLinearColor(FColor(33,33,33)))
	.Padding(150)
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	[
		SAssignNew(CardsContainer, SUniformGridPanel)
		.SlotPadding(5)
	]);

	PopulateContainer(CardsContainer);
}

void SPairsBoard::PopulateContainer(const TSharedPtr<SUniformGridPanel>& CardsContainer)
{
	FPairsRules* Rules = SlateGamesLib::GetGameMode()->As<FPairsGameMode>()->Rules.Get();
	if (Rules == nullptr)
		return;

	if (Rules->GridResolution % 2 == 1)
		return;

	for (int32 i = 0; i < Rules->CardsNum; i++)
	{
		FSlotOptions Options;
		Options.Column = i % Rules->GridResolution;
		Options.Row = i / Rules->GridResolution;
		Options.CardParams = Rules->GetCard(i);
		AddSlot(CardsContainer, Options);
	}
}

void SPairsBoard::AddSlot(const TSharedPtr<SUniformGridPanel>& CardsContainer, const FSlotOptions& Options)
{
	CardsContainer->AddSlot(Options.Column, Options.Row)
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	[
		SNew(SPairsCard)
		.Parameters(Options.CardParams)
	];
}