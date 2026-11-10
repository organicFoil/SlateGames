// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#pragma once

#include "Widgets/Input/SButton.h"

/**
 * Card parameters
 */
struct FCardParams
{
	int32 Value = 0;
	FLinearColor Color = FLinearColor();
};

/**
 * Pair card slate widget
 */
class SPairsCard : public SButton
{
public:
	SLATE_BEGIN_ARGS(SPairsCard) {}
		SLATE_ARGUMENT(FCardParams, Parameters)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	FReply OnClicked();
	void SetOpenState(bool bOpen);
	bool IsOpen() const;
	bool IsPairFor(const TSharedRef<SPairsCard>& TestCard) const;

	bool bPairFound = false;

private:
	FCardParams Parameters;
	TSharedPtr<STextBlock> TextBlock;
};
