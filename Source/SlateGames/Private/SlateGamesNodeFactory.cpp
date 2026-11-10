// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#include "SlateGamesNodeFactory.h"

#include "NumberFactory/Nodes/NumberFactoryNode.h"
#include "NumberFactory/Nodes/SNumberFactoryNode.h"

TSharedPtr<SGraphNode> FSlateGamesNodeFactory::CreateNode(UEdGraphNode* InNode) const
{
	if (UNumberFactoryNode* Node = Cast<UNumberFactoryNode>(InNode))
	{
		return SNew(SNumberFactoryNode, Node);
	}

	return nullptr;
}
