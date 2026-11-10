// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#pragma once

enum class EResultScreenType : uint8
{
	Win,
	Defeat
};

class SResultScreen : public SBorder
{
public:
	SLATE_BEGIN_ARGS(SResultScreen)	{}
		SLATE_ARGUMENT(EResultScreenType, Type)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	FReply OnRestartClicked();
};
