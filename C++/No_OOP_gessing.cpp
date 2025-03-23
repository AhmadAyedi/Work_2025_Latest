#include <iostream>
#include <string>

using namespace std;

// Function to check if the guessed letter is in the word
bool isLetterInWord(char letter, const string &word, string &displayWord)
{
    bool letterFound = false;
    for (size_t i = 0; i < word.length(); ++i)
    {
        if (word[i] == letter)
        {
            displayWord[i] = letter;
            letterFound = true;
        }
    }
    return letterFound;
}

int main()
{
    int attempts = 0;
    char guess;
    string wordPlayer1, wordPlayer2, displayWordPlayer1, displayWordPlayer2;

    cout << "Player 1, enter your secret word: ";
    cin >> wordPlayer1;
    // displayWordPlayer1 = string(wordPlayer1.length(), '-');
    for (int i = 0; i < wordPlayer1.size(); i++)
        displayWordPlayer1 += "-";

    cout << "Player 2, enter your secret word: ";
    cin >> wordPlayer2;
    // displayWordPlayer2 = string(wordPlayer2.length(), '-');
    for (int i = 0; i < wordPlayer2.size(); i++)
        displayWordPlayer2 += "-";

    bool player1Turn = true;

    cout << "Welcome to the Word Guessing Game!\n";

    do
    {
        cout << "Current word: " << (player1Turn ? displayWordPlayer1 : displayWordPlayer2) << "\n";
        cout << (player1Turn ? "Player 1" : "Player 2") << ", enter your guess (one letter): ";
        cin >> guess;

        if (player1Turn)
        {
            if (isLetterInWord(guess, wordPlayer2, displayWordPlayer2))
            {
                cout << "Good guess!\n";
            }
            else
            {
                cout << "Incorrect guess. Try again.\n";
            }
        }
        else
        {
            if (isLetterInWord(guess, wordPlayer1, displayWordPlayer1))
            {
                cout << "Good guess!\n";
            }
            else
            {
                cout << "Incorrect guess. Try again.\n";
            }
        }

        attempts++;
        player1Turn = !player1Turn;

    } while (displayWordPlayer1 != wordPlayer1 && displayWordPlayer2 != wordPlayer2);

    cout << "Congratulations! " << (player1Turn ? "Player 1" : "Player 2") << " guessed the word and won the game in " << attempts << " attempts.\n";

    return 0;
}
