// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#include "NumberFactoryShiftNode.h"

UNumberFactoryShiftNode::UNumberFactoryShiftNode()
{
	NodeType = ENumberFactoryNodeType::Shift;
}

void UNumberFactoryShiftNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, "");
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, "");
	RegisterPins(EGPD_Output);
}

FText UNumberFactoryShiftNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Shift"));
}

FLinearColor UNumberFactoryShiftNode::GetNodeTitleColor() const
{
	return FLinearColor::Blue;
}

void UNumberFactoryShiftNode::PinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::PinConnectionListChanged(Pin);
	if (!Pin || Pin->Direction != EGPD_Input) 
		return;

	RecalculateValue(Pin);

	TSet<UNumberFactoryNode*> Visited;
	RefreshConnectedNodes(Visited);
}

void UNumberFactoryShiftNode::RecalculateValue(UEdGraphPin* ContextPin /* = nullptr */)
{
	// Get input value
	FString InputValue = GetValueFromLinkedPin(ContextPin);

	if (InputValue.IsEmpty() || !NodeValues.IsValidIndex(0))
	{
		ResetNodeValue();
		return;
	}

	// Shift a string to left by 1 character
	const int32 Len = InputValue.Len();
	const TCHAR Last = InputValue[Len - 1];
	InputValue.RemoveAt(Len - 1);
	FString Result = FString::ConstructFromPtrSize(&Last, 1) + InputValue;

	// Set and update node values
	NodeValues[0].Value = Result;
	UpdateCosmeticValue();
}
