// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#include "NumberFactoryNode.h"

#define DELIMETER_STRING "  "
#define DEFAULT_STRING "-"

UNumberFactoryNode::UNumberFactoryNode()
{
	NodeType = ENumberFactoryNodeType::Result;
	UNumberFactoryNode::ResetNodeValue();
}

void UNumberFactoryNode::AutowireNewNode(UEdGraphPin* FromPin)
{
	UEdGraphPin* InputPin = *Pins.FindByPredicate([](UEdGraphPin* Pin)
	{
		return Pin->Direction == EGPD_Input;
	});

	if (FromPin && InputPin)
	{
		if (GetSchema()->TryCreateConnection(FromPin, InputPin))
		{
			FromPin->GetOwningNode()->NodeConnectionListChanged();
		}
	}
}

void UNumberFactoryNode::ResetNodeValue()
{
	CosmeticNodeValue = FText::FromString(DEFAULT_STRING);
	for (auto& Element : NodeValues)
	{
		Element.Value = FString();
	}
}

FString UNumberFactoryNode::GetPinValue(FGuid Id) const
{
	const FPinValue* Item = NodeValues.FindByPredicate(
	[&](const FPinValue& V)
	{
		return V.Id == Id;
	});
	if (Item)
		return Item->Value;
	return FString();
}

void UNumberFactoryNode::UpdateCosmeticValue()
{
	FString Result;
	for (int i = 0; i < NodeValues.Num(); ++i)
	{
		FString Prefix = i == 0 ? "" : DELIMETER_STRING;
		Result += Prefix + NodeValues[i].Value;
	}
	CosmeticNodeValue = FText::FromString(Result);
}

TArray<FPinValue>& UNumberFactoryNode::GetNodeValues()
{
	return NodeValues;
}

void UNumberFactoryNode::RegisterPins(EEdGraphPinDirection Direction /* = EGPD_Output */)
{
	for (auto Element : GetAllPins())
	{
		if (Element->Direction == Direction)
		{
			NodeValues.Emplace(Element->PinId);
		}
	}
}

FString UNumberFactoryNode::GetValueFromLinkedPin(UEdGraphPin* ContextPin, int32 LinkedPinIndex)
{
	if (ContextPin && ContextPin->LinkedTo.IsValidIndex(LinkedPinIndex))
	{
		if (auto LinkedPin = ContextPin->LinkedTo[LinkedPinIndex])
		{
			if (auto LinkedNode = Cast<UNumberFactoryNode>(LinkedPin->GetOwningNode()))
			{
				return LinkedNode->GetPinValue(LinkedPin->PinId);
			}
		}
	}
	return FString();
}

void UNumberFactoryNode::RefreshConnectedNodes(TSet<UNumberFactoryNode*>& Visited)
{
	if (Visited.Contains(this))
	{
		return;
	}

	Visited.Add(this);

	for (UEdGraphPin* Pin : GetAllPins())
	{
		if (Pin->Direction == EGPD_Output)
		{
			for (UEdGraphPin* LinkedPin : Pin->LinkedTo)
			{
				if (UNumberFactoryNode* Node = Cast<UNumberFactoryNode>(LinkedPin->GetOwningNode()))
				{
					Node->RecalculateValue(LinkedPin);
					Node->RefreshConnectedNodes(Visited);
				}
			}
		}
	}
}
