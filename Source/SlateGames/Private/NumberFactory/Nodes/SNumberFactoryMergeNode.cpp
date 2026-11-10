// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#include "SNumberFactoryMergeNode.h"

#include "NumberFactoryMergeNode.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SNumberFactoryMergeNode::Construct(const FArguments& InArgs, UNumberFactoryMergeNode* InNode)
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
