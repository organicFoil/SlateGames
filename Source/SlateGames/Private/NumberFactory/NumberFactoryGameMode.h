// Copyright Aleksandr Yakshigulov. All Rights Reserved.
// TODO: поправить copyrigth

#pragma once

#include "SlateGamesGameMode.h"
#include "Graph/NumberFactoryGraph.h"

// Generation class for Source and Result nodes
struct FValueGeneration
{
	FValueGeneration() = default;
	FValueGeneration(int32 SourceValuesNum, int32 SourceStringLenMin, int32 SourceStringLenMax, int32 ResultValuesNum, int32 ResultStringLen)
		: SourceValuesNum(SourceValuesNum),
		  SourceStringLenMin(SourceStringLenMin),
		  SourceStringLenMax(SourceStringLenMax),
		  ResultValuesNum(ResultValuesNum),
		  ResultStringLen(ResultStringLen)
	{
		GenerateSourceValues();
		GenerateResultValues();
	}

	TArray<FString> SourceValues;
	TArray<FString> ResultValues;

protected:
	void GenerateSourceValues();
	void GenerateResultValues();

	int32 SourceValuesNum = 0;      // Количество SourceValues
	int32 SourceStringLenMin = 0;   // Min Длина каждого SourceValue
	int32 SourceStringLenMax = 0;   // Max Длина каждого SourceValue
	int32 ResultValuesNum = 0;		// Количество ResultValues
	int32 ResultStringLen = 0;		// Длина каждого ResultValues
};

// "NumberFactory" game rules
struct FNumberFactoryRules
{
	FNumberFactoryRules() = default;

	int32 GetSourceValuesNum() const;
	FString GetSourceValue(int32 Id);
	int32 GetResultValuesNum() const;
	FString GetResultValue(int32 Id);

protected:
	FValueGeneration ValueGeneration = FValueGeneration();
};

// Easy difficulty
struct FNumberFactoryEasyRules : FNumberFactoryRules
{
	FNumberFactoryEasyRules()
	{
		ValueGeneration = FValueGeneration(3, 2 ,2, 1, 3);
	}
};

// Normal difficulty
struct FNumberFactoryNormalRules : FNumberFactoryRules
{
	FNumberFactoryNormalRules()
	{
		ValueGeneration = FValueGeneration(4, 2 ,3, 2, 3);
	}
};

// Hard difficulty
struct FNumberFactoryHardRules : FNumberFactoryRules
{
	FNumberFactoryHardRules()
	{
		ValueGeneration = FValueGeneration(5, 3 ,3, 3, 4);
	}
};

// "Number Factory" GameMode class
class FNumberFactoryGameMode : public FSlateGamesGameModeTyped<FNumberFactoryGameMode, SlateGames::EGameMode::NumberFactory>
{
public:
	FNumberFactoryGameMode(int32 DifficultyId);
	virtual void Restart() override;
	virtual void CheckFinishCondition() override;
	virtual TSharedRef<SWidget> CreateWidget() override;

	void ConstructNodes();
	static TArray<TSharedPtr<FGameModeDifficulty>> GetDifficultyList();

	TUniquePtr<FNumberFactoryRules> Rules;

private:
	TStrongObjectPtr<UNumberFactoryGraph> GraphAsset;
};
