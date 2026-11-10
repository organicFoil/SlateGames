// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#include "SNumberFactoryShiftNode.h"

#include "NumberFactoryShiftNode.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SNumberFactoryShiftNode::Construct(const FArguments& InArgs, UNumberFactoryShiftNode* InNode)
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
