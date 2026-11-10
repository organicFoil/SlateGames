// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#include "SMainWindow.h"

#include "SlateGamesGameInstance.h"
#include "SlateGamesLib.h"
#include "SResultScreen.h"
#include "NumberFactory/NumberFactoryGameMode.h"
#include "Pairs/PairsGameMode.h"

#define LOCTEXT_NAMESPACE "SMainWindow"

void SMainWindow::Construct(const FArguments& InArgs)
{
	FGameInstanceDefaultArgs::GetGameModeNames(GameModeNames);
	FGameInstanceDefaultArgs::GetDifficultyList(DifficultyList);

	SDockTab::Construct(SDockTab::FArguments()
	.TabRole(InArgs._TabRole)
	[
		SNew(SVerticalBox)
		// Top menu
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(6.0f, 6.0f, 6.0f, 2.0f)
		[
			SNew(SBorder)
			.Padding(6.f, 12.f)
			.HAlign(HAlign_Center)
			[
				SNew(SHorizontalBox)

				// GameMode type title
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(6.0f, 4.0f)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("Mode", "Mode"))
				]

				// GameMode type ComboBox
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.MinWidth(150.f)
				.Padding(0.f, 0.f, 20.f, 0.f)
				[
					SAssignNew(GameModeComboBox, SComboBox<TSharedPtr<FGameModeTypeAndName>>)
					.OptionsSource(&GameModeNames)
					.OnGenerateWidget_Lambda([](TSharedPtr<FGameModeTypeAndName> InItem)
					{
						return SNew(STextBlock).Text(InItem->Name);
					})
					.InitiallySelectedItem(GameModeNames[FGameInstanceDefaultArgs::GameModeId])
					.OnSelectionChanged(this, &SMainWindow::OnGameModeTypeChanged)
					[
						SNew(STextBlock)
						.Text_Lambda([this]()
						{
							return GameModeComboBox->GetSelectedItem()->Name;
						})
					]
				]

				// GameMode difficulty title
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(6.0f, 4.0f)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("Difficulty", "Difficulty"))
				]

				// GameMode difficulty ComboBox
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.MinWidth(120.f)
				[
					SAssignNew(DifficultyComboBox, SComboBox<TSharedPtr<FGameModeDifficulty>>)
					.OptionsSource(&DifficultyList)
					.OnGenerateWidget_Lambda([](TSharedPtr<FGameModeDifficulty> InItem)
					{
						return SNew(STextBlock).Text(InItem->Name);
					})
					.InitiallySelectedItem(DifficultyList[FGameInstanceDefaultArgs::DifficultyId])
					.OnSelectionChanged(this, &SMainWindow::OnGameModeDifficultyChanged)
					[
						SNew(STextBlock)
						.Text_Lambda([this]()
						{
							return DifficultyComboBox->GetSelectedItem()->Name;
						})
					]
				]

				// Start button
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.MinWidth(80.f)
				.Padding(20.f, 0.f, 0.f, 0.f)
				[
					SNew(SButton)
					.OnClicked(FOnClicked::CreateSP(this, &SMainWindow::OnStartGameClicked))
					[
						SAssignNew(StartTextBlock, STextBlock)
						.Justification(ETextJustify::Center)
					]
				]
			]
		]

		// Game container
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.Padding(6.0f, 2.0f, 6.0f, 6.0f)
		[
			SAssignNew(ViewportOverlay, SOverlay)
			+ SOverlay::Slot(0)
			[
				SAssignNew(GameContainer, SBox)
				[
					SlateGamesLib::GetGameMode()->CreateWidget()
				]
			]
		]
	]);
}

void SMainWindow::ResetWidgets()
{
	// Remove ResultScreen
	if (ViewportOverlay->HasSlotWithZOrder(1))
	{
		ViewportOverlay->RemoveSlot(1);
	}

	// Create widget of new game
	GameContainer->SetContent(SlateGamesLib::GetGameMode()->CreateWidget());

	// Update UI sub-elements for new game
	PostLoadGame();
}

void SMainWindow::PostLoadGame()
{
	// Update StartButton
	StartTextBlock->SetText(ConstructTextForStartButton(true));

	// Update DifficultyComboBox
	if (FSlateGamesGameMode* GameMode = SlateGamesLib::GetGameMode())
	{
		SetDifficultyList(GameMode->GetType(), GameMode->CurrentDifficulty);
	}
}

void SMainWindow::ShowResultScreen(bool IsWin)
{
	ViewportOverlay->AddSlot(1)
	[
		SNew(SResultScreen)
		.Type(IsWin ? EResultScreenType::Win : EResultScreenType::Defeat)
	];
}

FReply SMainWindow::OnStartGameClicked()
{
	SlateGames::EGameMode SelectedGameMode = GameModeComboBox->GetSelectedItem().Get()->Type;
	int32 Difficulty = DifficultyComboBox->GetSelectedItem().Get()->Id;
	SlateGamesLib::GetGameInstance()->SetGameMode(SelectedGameMode, Difficulty);
	SlateGamesLib::Restart();
	return FReply::Unhandled();
}

FText SMainWindow::ConstructTextForStartButton(bool bRestart) const
{
	if (bRestart)
	{
		return FText(LOCTEXT("RestartButton", "Restart"));
	}
	return FText(LOCTEXT("StartButton", "Start"));
}

void SMainWindow::OnGameModeTypeChanged(TSharedPtr<FGameModeTypeAndName> Item, ESelectInfo::Type SelectType)
{
	FSlateGamesGameMode* GameMode = SlateGamesLib::GetGameMode();
	if (!GameMode)
		return;

	if (Item->Type == GameMode->GetType()) 
	{
		StartTextBlock->SetText(ConstructTextForStartButton(true));
		SetDifficultyList(Item->Type, GameMode->CurrentDifficulty);
	}
	else
	{
		StartTextBlock->SetText(ConstructTextForStartButton(false));
		SetDifficultyList(Item->Type, FGameInstanceDefaultArgs::DifficultyId);
	}
}

void SMainWindow::OnGameModeDifficultyChanged(TSharedPtr<FGameModeDifficulty> Item, ESelectInfo::Type SelectType)
{
	FSlateGamesGameMode* GameMode = SlateGamesLib::GetGameMode();
	if (!GameMode)
		return;

	SlateGames::EGameMode SelectedGameMode = GameModeComboBox->GetSelectedItem().Get()->Type;
	int32 Difficulty = DifficultyComboBox->GetSelectedItem().Get()->Id;

	if (SelectedGameMode == GameMode->GetType() && Difficulty == GameMode->CurrentDifficulty)
	{
		StartTextBlock->SetText(ConstructTextForStartButton(true));
	}
	else
	{
		StartTextBlock->SetText(ConstructTextForStartButton(false));
	}
}

void SMainWindow::SetDifficultyList(SlateGames::EGameMode GameMode, int32 DifficultyId)
{
	switch (GameMode) {
	case SlateGames::EGameMode::Pairs:
		DifficultyList = FPairsGameMode::GetDifficultyList();
		break;
	case SlateGames::EGameMode::NumberFactory:
		DifficultyList = FNumberFactoryGameMode::GetDifficultyList();
		break;
	}

	DifficultyComboBox->SetItemsSource(&DifficultyList);
	if (DifficultyList.IsValidIndex(DifficultyId))
	{
		DifficultyComboBox->SetSelectedItem(DifficultyList[DifficultyId]);
	}
	DifficultyComboBox->RefreshOptions();
}

#undef LOCTEXT_NAMESPACE
