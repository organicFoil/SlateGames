// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#pragma once

#include "SlateGamesGameMode.h"
#include "SPairsCard.h"

// Slot on board
struct FSlotOptions
{
	FCardParams CardParams;
	int32 Column = 0;
	int32 Row = 0;
};

// "Pairs" game rules
struct FPairsRules
{
	int32 GridResolution = 4;
	int32 CardsNum = 0;
	int32 PairsNum = 0;

protected:
	const int32 MinValue = 10;
	const int32 MaxValue = 99;
	TArray<FCardParams> ExclusiveCards;
	TArray<FCardParams> Cards;

public:
	FPairsRules(int32 InGridResolution);
	FCardParams GetCard(const int32& Index) const { return Cards[Index]; }

protected:
	int32 CalcValue() const;
	FLinearColor CalcColor(const int32& It) const;
};

// Easy difficulty
struct FPairsEasyRules : FPairsRules
{
	FPairsEasyRules() : FPairsRules(4)
	{
	}
};

// Normal difficulty
struct FPairsNormalRules : FPairsRules
{
	FPairsNormalRules() : FPairsRules(6)
	{
	}
};

// Hard difficulty
struct FPairsHardRules : FPairsRules
{
	FPairsHardRules() : FPairsRules(8)
	{
	}
};

// "Pairs" GameMode class
class FPairsGameMode : public FSlateGamesGameModeTyped<FPairsGameMode, SlateGames::EGameMode::Pairs>
{
public:
	FPairsGameMode(int32 DifficultyId);
	virtual void Restart() override;
	virtual void CheckFinishCondition() override;
	virtual TSharedRef<SWidget> CreateWidget() override;

	bool OnCardClicked(const TSharedRef<SPairsCard>& Card);
	static TArray<TSharedPtr<FGameModeDifficulty>> GetDifficultyList();

	TUniquePtr<FPairsRules> Rules;

private:
	TWeakPtr<SPairsCard> LastClickedCard;
	int32 RevealedPairs = 0;
};
