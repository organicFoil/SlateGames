// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#pragma once

#include "SNumberFactoryNode.h"

class UNumberFactorySplitNode;

class SNumberFactorySplitNode : public SNumberFactoryNode
{
public:
	SLATE_BEGIN_ARGS(SNumberFactorySplitNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UNumberFactorySplitNode* InNode);
};
