// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#pragma once

#include "Widgets/Layout/SBorder.h"

class SUniformGridPanel;
struct FSlotOptions;

class SPairsBoard : public SBorder
{
public:
	void Construct(const FArguments& InArgs);

private:
	void PopulateContainer(const TSharedPtr<SUniformGridPanel>& CardsContainer);
	void AddSlot(const TSharedPtr<SUniformGridPanel>& CardsContainer, const FSlotOptions& Options);
};
