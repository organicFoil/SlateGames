// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#include "NumberFactoryGraphSchema.h"

#include "Framework/Commands/GenericCommands.h"
#include "Nodes/NumberFactoryNode.h"
#include "Nodes/NumberFactoryMergeNode.h"
#include "Nodes/NumberFactoryShiftNode.h"
#include "Nodes/NumberFactorySplitNode.h"

FLinearColor UNumberFactoryGraphSchema::GetPinColor(const UEdGraphPin* InPin) const
{
	return FLinearColor::White;
}

const FPinConnectionResponse UNumberFactoryGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	if (!A || !B)
	{
		return FPinConnectionResponse(
			CONNECT_RESPONSE_DISALLOW,
			TEXT("Invalid pin"));
	}

	if (A->GetOwningNode() == B->GetOwningNode())
	{
		return FPinConnectionResponse(
			CONNECT_RESPONSE_DISALLOW,
			TEXT("Cannot connect pins on the same node"));
	}

	if (A->Direction == B->Direction)
	{
		return FPinConnectionResponse(
			CONNECT_RESPONSE_DISALLOW,
			TEXT("Pins must have opposite directions"));
	}

	// Output pin can have multiple connections
	if (A->Direction == EGPD_Output && B->Direction == EGPD_Input)
	{
		return FPinConnectionResponse(
		CONNECT_RESPONSE_BREAK_OTHERS_B,
		TEXT(""));
	}

	// In other case pins must have only one connection
	return FPinConnectionResponse(
		CONNECT_RESPONSE_BREAK_OTHERS_A,
		TEXT(""));
}

void UNumberFactoryGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	auto AddAction = [&ContextMenuBuilder](UClass* NodeClass)
	{
		UNumberFactoryNode* Template = NewObject<UNumberFactoryNode>(GetTransientPackage(), NodeClass);
		FFormatNamedArguments Arguments;
		const FText Name = Template->GetNodeTitle(ENodeTitleType::FullTitle);
		Arguments.Add(TEXT("Name"), Name);
		const FText AddToolTip = FText::Format(FText::FromString("Adds {Name} node here"), Arguments);
		TSharedPtr<FEdGraphSchemaAction_NewNode> NodeAction(new FEdGraphSchemaAction_NewNode(FText::FromString("Number Factory"), Name, AddToolTip, 0));
		NodeAction->NodeTemplate = Template;
		ContextMenuBuilder.AddAction(NodeAction);
	};

	AddAction(UNumberFactorySplitNode::StaticClass());
	AddAction(UNumberFactoryMergeNode::StaticClass());
	AddAction(UNumberFactoryShiftNode::StaticClass());
}

void UNumberFactoryGraphSchema::GetContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const
{
	if (Context->Node)
	{
		FToolMenuSection& Section = Menu->AddSection("NumberFactoryGraphSchemaNodeActions", FText::FromString("Node actions"));
		Section.AddMenuEntry(FGenericCommands::Get().Delete);
		Section.AddMenuEntry(FGenericCommands::Get().Cut);
		Section.AddMenuEntry(FGenericCommands::Get().Copy);
		Section.AddMenuEntry(FGenericCommands::Get().Duplicate);
	}

	Super::GetContextMenuActions(Menu, Context);
}
