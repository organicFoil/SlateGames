// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#pragma once

#include "NumberFactoryNode.h"
#include "NumberFactoryShiftNode.generated.h"

UCLASS()
class UNumberFactoryShiftNode : public UNumberFactoryNode
{
	GENERATED_BODY()

public:
	UNumberFactoryShiftNode();
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;
	virtual void RecalculateValue(UEdGraphPin* ContextPin = nullptr) override;
};
