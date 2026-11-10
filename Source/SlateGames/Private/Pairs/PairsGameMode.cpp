// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#include "PairsGameMode.h"

#include "SlateGamesLib.h"
#include "SPairsBoard.h"
#include "Algo/RandomShuffle.h"

FPairsRules::FPairsRules(int32 InGridResolution)
{
	GridResolution = InGridResolution;
	CardsNum = GridResolution * GridResolution;
	PairsNum = CardsNum / 2;

	// Create cards data
	for (int32 i = 0; i < PairsNum; i++)
	{
		FCardParams Params;
		Params.Value = CalcValue();
		Params.Color = CalcColor(i);
		ExclusiveCards.Add(Params);
	}

	// Create cards collection
	for (auto& El : ExclusiveCards)
	{
		Cards.Emplace(El);
		Cards.Emplace(El);
	}

	// Shuffle cards
	for (int32 i = 0; i < CardsNum; i++)
	{
		Algo::RandomShuffle(Cards);
	}
}

int32 FPairsRules::CalcValue() const
{
	TSet<int32> UsedSet;
	for (auto& It : ExclusiveCards)
	{
		UsedSet.Emplace(It.Value);
	}

	TArray<int32> Available;
	for (int32 i = MinValue; i <= MaxValue; i++)
	{
		if (!UsedSet.Contains(i))
		{
			Available.Add(i);
		}
	}

	if (Available.IsEmpty())
	{
		return -1;
	}

	return Available[FMath::RandRange(0, Available.Num() - 1)];
}

FLinearColor FPairsRules::CalcColor(const int32& It) const
{
	FLinearColor HSV = FLinearColor(1,1,1,1);
	HSV.R = (360.0f / PairsNum) * It;   // Hue [0, 360)
	return HSV.HSVToLinearRGB();
}

#define LOCTEXT_NAMESPACE "PairsGameMode"

FPairsGameMode::FPairsGameMode(int32 DifficultyId)
{
	CurrentDifficulty = DifficultyId;
}

void FPairsGameMode::Restart()
{
	switch (CurrentDifficulty)
	{
	case 0:
		Rules = MakeUnique<FPairsEasyRules>();
		break;
	case 1:
		Rules = MakeUnique<FPairsNormalRules>();
		break;
	case 2:
	default:
		Rules = MakeUnique<FPairsHardRules>();
		break;
	}

	RevealedPairs = 0;
	FSlateGamesGameMode::Restart();
}

void FPairsGameMode::CheckFinishCondition()
{
	if (FPairsRules* PairsRules = Rules.Get())
	{
		if (RevealedPairs >= PairsRules->PairsNum)
		{
			SlateGamesLib::ShowResultScreen();
		}
	}
}

TSharedRef<SWidget> FPairsGameMode::CreateWidget()
{
	return SNew(SPairsBoard);
}

bool FPairsGameMode::OnCardClicked(const TSharedRef<SPairsCard>& Card)
{
	if (Card->bPairFound)
		return true;

	if (LastClickedCard.IsValid())
	{
		if (LastClickedCard.Pin() != Card) // If clicked another card
		{
			if (LastClickedCard.Pin()->IsPairFor(Card)) // Check pair
			{
				LastClickedCard.Pin()->bPairFound = true;
				Card->bPairFound = true;
				LastClickedCard.Reset();
				RevealedPairs++;
				CheckFinishCondition();
				return true;
			}

			// Pair check failed
			LastClickedCard.Pin()->SetOpenState(false);
			LastClickedCard = Card;
			return true;
		}

		// Same card
		LastClickedCard.Reset();
		return false;
	}
	LastClickedCard = Card;
	return true;
}

TArray<TSharedPtr<FGameModeDifficulty>> FPairsGameMode::GetDifficultyList()
{
	TArray<TSharedPtr<FGameModeDifficulty>> DifficultyList;
	DifficultyList.Add(MakeShared<FGameModeDifficulty>(LOCTEXT("DifficultyEasy", "Small"), 0));
	DifficultyList.Add(MakeShared<FGameModeDifficulty>(LOCTEXT("DifficultyNormal", "Medium"), 1));
	DifficultyList.Add(MakeShared<FGameModeDifficulty>(LOCTEXT("DifficultyHard", "Large"), 2));
	return DifficultyList;
}

#undef LOCTEXT_NAMESPACE