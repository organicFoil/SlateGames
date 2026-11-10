// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#include "NumberFactoryResultNode.h"

#include "SlateGamesGameMode.h"
#include "SlateGamesLib.h"

UNumberFactoryResultNode::UNumberFactoryResultNode()
{
	NodeType = ENumberFactoryNodeType::Result;
}

void UNumberFactoryResultNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, "");
	RegisterPins(EGPD_Input);
}

FText UNumberFactoryResultNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Result"));
}

FLinearColor UNumberFactoryResultNode::GetNodeTitleColor() const
{
	return FLinearColor::White;
}

bool UNumberFactoryResultNode::CanUserDeleteNode() const
{
	return false;
}

bool UNumberFactoryResultNode::CanDuplicateNode() const
{
	return false;
}

void UNumberFactoryResultNode::PinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::PinConnectionListChanged(Pin);
	if (!Pin || Pin->Direction != EGPD_Input) 
		return;

	// Get input value
	FString InputValue = GetValueFromLinkedPin(Pin);

	if (InputValue.IsEmpty() || !NodeValues.IsValidIndex(0))
		return;

	// If Value of this ResultNode is reached, then check other nodes of the graph.
	if (InputValue == NodeValues[0].Value)
	{
		SlateGamesLib::GetGameMode()->CheckFinishCondition();
	}
}

bool UNumberFactoryResultNode::VerifyConnection()
{
	FString InputValue;

	// Get input value
	for (const auto& Pin : GetAllPins())
	{
		if (Pin->Direction == EGPD_Input)
		{
			InputValue = GetValueFromLinkedPin(Pin);
			break;
		}
	}

	// Compare InputValue with target value of ResultNode
	if (InputValue.IsEmpty() || !NodeValues.IsValidIndex(0))
	{
		return false;
	}
	return InputValue == NodeValues[0].Value;
}
