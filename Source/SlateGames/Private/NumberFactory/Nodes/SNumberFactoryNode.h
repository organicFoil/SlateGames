// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#pragma once

#include "SGraphNode.h"

class UNumberFactoryNode;

class SNumberFactoryNode : public SGraphNode
{
	SLATE_BEGIN_ARGS(SNumberFactoryNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UNumberFactoryNode* InNode);

	virtual void UpdateGraphNode() override;
	virtual TSharedRef<SWidget> CreateNodeContentArea() override;
};
