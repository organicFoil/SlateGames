// Copyright Aleksandr Yakshigulov. All Rights Reserved.

#pragma once

class SMainWindow;

namespace SlateGames
{
	enum class EGameMode : uint8
	{
		Pairs,
		NumberFactory
	};
}

// GameMode type and name
struct FGameModeTypeAndName
{
	FGameModeTypeAndName(FText Name, SlateGames::EGameMode Type)
		: Name(Name),
		Type(Type)
	{
	}

	FText Name;
	SlateGames::EGameMode Type;
};

// GameMode difficulty ID and name
struct FGameModeDifficulty
{
	FGameModeDifficulty(FText Name, int32 Id)
		: Name(Name),
		Id(Id)
	{
	}

	FText Name;
	int32 Id;
};

// Base GameMode class
class FSlateGamesGameMode
{
public:
	virtual ~FSlateGamesGameMode() = default;
	virtual SlateGames::EGameMode GetType() const = 0;
	virtual void Restart() {}
	virtual void CheckFinishCondition() {}
	virtual TSharedRef<SWidget> CreateWidget() { return SNew(SBox); }
	virtual void SetDifficulty(int32 Id) { CurrentDifficulty = Id; }

	template<typename T>
	T* As()
	{
		return (GetType() == T::StaticType()) 
			? static_cast<T*>(this) 
			: nullptr;
	}

	int32 CurrentDifficulty = 0;
};

// CRTP GameMode class
template<typename TDerived, SlateGames::EGameMode TType>
class FSlateGamesGameModeTyped : public FSlateGamesGameMode
{
public:
	SlateGames::EGameMode GetType() const override { return TType; }
	static SlateGames::EGameMode StaticType()      { return TType; }
};