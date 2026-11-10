// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#include "NumberFactoryGraph.h"

#include "EdGraphUtilities.h"
#include "SNodePanel.h"
#include "NumberFactory/NumberFactoryGraphSchema.h"
#include "SNumberFactoryGraph.h"
#include "Framework/Commands/GenericCommands.h"
#include "HAL/PlatformApplicationMisc.h"
#include "NumberFactory/Nodes/NumberFactoryNode.h"

#define LOCTEXT_NAMESPACE "NumberFactoryGraph"

UNumberFactoryGraph::UNumberFactoryGraph()
{
	Schema = UNumberFactoryGraphSchema::StaticClass();
}

void UNumberFactoryGraph::PostEditUndo()
{
	Super::PostEditUndo();
	NotifyGraphChanged();
}

FGraphAppearanceInfo UNumberFactoryGraph::GetAppearanceInfo()
{
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = LOCTEXT("GraphCornerText", "Number Factory");
	return AppearanceInfo;
}

FGraphPanelSelectionSet UNumberFactoryGraph::GetSelectedNodes() const
{
	FGraphPanelSelectionSet CurrentSelection;
	if (GraphEditorWidget)
	{
		CurrentSelection = GraphEditorWidget->GetSelectedNodes();
	}
	return CurrentSelection;
}

void UNumberFactoryGraph::DeleteSelectedDuplicatableNodes()
{
	// Cache off the old selection
	const FGraphPanelSelectionSet OldSelectedNodes = GetSelectedNodes();

	// Clear the selection and only select the nodes that can be duplicated
	FGraphPanelSelectionSet RemainingNodes;
	GraphEditorWidget->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(OldSelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if ((Node != NULL) && Node->CanDuplicateNode())
		{
			GraphEditorWidget->SetNodeSelection(Node, true);
		}
		else
		{
			RemainingNodes.Add(Node);
		}
	}

	// Delete the duplicatable nodes
	DeleteSelectedNodes();

	// Reselect whatever's left from the original selection after the deletion
	GraphEditorWidget->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(RemainingNodes); SelectedIter; ++SelectedIter)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter))
		{
			GraphEditorWidget->SetNodeSelection(Node, true);
		}
	}
}

void UNumberFactoryGraph::DeleteSelectedNodes()
{
	if (!GraphEditorWidget) return;

	const FScopedTransaction Transaction( NSLOCTEXT("UnrealEd", "NumberFactoryGraphDeleteSelectedNode", "Delete selected nodes") );

	Modify();

	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();

	GraphEditorWidget->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
	{
		UEdGraphNode* Node = CastChecked<UEdGraphNode>(*NodeIt);

		if (Node->CanUserDeleteNode())
		{
			Node->Modify();
			RemoveNode(Node);
		}
	}
}

bool UNumberFactoryGraph::CanDeleteNodes() const
{
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	if (SelectedNodes.Num() == 1)
	{
		for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
		{
			if(UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter))
			{
				return Node->CanUserDeleteNode();
			}
		}
	}
	return SelectedNodes.Num() > 0;
}

void UNumberFactoryGraph::CopySelectedNodes()
{
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();

	FString ExportedText;

	FEdGraphUtilities::ExportNodesToText(SelectedNodes, /*out*/ ExportedText);
	FPlatformApplicationMisc::ClipboardCopy(*ExportedText);
}

bool UNumberFactoryGraph::CanCopyNodes() const
{
	// If any of the nodes can be duplicated then we should allow copying
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if ((Node != NULL) && Node->CanDuplicateNode())
		{
			return true;
		}
	}
	return false;
}

void UNumberFactoryGraph::CutSelectedNodes()
{
	CopySelectedNodes();

	// Cut should only delete nodes that can be duplicated
	DeleteSelectedDuplicatableNodes();
}

bool UNumberFactoryGraph::CanCutNodes() const
{
	return CanCopyNodes() && CanDeleteNodes();
}

void UNumberFactoryGraph::PasteNodes()
{
	UE::Slate::FDeprecateVector2DResult Location = GraphEditorWidget->GetPasteLocation2f();

	// Undo/Redo support
	const FScopedTransaction Transaction( NSLOCTEXT("UnrealEd", "NumberFactoryGraphPasteNode", "Paste node") );
	Modify();

	// Clear the selection set (newly pasted stuff will be selected)
	GraphEditorWidget->ClearSelectionSet();

	// Grab the text to paste from the clipboard.
	FString TextToImport;
	FPlatformApplicationMisc::ClipboardPaste(TextToImport);

	// Import the nodes
	TSet<UEdGraphNode*> PastedNodes;
	FEdGraphUtilities::ImportNodesFromText(this, TextToImport, /*out*/ PastedNodes);

	//Average position of nodes so we can move them while still maintaining relative distances to each other
	FVector2D AvgNodePosition(0.0f,0.0f);

	for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
	{
		UEdGraphNode* Node = *It;
		AvgNodePosition.X += Node->NodePosX;
		AvgNodePosition.Y += Node->NodePosY;
	}

	if ( PastedNodes.Num() > 0 )
	{
		float InvNumNodes = 1.0f/float(PastedNodes.Num());
		AvgNodePosition.X *= InvNumNodes;
		AvgNodePosition.Y *= InvNumNodes;
	}

	for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
	{
		UEdGraphNode* Node = *It;

		// Select the newly pasted stuff
		GraphEditorWidget->SetNodeSelection(Node, true);

		Node->NodePosX = (Node->NodePosX - AvgNodePosition.X) + Location.X ;
		Node->NodePosY = (Node->NodePosY - AvgNodePosition.Y) + Location.Y ;

		Node->SnapToGrid(SNodePanel::GetSnapGridSize());

		// Give new node a different Guid from the old one
		Node->CreateNewGuid();

		if (auto NumberFactoryNode = Cast<UNumberFactoryNode>(Node))
		{
			// We cant paste ResultNode, so need to register only output pins
			NumberFactoryNode->RegisterPins(EGPD_Output);
		}
	}

	// Update UI
	NotifyGraphChanged();
}

bool UNumberFactoryGraph::CanPasteNodes() const
{
	FString ClipboardContent;
	FPlatformApplicationMisc::ClipboardPaste(ClipboardContent);

	return FEdGraphUtilities::CanImportNodesFromText(this, ClipboardContent);
}

void UNumberFactoryGraph::DuplicateNodes()
{
	CopySelectedNodes();
	PasteNodes();
}

bool UNumberFactoryGraph::CanDuplicateNodes() const
{
	return CanCopyNodes();
}

void UNumberFactoryGraph::SelectAllNodes()
{
	GraphEditorWidget->SelectAllNodes();
}

bool UNumberFactoryGraph::CanSelectAllNodes() const
{
	return true;
}

TSharedRef<SWidget> UNumberFactoryGraph::CreateGraphWidget()
{
	// No need to regenerate the commands.
	if(!GraphEditorCommands.IsValid())
	{
		GraphEditorCommands = MakeShareable( new FUICommandList );

		GraphEditorCommands->MapAction( FGenericCommands::Get().Delete,
				FExecuteAction::CreateUObject( this, &UNumberFactoryGraph::DeleteSelectedNodes ),
				FCanExecuteAction::CreateUObject( this, &UNumberFactoryGraph::CanDeleteNodes )
				);

		GraphEditorCommands->MapAction( FGenericCommands::Get().Copy,
				FExecuteAction::CreateUObject( this, &UNumberFactoryGraph::CopySelectedNodes ),
				FCanExecuteAction::CreateUObject( this, &UNumberFactoryGraph::CanCopyNodes )
				);

		GraphEditorCommands->MapAction( FGenericCommands::Get().Cut,
			FExecuteAction::CreateUObject( this, &UNumberFactoryGraph::CutSelectedNodes ),
			FCanExecuteAction::CreateUObject( this, &UNumberFactoryGraph::CanCutNodes )
			);

		GraphEditorCommands->MapAction( FGenericCommands::Get().Paste,
			FExecuteAction::CreateUObject( this, &UNumberFactoryGraph::PasteNodes ),
			FCanExecuteAction::CreateUObject( this, &UNumberFactoryGraph::CanPasteNodes )
			);

		GraphEditorCommands->MapAction( FGenericCommands::Get().Duplicate,
			FExecuteAction::CreateUObject( this, &UNumberFactoryGraph::DuplicateNodes ),
			FCanExecuteAction::CreateUObject( this, &UNumberFactoryGraph::CanDuplicateNodes )
			);

		GraphEditorCommands->MapAction( FGenericCommands::Get().SelectAll,
			FExecuteAction::CreateUObject( this, &UNumberFactoryGraph::SelectAllNodes ),
			FCanExecuteAction::CreateUObject( this, &UNumberFactoryGraph::CanSelectAllNodes )
			);
	}

	GraphEditorWidget = SNew(SNumberFactoryGraph)
		.Appearance(GetAppearanceInfo())
		.AdditionalCommands(GraphEditorCommands)
		.GraphToEdit(this);

	return GraphEditorWidget.ToSharedRef();
}

#undef LOCTEXT_NAMESPACE