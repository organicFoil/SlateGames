// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#include "NumberFactoryMergeNode.h"

UNumberFactoryMergeNode::UNumberFactoryMergeNode()
{
	NodeType = ENumberFactoryNodeType::Merge;
}

void UNumberFactoryMergeNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, "");
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, "");
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, "");
	RegisterPins(EGPD_Output);
}

FText UNumberFactoryMergeNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Merge"));
}

FLinearColor UNumberFactoryMergeNode::GetNodeTitleColor() const
{
	return FLinearColor::Green;
}

void UNumberFactoryMergeNode::PinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::PinConnectionListChanged(Pin);
	if (!Pin || Pin->Direction != EGPD_Input) 
		return;

	RecalculateValue();

	TSet<UNumberFactoryNode*> Visited;
	RefreshConnectedNodes(Visited);
}

void UNumberFactoryMergeNode::RecalculateValue(UEdGraphPin* ContextPin /* = nullptr */)
{
	// Collect Values
	TArray<FString> Values;
	for (const auto& Pin : GetAllPins())
	{
		if (Pin->Direction == EGPD_Input)
		{
			Values.Emplace(GetValueFromLinkedPin(Pin));
		}
	}

	// Merge values
	FString Result;
	for (const auto& Value : Values)
	{
		Result += Value;
	}

	if (Result.IsEmpty())
	{
		ResetNodeValue();
		return;
	}

	NodeValues[0].Value = Result;
	UpdateCosmeticValue();
}
