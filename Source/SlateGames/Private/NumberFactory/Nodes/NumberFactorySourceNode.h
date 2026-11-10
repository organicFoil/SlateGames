// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#pragma once

#include "NumberFactoryNode.h"
#include "NumberFactorySourceNode.generated.h"

UCLASS()
class UNumberFactorySourceNode : public UNumberFactoryNode
{
	GENERATED_BODY()

public:
	UNumberFactorySourceNode();
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual bool CanUserDeleteNode() const override;
	virtual bool CanDuplicateNode() const override;
};
