// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#pragma once

#include "EdGraphUtilities.h"

class FSlateGamesNodeFactory : public FGraphPanelNodeFactory
{
public:
	virtual TSharedPtr<SGraphNode> CreateNode(UEdGraphNode* InNode) const override;
};
