#include <iostream>

using namespace std;

// Function to check Bulls and Cows
void checkGuess(const string &secretCode, const string &guess, int &bulls, int &cows)
{
    bulls = cows = 0;

    // Check for Bulls
    for (int i = 0; i < secretCode.size(); ++i)
    {
        if (guess[i] == secretCode[i])
        {
            bulls++;
        }
    }

    // Check for Cows
    for (int i = 0; i < secretCode.size(); ++i)
    {
        for (int j = 0; j < secretCode.size(); ++j)
        {
            if (i != j && guess[i] == secretCode[j])
            {
                cows++;
            }
        }
    }
}

int main()
{
    string player1Name, player2Name, player1Secret, player2Secret, player1Guess, player2Guess;
    int numDigits; // Variable to store the number of digits
    int player1Bulls, player1Cows, player2Bulls, player2Cows;

    // Input the number of digits for the secret code
    cout << "Enter the number of digits for the secret code: ";
    cin >> numDigits;

    // Input names and secret codes for each player
    cout << "Player 1, enter your name: ";
    cin >> player1Name;
    cout << player1Name << ", enter your " << numDigits << "-digit secret code: ";
    cin >> player1Secret;

    cout << "Player 2, enter your name: ";
    cin >> player2Name;
    cout << player2Name << ", enter your " << numDigits << "-digit secret code: ";
    cin >> player2Secret;

    // Game loop
    do
    {
        // Player 1's turn
        cout << player1Name << ", enter your guess for " << player2Name << "'s code: ";
        cin >> player1Guess;

        checkGuess(player2Secret, player1Guess, player1Bulls, player1Cows);

        cout << "Bulls: " << player1Bulls << " Cows: " << player1Cows << "\n\n";

        if (player1Bulls == numDigits)
        {
            cout << player1Name << " wins! Correct guess for " << player2Name << "'s code.\n";
            break;
        }

        // Player 2's turn
        cout << player2Name << ", enter your guess for " << player1Name << "'s code: ";
        cin >> player2Guess;

        checkGuess(player1Secret, player2Guess, player2Bulls, player2Cows);

        cout << "Bulls: " << player2Bulls << " Cows: " << player2Cows << "\n\n";

        if (player2Bulls == numDigits)
        {
            cout << player2Name << " wins! Correct guess for " << player1Name << "'s code.\n";
            break;
        }

    } while (true);

    return 0;
}
