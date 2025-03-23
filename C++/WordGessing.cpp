#include <iostream>
#include <string>

using namespace std;

class Player
{
public:
    Player(const string &playerName);

    void playGame(Player &opponent);

private:
    string word;
    string displayWord;
    string playerName;

    bool isLetterInWord(char letter, const string &opponentWord, string &opponentDisplayWord) const;
};

Player::Player(const string &playerName) : playerName(playerName)
{
    cout << playerName << ", enter your secret word: ";
    cin >> word;
    displayWord = string(word.length(), '-');
}

bool Player::isLetterInWord(char letter, const string &opponentWord, string &opponentDisplayWord) const
{
    bool letterFound = false;

    for (size_t i = 0; i < opponentWord.length(); ++i)
    {
        if (opponentWord[i] == letter)
        {
            opponentDisplayWord[i] = letter;
            letterFound = true;
        }
    }

    return letterFound;
}

void Player::playGame(Player &opponent)
{
    int attempts = 0;
    char guess;

    cout << "Welcome to the Word Guessing Game!\n";

    do
    {
        cout << "Current word: " << displayWord << "\n";
        cout << playerName << ", enter your guess (one letter): ";
        cin >> guess;

        if (isLetterInWord(guess, opponent.word, opponent.displayWord))
        {
            cout << "Good guess!\n";
        }
        else
        {
            cout << "Incorrect guess. Try again.\n";
        }

        attempts++;

        // Swap players for the next turn
        swap(*this, opponent);

    } while (displayWord != word && opponent.displayWord != opponent.word);

    cout << "Congratulations! " << playerName << " guessed the word and won the game in " << attempts << " attempts.\n";
}

int main()
{
    string name1, name2;

    cout << "Enter Player 1's name: ";
    cin >> name1;

    cout << "Enter Player 2's name: ";
    cin >> name2;

    Player player1(name1);
    Player player2(name2);

    player1.playGame(player2);

    return 0;
}
