#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <cmath>
#include <string>

using namespace std;

const int MAX_WORDS = 8192;
const int MAX_LETTERS = 100;
const int CONJUNCTIONS_SIZE = 18;
const int SHINGLE_SIZE = 3;
const int Z_CAPITAL = 90;
const int A_CAPITAL = 65;
const int UPPERCASE_TO_LOWERCASE = 32;
const int CHAR_0 = 48;
const int CHAR_9 = 57;
const double MAX_UNIQUENESS = 100.0;

string processedOriginalText[MAX_WORDS];
string processedUserText[MAX_WORDS];

bool isSeparator(char);
int processText(string text, string(&processedText)[MAX_WORDS]);
bool isConjunction(char[]);
char toLower(char);
int calculateUniqueness(int originalTextLength, int userTextLength);
bool isShingleUnique(string[], int);
double antiPlagiarism(string text, string fragment);

double antiPlagiarism(string text, string fragment) {
	int originalTextLength = 0;
	int userTextLength = 0;
	double uniqueness = 0.0;

	originalTextLength = processText(text, processedOriginalText);

	userTextLength = processText(fragment, processedUserText);
	if (userTextLength < SHINGLE_SIZE)
	{
		return 0.0;
	}

	uniqueness = calculateUniqueness(originalTextLength, userTextLength);

	return 100.0 - uniqueness;
}

bool isSeparator(char ch)
{
	char separators[] = " ,.!?:;()-_=+@#$%^&*'\"\\/<>[]{}`~";

	for (int i = 0; separators[i] != '\0'; i++)
	{
		if (separators[i] == ch)
		{
			return true;
		}
	}
	return false;
}

int processText(string text, string(&processedText)[MAX_WORDS])
{
	char word[MAX_LETTERS]{};
	int iw = 0;
	int counter = 0;

	for (int i = 0; text[i] != '\0'; i++)
	{
		if (!isSeparator(text[i]) and (text[i] < CHAR_0 or text[i] > CHAR_9))
		{
			word[iw] = text[i];

			if (iw == MAX_LETTERS - 1)
			{
				return counter;
			}

			iw++;

			if (isSeparator(text[i + 1]) or text[i + 1] == '\0')
			{
				word[iw] = '\0';
				iw = 0;

				for (int j = 0; word[j] != '\0'; j++)
				{
					word[j] = toLower(word[j]);
				}

				if (!isConjunction(word) and (counter == 0 or word != processedText[counter - 1]))
				{
					processedText[counter] = word;

					if (counter == MAX_WORDS - 1)
					{
						return counter;
					}
					counter++;
				}
			}
		}
	}

	return counter;
}

int calculateUniqueness(int originalTextLength, int userTextLength)
{
	string shingle[SHINGLE_SIZE];
	int shingleMaxNumber = 0;
	int uniqueShingles = 0;
	int uniqueness = 0;

	if (originalTextLength < SHINGLE_SIZE or userTextLength < SHINGLE_SIZE)
	{
		uniqueness = MAX_UNIQUENESS;
		return uniqueness;
	}

	shingleMaxNumber = userTextLength - SHINGLE_SIZE + 1;
	for (int shingleCount = 0; shingleCount < shingleMaxNumber; shingleCount++)
	{
		for (int i = 0; i < SHINGLE_SIZE; i++)
		{
			shingle[i] = processedUserText[i + shingleCount];
		}

		if (isShingleUnique(shingle, originalTextLength))
		{
			uniqueShingles++;
		}
	}

	uniqueness = round(MAX_UNIQUENESS * uniqueShingles / shingleMaxNumber);

	return uniqueness;
}

bool isConjunction(char word[])
{
	string conjunctions[CONJUNCTIONS_SIZE] = { "and","or","for","nor","at","by","till","to","from","in",
								"of","about","a","an","the","on","but","as" };

	for (int i = 0; i < CONJUNCTIONS_SIZE; i++)
	{
		if (word == conjunctions[i])
		{
			return true;
		}
	}
	return false;
}

bool isShingleUnique(string shingle[], int originalTextLength)
{
	int checkups = originalTextLength - SHINGLE_SIZE + 1;
	int matchingWords = 0;

	for (int i = 0; i < checkups; i++)
	{
		matchingWords = 0;

		for (int j = 0; j < SHINGLE_SIZE; j++)
		{
			if (shingle[j] == processedOriginalText[i + j])
			{
				matchingWords++;
			}
		}

		if (matchingWords == SHINGLE_SIZE)
		{
			return false;
		}
	}

	return true;
}

char toLower(char ch)
{
	if (ch >= A_CAPITAL and ch <= Z_CAPITAL)
	{
		ch += UPPERCASE_TO_LOWERCASE;
	}

	return ch;
}
