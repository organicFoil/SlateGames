// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#pragma once

#include "EdGraph/EdGraph.h"
#include "NumberFactoryGraph.generated.h"

UCLASS()
class UNumberFactoryGraph : public UEdGraph
{
	GENERATED_BODY()

protected:
	UNumberFactoryGraph();

public:
	virtual void PostEditUndo() override;

	FGraphAppearanceInfo GetAppearanceInfo();
	FGraphPanelSelectionSet GetSelectedNodes() const;
	void DeleteSelectedDuplicatableNodes();

	void DeleteSelectedNodes();
	bool CanDeleteNodes() const;
	void CopySelectedNodes();
	bool CanCopyNodes() const;
	void CutSelectedNodes();
	bool CanCutNodes() const;
	void PasteNodes();
	bool CanPasteNodes() const;
	void DuplicateNodes();
	bool CanDuplicateNodes() const;
	void SelectAllNodes();
	bool CanSelectAllNodes() const;

	TSharedRef<SWidget> CreateGraphWidget();

private:
	TSharedPtr<FUICommandList> GraphEditorCommands;
	TSharedPtr<SGraphEditor> GraphEditorWidget;
};
