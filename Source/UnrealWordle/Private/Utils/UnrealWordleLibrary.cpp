// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/UnrealWordleLibrary.h"

#include "Kismet/KismetStringLibrary.h"

bool UUnrealWordleLibrary::LoadWordsFromFile(FString FileName, int32 RequiredWordLength,
                                             TMap<int32, FStringArray>& Words)
{
	// Load Raw Text from File
	FString ParsedText;
	if(!FFileHelper::LoadFileToString(ParsedText, *(FPaths::ProjectContentDir() + FileName)))
	{
		return false;
	}

	// Parse the text so we have one entry per word
	TArray<FString> ParsedWords = UKismetStringLibrary::ParseIntoArray(ParsedText, "\n");

	for(FString& Word: ParsedWords)
	{
		// Make sure the word is Required Length
		if(Word.Len() < RequiredWordLength)
			continue;

		// Make Words Uppercase
		Word.ToUpperInline();

		// Make Sure only valid containers are contained
		bool bContainsOnlyValidCharacter = true;
		for(const wchar_t&  Letter: Word.GetCharArray())
		{
			if(Letter != '\0' && !IsASCIILetter(Letter))
			{
				bContainsOnlyValidCharacter = false;
				break;
			}
		}

		// Add the Word to our map if only valid letters are contained
		if(bContainsOnlyValidCharacter)
		{
			Words.FindOrAdd(Word.Len()).Strings.Add(Word);
		}
	}

	return true;
}

bool UUnrealWordleLibrary::IsLetter(FString String)
{
	if(String.Len() != 1)
		return false;

	return IsASCIILetter(String.ToUpper().GetCharArray()[0]);
}

bool UUnrealWordleLibrary::IsASCIILetter(char c)
{
	return (c >= 65 && c <= 90);
}
