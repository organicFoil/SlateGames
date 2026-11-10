// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#pragma once

#include "SNumberFactoryNode.h"

class UNumberFactoryMergeNode;

class SNumberFactoryMergeNode : public SNumberFactoryNode
{
public:
	SLATE_BEGIN_ARGS(SNumberFactoryMergeNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UNumberFactoryMergeNode* InNode);
};
