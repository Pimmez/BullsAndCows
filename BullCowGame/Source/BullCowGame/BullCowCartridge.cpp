// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);    
    
    Isograms = GetValidWords(Words);

    SetupGame(); //Setting up game
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if(bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else //Checking PlayerGuess
    {
        ProcessGuess(PlayerInput);
    }    
}

void UBullCowCartridge::SetupGame()
{
    PrintLine(TEXT("Welcome to the Bull-Cow Game!"));
   
    Hiddenword = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    Lives = Hiddenword.Len() * 2; //Set Lives
    bGameOver = false;

    PrintLine(FString::Printf(TEXT("Guess the %i letter word!"), Hiddenword.Len()));
    //PrintLine(FString::Printf(TEXT("The Hidden word is: %s."), *Hiddenword)); //Debug line
    PrintLine(FString::Printf(TEXT("You start with %i lives"), Lives));
    PrintLine(TEXT("Type in your guess,  \npress enter to continue..."));

}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == Hiddenword)
    {
        PrintLine(TEXT("You did it! You have won!"));
        EndGame();
        return;
    }   

    //prompt to guess 
    //Check right number of characters
    if (Guess.Len() != Hiddenword.Len())
    {
        PrintLine(FString::Printf(TEXT("The hidden word is %i letters long"), Hiddenword.Len()));
        PrintLine(FString::Printf(TEXT("Sorry try guessing again! \nYou have %i lives remaining"), Lives));
        return;
    }
    
    //Check if right Isogram
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters, guess again"));
        return;
    }

    //Remove the life;
    PrintLine(TEXT("Lost a life!"));
    --Lives;
    
    //check if lives > 0
    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left!"));
        PrintLine(FString::Printf(TEXT("The hidden word was: %s"), *Hiddenword));
        EndGame();
        return;
    }
    
    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(FString::Printf(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows));
    PrintLine(FString::Printf(TEXT("Guess again, you have %i lives left"), Lives));
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() >= 3 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }
    
    return ValidWords;  
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == Hiddenword[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < Hiddenword.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == Hiddenword[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}