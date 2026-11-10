// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#pragma once

#include "NumberFactory/NumberFactoryNodeTypes.h"
#include "EdGraph/EdGraphNode.h"
#include "NumberFactoryNode.generated.h"

struct FPinValue
{
	FPinValue() = default;
	FPinValue(FGuid Id) : Id(Id) { }

	FGuid Id;
	FString Value;
};

UCLASS(Abstract)
class UNumberFactoryNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UNumberFactoryNode();
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;

	virtual void ResetNodeValue();
	virtual void RecalculateValue(UEdGraphPin* ContextPin = nullptr) { }
	FString GetPinValue(FGuid Id) const;
	void UpdateCosmeticValue();
	TArray<FPinValue>& GetNodeValues();
	void RegisterPins(EEdGraphPinDirection Direction = EGPD_Output);

protected:
	/** 
	 * @param ContextPin Pin to search links for
	*/
	FString GetValueFromLinkedPin(UEdGraphPin* ContextPin, int32 LinkedPinIndex = 0);
	void RefreshConnectedNodes(TSet<UNumberFactoryNode*>& Visited);

public:
	FText CosmeticNodeValue;

protected:
	ENumberFactoryNodeType NodeType;
	TArray<FPinValue> NodeValues;
};