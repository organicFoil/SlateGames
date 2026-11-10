// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#pragma once

#include "SNumberFactoryNode.h"

class UNumberFactoryShiftNode;

class SNumberFactoryShiftNode : public SNumberFactoryNode
{
public:
	SLATE_BEGIN_ARGS(SNumberFactoryShiftNode)	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UNumberFactoryShiftNode* InNode);
};
