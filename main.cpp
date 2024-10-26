#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

using namespace std;

const int N = 3;                             // Size of the board (3x3)
const int MAX_PIECES = 6;                    // Max number of pieces that can be placed in one cell
const string DATA_FILE = "players_data.txt"; // File to store players' data

struct Cell
{
    char pieces[MAX_PIECES]; // Store player pieces 'B1', 'B2', 'B3', 'R1', etc.
    int pieceCount;          // How many pieces are in the cell

    Cell()
    {
        pieceCount = 0;
        memset(pieces, 0, sizeof(pieces)); // Initialize all pieces as empty
    }

    // Add a piece to the top of the stack in the cell
    void addPiece(char player, int size)
    {
        pieces[pieceCount] = player;
        pieces[pieceCount + 1] = size + '0'; // Store size as a char
        pieceCount += 2;                     // Each piece takes two spots (player and size)
    }

    // Remove the top piece from the stack in the cell
    void removeTopPiece()
    {
        if (pieceCount >= 2)
            pieceCount -= 2; // Remove the top two characters (player and size)
    }

    // Get the top piece player ('B' or 'R')
    char getTopPlayer()
    {
        if (pieceCount >= 2)
            return pieces[pieceCount - 2]; // The player is stored before the size

        return 0; // Empty
    }

    // Get the top piece size (1, 2, or 3)
    int getTopSize()
    {
        if (pieceCount >= 2)
            return pieces[pieceCount - 1] - '0'; // Convert the char size to int

        return 0; // Empty
    }

    // Check if the cell is empty
    bool isEmpty()
    {
        return pieceCount == 0;
    }
};

// Game board (3x3) with stacks of pieces
Cell board[N][N];

void initBoard()
{
    // Initialize the board by clearing all cells
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            board[i][j] = Cell(); // Reset each cell
}

void printBoard()
{
    // Print the current state of the board
    cout << "\nCurrent board:\n";
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (board[i][j].isEmpty())
                cout << "#   ";
            else
            {
                // Get the player and size of the top piece
                char player = board[i][j].getTopPlayer();
                int size = board[i][j].getTopSize();
                size++;
                // Print with color based on the player
                if (player == 'B')
                {
                    cout << player << size << "  ";
                }
                else if (player == 'R')
                {
                    cout << player << size << "  ";
                }
            }
        }
        cout << endl;
    }
    cout << endl;
}

bool checkWin(char &winner)
{
    // Check if either player has aligned three pieces in a row, column, or diagonal
    for (int i = 0; i < N; i++)
    {
        // Check rows for both players
        if (board[i][0].getTopPlayer() == 'B' && board[i][1].getTopPlayer() == 'B' && board[i][2].getTopPlayer() == 'B')
        {
            winner = 'B';
            return true;
        }

        if (board[i][0].getTopPlayer() == 'R' && board[i][1].getTopPlayer() == 'R' && board[i][2].getTopPlayer() == 'R')
        {
            winner = 'R';
            return true;
        }

        // Check columns for both players
        if (board[0][i].getTopPlayer() == 'B' && board[1][i].getTopPlayer() == 'B' && board[2][i].getTopPlayer() == 'B')
        {
            winner = 'B';
            return true;
        }

        if (board[0][i].getTopPlayer() == 'R' && board[1][i].getTopPlayer() == 'R' && board[2][i].getTopPlayer() == 'R')
        {
            winner = 'R';
            return true;
        }
    }

    // Check diagonals for both players
    if (board[0][0].getTopPlayer() == 'B' && board[1][1].getTopPlayer() == 'B' && board[2][2].getTopPlayer() == 'B')
    {
        winner = 'B';
        return true;
    }

    if (board[0][0].getTopPlayer() == 'R' && board[1][1].getTopPlayer() == 'R' && board[2][2].getTopPlayer() == 'R')
    {
        winner = 'R';
        return true;
    }

    if (board[0][2].getTopPlayer() == 'B' && board[1][1].getTopPlayer() == 'B' && board[2][0].getTopPlayer() == 'B')
    {
        winner = 'B';
        return true;
    }

    if (board[0][2].getTopPlayer() == 'R' && board[1][1].getTopPlayer() == 'R' && board[2][0].getTopPlayer() == 'R')
    {
        winner = 'R';
        return true;
    }

    return false;
}

bool placePiece(int x, int y, char player, int size)
{
    // Check if the placement is valid
    if (x < 0 || x >= N || y < 0 || y >= N || (!board[x][y].isEmpty() && board[x][y].getTopSize() >= size))
    {
        cout << "Invalid move. Try again.\n";
        return false;
    }

    // Place the piece by adding it to the cell
    board[x][y].addPiece(player, size);

    return true;
}

bool movePiece(int x1, int y1, int x2, int y2, char player)
{
    // Validate move
    if (x1 < 0 || x1 >= N || y1 < 0 || y1 >= N || x2 < 0 || x2 >= N || y2 < 0 || y2 >= N)
    {
        cout << "Invalid move. Out of bounds.\n";
        return false;
    }

    if (board[x1][y1].isEmpty() || board[x1][y1].getTopPlayer() != player)
    {
        cout << "Invalid move. No piece of yours at the source location.\n";
        return false;
    }

    int size = board[x1][y1].getTopSize();
    if (!board[x2][y2].isEmpty() && board[x2][y2].getTopSize() >= size)
    {
        cout << "Invalid move. Cannot place a smaller or equal-sized piece on top of a larger one.\n";
        return false;
    }

    // Move the top piece from (x1, y1) to (x2, y2)
    board[x2][y2].addPiece(player, size);
    board[x1][y1].removeTopPiece();

    return true;
}

bool isDraw()
{
    // Check if the board is full
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (board[i][j].isEmpty())
                return false;

    return true;
}

// Function to read player data from file
bool readPlayerData(const string &player1, const string &player2, int &wins1, int &wins2)
{
    ifstream file(DATA_FILE);
    if (!file)
        return false;

    string p1, p2;
    int w1, w2;

    while (file >> p1 >> p2 >> w1 >> w2)
    {
        if ((p1 == player1 && p2 == player2) || (p1 == player2 && p2 == player1))
        {
            wins1 = (p1 == player1) ? w1 : w2;
            wins2 = (p1 == player1) ? w2 : w1;
            return true;
        }
    }
    return false;
}

// Function to update player data
void updatePlayerData(const string &player1, const string &player2, int wins1, int wins2)
{
    ifstream file(DATA_FILE);
    ofstream temp("temp.txt");

    string p1, p2;
    int w1, w2;
    bool updated = false;

    // Copy all data to the temp file, and update the win count for the current players
    while (file >> p1 >> p2 >> w1 >> w2)
    {
        if ((p1 == player1 && p2 == player2) || (p1 == player2 && p2 == player1))
        {
            // Update the win count for the current players
            if (p1 == player1)
                temp << player1 << " " << player2 << " " << wins1 << " " << wins2 << endl;
            else
                temp << player1 << " " << player2 << " " << wins2 << " " << wins1 << endl;
            updated = true;
        }
        else
        {
            temp << p1 << " " << p2 << " " << w1 << " " << w2 << endl;
        }
    }

    // If the players are not found in the file, add their new data
    if (!updated)
        temp << player1 << " " << player2 << " " << wins1 << " " << wins2 << endl;

    // Close the files and replace the original file with the updated temp file
    file.close();
    temp.close();
    remove(DATA_FILE.c_str());
    rename("temp.txt", DATA_FILE.c_str());
}

int main()
{
    string player1, player2;
    char player = 'B'; // 'B' for player1, 'R' for player2
    // Available pieces for each player: 2 of size 1, 2, and 3
    int player1Pieces[3] = {2, 2, 2};
    int player2Pieces[3] = {2, 2, 2};
    char winner;
    int x, y, size, choice, x1, y1;
    bool gameWon = false;

    // Get the usernames of the two players
    cout << "Enter Player 1 username: ";
    cin >> player1;
    cout << "Enter Player 2 username: ";
    cin >> player2;

    int wins1 = 0, wins2 = 0;
    bool previousGame = readPlayerData(player1, player2, wins1, wins2);

    if (previousGame)
    {
        cout << "Previous games between " << player1 << " and " << player2 << " found.\n";
        cout << player1 << " wins: " << wins1 << ", " << player2 << " wins: " << wins2 << "\n";

        // Determine who starts based on wins
        if (wins2 > wins1)
        {
            player = 'R'; // Player 2 starts
            cout << player2 << " will start first.\n";
        }
        else
        {
            player = 'B'; // Player 1 starts
            cout << player1 << " will start first.\n";
        }
    }
    else
    {
        cout << "No previous games found between " << player1 << " and " << player2 << ".\n";
    }

    initBoard();
    printBoard();

    while (!gameWon && !isDraw())
    {
        // Alternate turns
        if (player == 'B')
            cout << player1 << "'s turn.\n";
        else
            cout << player2 << "'s turn.\n";

        // Choose to place or move a piece
        cout << "Enter 1 to place a piece, 2 to move a piece: ";
        cin >> choice;

        if (choice == 1)
        {
            if (player == 'B')
            {
                cout << "Remaining pieces: [Size 1: " << player1Pieces[0]
                     << ", Size 2: " << player1Pieces[1] << ", Size 3: " << player1Pieces[2] << "]\n";
            }
            else if (player == 'R')
            {
                cout << "Remaining pieces: [Size 1: " << player2Pieces[0]
                     << ", Size 2: " << player2Pieces[1] << ", Size 3: " << player2Pieces[2] << "]\n";
            }
            // Place a piece
            cout << "Enter size of piece to place (1, 2, or 3): ";
            cin >> size;
            size--;
            if ((player == 'B' && player1Pieces[size] == 0) || (player == 'R' && player2Pieces[size] == 0))
            {
                cout << "No pieces of this size available. Try again.\n";
                continue;
            }

            cout << "Enter coordinates (row and column) to place piece (1-3): ";
            cin >> x >> y;
            x--, y--;

            if (placePiece(x, y, player, size))
            {
                if (player == 'B')
                    player1Pieces[size]--;
                else
                    player2Pieces[size]--;
            }
            else
            {
                continue;
            }
        }
        else if (choice == 2)
        {
            // Move a piece
            cout << "Enter coordinates of the piece to move (row and column): ";
            cin >> x1 >> y1;
            x1--, y1--;
            cout << "Enter new coordinates to move piece to (row and column): ";
            cin >> x >> y;
            x--, y--;

            if (!movePiece(x1, y1, x, y, player))
            {
                continue;
            }
        }

        printBoard();
        gameWon = checkWin(winner);

        // Switch players
        player = (player == 'B') ? 'R' : 'B';
    }

    if (gameWon)
    {
        cout << (winner == 'B' ? player1 : player2) << " wins!!!\n";

        if (winner == 'B')
            wins1++;
        else
            wins2++;

        // Update the players' win data in the file
        updatePlayerData(player1, player2, wins1, wins2);
    }
    else
    {
        cout << "Nobody wins!!!\n";
    }

    return 0;
}