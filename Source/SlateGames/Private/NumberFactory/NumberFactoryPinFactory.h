// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#pragma once

#include "EdGraphUtilities.h"
#include "KismetPins/SGraphPinExec.h"

/**
 * GraphNode Pin factory
 */
class FNumberFactoryPinFactory : public FGraphPanelPinFactory
{
public:
	virtual TSharedPtr<class SGraphPin> CreatePin(class UEdGraphPin* Pin) const override;
};
