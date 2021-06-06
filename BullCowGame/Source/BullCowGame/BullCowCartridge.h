// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

struct FBullCowCount
{
	int32 Bulls = 0;
	int32 Cows = 0;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString& PlayerInput) override;
	void SetupGame();
	void EndGame();
	void ProcessGuess(const FString& Guess);
	bool IsIsogram(const FString& Word) const;
	TArray<FString> GetValidWords(const TArray<FString>& WordList) const;
	FBullCowCount GetBullCows(const FString& Guess) const;


	// Your declarations go below!
	private: //Char 1 Byte per character
	FString Hiddenword;  //Float = 32 bit again 4 Bytes
	int32 Lives; //32 bits, 32 / 8 = 4. int32 takes 4 Bytes space of memory
	bool bGameOver; //a 1 bit, but takes up 1 Byte
	TArray<FString> Words; //Dynamic at creation = Chars + '\0'
	TArray<FString> Isograms;
	//Memory 16 counts = 0x0000001F

};