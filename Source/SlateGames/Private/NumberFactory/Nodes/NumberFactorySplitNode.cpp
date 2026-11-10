// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#include "NumberFactorySplitNode.h"

UNumberFactorySplitNode::UNumberFactorySplitNode()
{
	NodeType = ENumberFactoryNodeType::Split;
}

void UNumberFactorySplitNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, "");
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, "");
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, "");
	RegisterPins(EGPD_Output);
}

FText UNumberFactorySplitNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Split"));
}

FLinearColor UNumberFactorySplitNode::GetNodeTitleColor() const
{
	return FLinearColor::Yellow;
}

void UNumberFactorySplitNode::PinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::PinConnectionListChanged(Pin);
	if (!Pin || Pin->Direction != EGPD_Input) 
		return;

	RecalculateValue(Pin);

	TSet<UNumberFactoryNode*> Visited;
	RefreshConnectedNodes(Visited);
}

void UNumberFactorySplitNode::RecalculateValue(UEdGraphPin* ContextPin /* = nullptr */)
{
	// Get input value
	FString InputValue = GetValueFromLinkedPin(ContextPin);

	// Split value
	int32 ValueLen = InputValue.Len();

	if (ValueLen <= 1 || !NodeValues.IsValidIndex(0) || !NodeValues.IsValidIndex(1))
	{
		ResetNodeValue();
		return;
	}

	NodeValues[0].Value = InputValue.Left((ValueLen + 1) / 2);
	NodeValues[1].Value = InputValue.Right(ValueLen / 2);
	UpdateCosmeticValue();
}
