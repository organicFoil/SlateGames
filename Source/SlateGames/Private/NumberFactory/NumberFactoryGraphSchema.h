// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#pragma once

#include "EdGraph/EdGraphSchema.h"
#include "NumberFactoryGraphSchema.generated.h"

UCLASS()
class UNumberFactoryGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()

	virtual FLinearColor GetPinColor(const UEdGraphPin* InPin) const override;
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	virtual void GetContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
};
