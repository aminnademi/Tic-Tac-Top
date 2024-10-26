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