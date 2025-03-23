#include <iostream>
#include <string>

using namespace std;

class Game
{
public:
    virtual void playGame() = 0;
};

class BullsAndCowsGame : public Game
{
private:
    string name1, name2;

    class Player
    {
    private:
        string name;
        string secretCode;
        string guess;
        int bulls;
        int cows;

    public:
        // Constructor to initialize player's name and secret code
        Player(const string &playerName, int numDigits) : name(playerName), bulls(0), cows(0)
        {
            bool validCode = false;
            while (!validCode)
            {
                cout << name << ", enter your " << numDigits << "-digit secret code: ";
                cin >> secretCode;
                validCode = isValidCode(secretCode, numDigits);
                if (!validCode)
                {
                    cout << "Error: Please enter a " << numDigits << "-digit code with no repeated digits.\n";
                }
            }
        }

        // Function to make a guess and check Bulls and Cows
        void makeGuess(const Player &opponent, int numDigits)
        {
            bool validGuess = false;
            while (!validGuess)
            {
                cout << name << ", enter your guess for " << opponent.name << "'s code: ";
                cin >> guess;
                validGuess = isValidCode(guess, numDigits);
                if (!validGuess)
                {
                    cout << "Error: Please enter a " << numDigits << "-digit guess with no repeated digits.\n";
                }
            }

            bulls = cows = 0;

            // Check for Bulls
            for (int i = 0; i < secretCode.size(); ++i)
            {
                if (guess[i] == opponent.secretCode[i])
                {
                    bulls++;
                }
            }

            // Check for Cows
            for (int i = 0; i < secretCode.size(); ++i)
            {
                for (int j = 0; j < secretCode.size(); ++j)
                {
                    if (i != j && guess[i] == opponent.secretCode[j])
                    {
                        cows++;
                    }
                }
            }

            cout << "Bulls: " << bulls << " Cows: " << cows << "\n\n";
        }

        // Function to check if the player has guessed the code
        bool hasGuessed() const
        {
            return bulls == secretCode.size();
        }

        // Getter for the player's name
        string getName() const
        {
            return name;
        }

    private:
        // Function to check if the code is valid (correct number of digits and no repeated digits)
        bool isValidCode(const string &code, int numDigits) const
        {
            if (code.size() != numDigits)
            {
                return false;
            }

            for (int i = 0; i < numDigits; ++i)
            {
                for (int j = i + 1; j < numDigits; ++j)
                {
                    if (code[i] == code[j])
                    {
                        return false;
                    }
                }
            }

            return true;
        }
    };

public:
    void playGame() override
    {
        int numDigits;

        // Input the number of digits for the secret code
        cout << "Enter the number of digits for the secret code: ";
        cin >> numDigits;

        // Input names for each player
        cout << "Enter Player 1's name: ";
        cin >> name1;

        cout << "Enter Player 2's name: ";
        cin >> name2;

        // Create Player objects
        Player player1(name1, numDigits);
        Player player2(name2, numDigits);

        // Game loop
        do
        {
            // Player 1's turn
            player1.makeGuess(player2, numDigits);

            if (player1.hasGuessed())
            {
                cout << player1.getName() << " wins! Correct guess for " << player2.getName() << "'s code.\n";
                break;
            }

            // Player 2's turn
            player2.makeGuess(player1, numDigits);

            if (player2.hasGuessed())
            {
                cout << player2.getName() << " wins! Correct guess for " << player1.getName() << "'s code.\n";
                break;
            }

        } while (true);
    }
};

class WordGuessingGame : public Game
{
private:
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

public:
    void playGame() override
    {
        string name1, name2;

        cout << "Enter Player 1's name: ";
        cin >> name1;

        cout << "Enter Player 2's name: ";
        cin >> name2;

        Player player1(name1);
        Player player2(name2);

        player1.playGame(player2);
    }
};

WordGuessingGame::Player::Player(const string &playerName) : playerName(playerName)
{
    cout << playerName << ", enter your secret word: ";
    cin >> word;
    displayWord = string(word.length(), '-');
}

bool WordGuessingGame::Player::isLetterInWord(char letter, const string &opponentWord, string &opponentDisplayWord) const
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

void WordGuessingGame::Player::playGame(Player &opponent)
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
    int choice;

    cout << "Choose a game:\n";
    cout << "1. Bulls and Cows\n";
    cout << "2. Word Guessing\n";
    cout << "Enter your choice (1 or 2): ";
    cin >> choice;

    Game *game;

    if (choice == 1)
    {
        game = new BullsAndCowsGame();
    }
    else if (choice == 2)
    {
        game = new WordGuessingGame();
    }
    else
    {
        cout << "Invalid choice. Exiting program.\n";
        return 1;
    }

    game->playGame();
}
