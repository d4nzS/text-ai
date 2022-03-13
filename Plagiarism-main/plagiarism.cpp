#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

// Define constants here
// ...

const int LEN_SHINGLE = 3;

// Put function declaration here
// ...

int textLen(string text);

bool isSeparator(char symbol);

string textToLower(string text);

void separateWords(string text, string words[]);

string delNumbers(string text);

string delUselessWords(string text);

double percentOrig(string plag[], string orig[], int numberWordsPlag, int numberWordsOrig);

double antiPlagiarism(string text, string fragment);

// Put function definition here
// ...

double antiPlagiarism(string text, string fragment) {
    string textPlag = fragment;
    string textOrig = text;

    textPlag = textToLower(textPlag);
    textPlag = delNumbers(textPlag);
    textPlag = delUselessWords(textPlag);

    textOrig = textToLower(textOrig);
    textOrig = delNumbers(textOrig);
    textOrig = delUselessWords(textOrig);

    const int NUMBER_OF_WORDS_PLAG = textLen(textPlag);
    const int NUMBER_OF_WORDS_ORIG = textLen(textOrig);
    string wordsPlag[NUMBER_OF_WORDS_PLAG];
    string wordsOrig[NUMBER_OF_WORDS_ORIG];

    separateWords(textPlag, wordsPlag);
    separateWords(textOrig, wordsOrig);

    return percentOrig(wordsPlag, wordsOrig, NUMBER_OF_WORDS_PLAG, NUMBER_OF_WORDS_ORIG);
}

int textLen(string text) {
    int numberWords = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        if (!isSeparator(text[i])) {
            if (isSeparator(text[i + 1]) or text[i + 1] == '\0') {
                numberWords++;
            }
        }
    }

    return numberWords;
}

bool isSeparator(char symbol) {
    string separators = " !?@#$%^&*():;’”\"',.[]{}/\\*-+_=<>`~";

    for (int i = 0; separators[i] != '\0'; i++) {
        if (separators[i] == symbol) {
            return true;
        }
    }

    return false;
}

string textToLower(string text) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] <= 90 and text[i] >= 65) {
            text[i] += 32;
        }
    }

    return text;
}

void separateWords(string text, string words[]) {
    int indexWord = 0;
    string word;

    for (int i = 0; text[i] != '\0'; i++) {
        if (!isSeparator(text[i])) {
            word += text[i];

            if (isSeparator(text[i + 1]) or text[i + 1] == '\0') {
                words[indexWord] = word;
                indexWord++;
                word = "";
            }
        }
    }
}

string delNumbers(string text) {
    string numbers = "1234567890";
    int size = 10;

    for (int i = 0; text[i] != '\0'; i++) {
        for (int j = 0; j < size; j++) {
            if (text[i] == numbers[j]) {
                text[i] = ' ';
            }
        }
    }

    return text;
}

string delUselessWords(string text) {
    int leftIndexWord = 0;
    int rightIndexWord = 0;
    string word;
    string wordPrev;
    string uselessWords[] = {"and", "or", "for", "nor", "at", "by", "till", "to", "from", "in", "of", "about", "a",
                             "an", "the", "on", "but", "as"};
    int size = sizeof(uselessWords) / sizeof(uselessWords[0]);


    for (int i = 0; text[i] != '\0'; i++) {
        if (!isSeparator(text[i])) {
            word += text[i];

            if (isSeparator(text[i + 1]) or text[i + 1] == '\0') {
                rightIndexWord = i;

                if (word == wordPrev) {
                    for (int j = leftIndexWord; j <= rightIndexWord; j++) {
                        text[j] = ' ';
                    }
                }

                for (int j = 0; j < size; j++) {
                    if (word == uselessWords[j]) {
                        for (int k = leftIndexWord; k <= rightIndexWord; k++) {
                            text[k] = ' ';
                        }
                    }
                }

                wordPrev = word;
                word = "";
            }
        } else {
            leftIndexWord = i + 1;
        }
    }

    return text;
}

string makeShingle(string text[], int firstWord) {
    string shingle;

    for (int i = firstWord; i < firstWord + LEN_SHINGLE; i++) {
        shingle += text[i];
    }

    return shingle;
}

double percentOrig(string plag[], string orig[], int numberWordsPlag, int numberWordsOrig) {
    int sizeShinglesPlag = numberWordsPlag - LEN_SHINGLE + 1;
    int sizeShinglesOrig = numberWordsOrig - LEN_SHINGLE + 1;
    string shinglesPlag[sizeShinglesPlag];
    string shinglesFrag[sizeShinglesOrig];
    double numberShinglePlag = 0.0;
    double percentOrig = 100;

    if (numberWordsPlag < LEN_SHINGLE) {
        return percentOrig;
    }

    for (int i = 0; i < sizeShinglesPlag; i++) {
        shinglesPlag[i] = makeShingle(plag, i);
    }

    for (int i = 0; i < sizeShinglesOrig; i++) {
        shinglesFrag[i] = makeShingle(orig, i);
    }

    for (int i = 0; i < sizeShinglesPlag; i++) {
        for (int j = 0; j < sizeShinglesOrig; j++) {
            if (shinglesPlag[i] == shinglesFrag[j]) {
                numberShinglePlag++;
                break;
            }
        }
    }

    percentOrig = 100 - numberShinglePlag / sizeShinglesPlag * 100;
    return percentOrig;
}
