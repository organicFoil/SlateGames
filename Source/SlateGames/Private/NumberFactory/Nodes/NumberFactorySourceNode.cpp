// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#include "NumberFactorySourceNode.h"

UNumberFactorySourceNode::UNumberFactorySourceNode()
{
	NodeType = ENumberFactoryNodeType::Source;
}

void UNumberFactorySourceNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, "");
	RegisterPins(EGPD_Output);
}

FText UNumberFactorySourceNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString("Source");
}

FLinearColor UNumberFactorySourceNode::GetNodeTitleColor() const
{
	return FLinearColor::Red;
}

bool UNumberFactorySourceNode::CanUserDeleteNode() const
{
	return false;
}

bool UNumberFactorySourceNode::CanDuplicateNode() const
{
	return false;
}
