// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#include "SNumberFactorySplitNode.h"

#include "NumberFactorySplitNode.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SNumberFactorySplitNode::Construct(const FArguments& InArgs, UNumberFactorySplitNode* InNode)
{
	GraphNode = InNode;
	/*
	ChildSlot
	[
		// Populate the widget
	];
	*/
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
