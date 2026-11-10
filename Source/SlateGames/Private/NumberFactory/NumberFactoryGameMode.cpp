// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#include "NumberFactoryGameMode.h"

#include "SlateGamesLib.h"
#include "Algo/RandomShuffle.h"
#include "Graph/NumberFactoryGraph.h"
#include "Nodes/NumberFactoryNode.h"
#include "Nodes/NumberFactoryResultNode.h"
#include "Nodes/NumberFactorySourceNode.h"

void FValueGeneration::GenerateSourceValues()
{
	SourceValues.Reset();

	for (int32 i = 0; i < SourceValuesNum; ++i)
	{
		int32 SourceStringLen = FMath::RandRange(SourceStringLenMin, SourceStringLenMax);

		FString Value;
		Value.Reserve(SourceStringLen);

		for (int32 j = 0; j < SourceStringLen; ++j)
		{
			Value.AppendChar(TEXT('0') + FMath::RandRange(0, 9));
		}

		SourceValues.Add(MoveTemp(Value));
	}
}

void FValueGeneration::GenerateResultValues()
{
	ResultValues.Reset();

	// Collect all characters from Source array
	TArray<TCHAR> Characters;
	for (const FString& Value : SourceValues)
	{
		for (TCHAR Ch : Value)
		{
			Characters.Add(Ch);
		}
	}

	// Clamp ResultStringLen
	int32 SourceCharactersCount = Characters.Num();
	const int32 ResultLength = FMath::Floor(FMath::Min(SourceCharactersCount / ResultValuesNum, ResultStringLen));

	// Num check
	int32 ResultCharactersCount = ResultValuesNum * ResultLength;
	check(ResultCharactersCount > 0 && Characters.Num() >= ResultCharactersCount);

	// Shuffle all characters
	Algo::RandomShuffle(Characters);

	int32 CharIndex = 0;

	for (int32 i = 0; i < ResultValuesNum; ++i)
	{
		FString Value;
		Value.Reserve(ResultLength);

		for (int32 j = 0; j < ResultLength; ++j)
		{
			Value.AppendChar(Characters[CharIndex++]);
		}

		ResultValues.Add(MoveTemp(Value));
	}
}

int32 FNumberFactoryRules::GetSourceValuesNum() const
{
	return ValueGeneration.SourceValues.Num();
}

FString FNumberFactoryRules::GetSourceValue(int32 Id)
{
	if (ValueGeneration.SourceValues.IsValidIndex(Id))
	{
		return ValueGeneration.SourceValues[Id];
	}
	return FString();
}

int32 FNumberFactoryRules::GetResultValuesNum() const
{
	return ValueGeneration.ResultValues.Num();
}

FString FNumberFactoryRules::GetResultValue(int32 Id)
{
	if (ValueGeneration.ResultValues.IsValidIndex(Id))
	{
		return ValueGeneration.ResultValues[Id];
	}
	return FString();
}

#define LOCTEXT_NAMESPACE "NumberFactoryGameMode"

FNumberFactoryGameMode::FNumberFactoryGameMode(int32 DifficultyId)
{
	CurrentDifficulty = DifficultyId;
}

void FNumberFactoryGameMode::Restart()
{
	switch (CurrentDifficulty)
	{
	case 0:
		Rules = MakeUnique<FNumberFactoryEasyRules>();
		break;
	case 1:
		Rules = MakeUnique<FNumberFactoryNormalRules>();
		break;
	case 2:
	default:
		Rules = MakeUnique<FNumberFactoryHardRules>();
		break;
	}
	FSlateGamesGameMode::Restart();
}

void FNumberFactoryGameMode::CheckFinishCondition()
{
	if (!GraphAsset)
		return;

	bool bSuccess = true;
	bool bResultNodesExist = false;

	for (auto& Node : GraphAsset->Nodes)
	{
		if (auto ResultNode = Cast<UNumberFactoryResultNode>(Node))
		{
			bResultNodesExist |= true;
			bSuccess &= ResultNode->VerifyConnection();
			if (!bSuccess)
				break;
		}
	}

	if (bSuccess && bResultNodesExist)
	{
		SlateGamesLib::ShowResultScreen();
	}
}

TSharedRef<SWidget> FNumberFactoryGameMode::CreateWidget()
{
	GraphAsset.Reset(NewObject<UNumberFactoryGraph>(GetTransientPackage(), UNumberFactoryGraph::StaticClass(), NAME_None, RF_Transactional));
	TSharedRef<SWidget> Widget = GraphAsset->CreateGraphWidget();

	ConstructNodes();

	return Widget;
}

void FNumberFactoryGameMode::ConstructNodes()
{
	constexpr int32 StepY = 200;
	constexpr int32 ResultNodeOffsetX = 1200;

	// Source nodes construction
	for (int32 Index = 0; Index < Rules->GetSourceValuesNum(); ++Index)
	{
		UNumberFactorySourceNode* Node = NewObject<UNumberFactorySourceNode>(GraphAsset.Get(), UNumberFactorySourceNode::StaticClass(), NAME_None, RF_Transactional);
		Node->NodePosY = Index * StepY;
		Node->CreateNewGuid();
		Node->AllocateDefaultPins();
		if (Node->GetNodeValues().IsValidIndex(0))
		{
			Node->GetNodeValues()[0].Value = Rules->GetSourceValue(Index);
			Node->UpdateCosmeticValue();
		}

		GraphAsset->AddNode(Node, true, false);
	}

	// Result nodes construction
	for (int32 Index = 0; Index < Rules->GetResultValuesNum(); ++Index)
	{
		auto Node = NewObject<UNumberFactoryResultNode>(GraphAsset.Get(), UNumberFactoryResultNode::StaticClass(), NAME_None, RF_Transactional);
		Node->NodePosY = Index * StepY;
		Node->NodePosX = ResultNodeOffsetX;
		Node->CreateNewGuid();
		Node->AllocateDefaultPins();

		if (Node->GetNodeValues().IsValidIndex(0))
		{
			Node->GetNodeValues()[0].Value = Rules->GetResultValue(Index);
			Node->UpdateCosmeticValue();
		}

		GraphAsset->AddNode(Node, true, false);
	}
}

TArray<TSharedPtr<FGameModeDifficulty>> FNumberFactoryGameMode::GetDifficultyList()
{
	TArray<TSharedPtr<FGameModeDifficulty>> DifficultyList;
	DifficultyList.Add(MakeShared<FGameModeDifficulty>(LOCTEXT("DifficultyEasy", "Easy"), 0));
	DifficultyList.Add(MakeShared<FGameModeDifficulty>(LOCTEXT("DifficultyNormal", "Normal"), 1));
	DifficultyList.Add(MakeShared<FGameModeDifficulty>(LOCTEXT("DifficultyHard", "Hard"), 2));
	return DifficultyList;
}

#undef LOCTEXT_NAMESPACE