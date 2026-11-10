// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#pragma once

#include "NumberFactoryNode.h"
#include "NumberFactoryResultNode.generated.h"

UCLASS()
class UNumberFactoryResultNode : public UNumberFactoryNode
{
	GENERATED_BODY()

public:
	UNumberFactoryResultNode();
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual bool CanUserDeleteNode() const override;
	virtual bool CanDuplicateNode() const override;
	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;

	bool VerifyConnection();
};
