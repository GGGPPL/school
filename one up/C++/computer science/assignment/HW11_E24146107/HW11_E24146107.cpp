/*
 * Assignment: Connect Four Game
 * Student ID: E2413XXXX (Please replace with your ID)
 * Date: 2025/12/21
 * Description: 
 * This program implements a 2-player Connect Four game on a 6x7 board.
 * It handles input validation, simulates gravity for piece placement,
 * checks for win conditions (horizontal, vertical, diagonal), and detects draws.
 */

#include <iostream>
#include <vector>
#include <limits> // For clearing input buffer

using namespace std;

// Constants for board dimensions
const int ROWS = 6;
const int COLS = 7;

// Constants for players
const char PLAYER_1 = 'X';
const char PLAYER_2 = 'O';
const char EMPTY = '.';

// Global board variable
char board[ROWS][COLS];

// Function to initialize the board with empty characters
void initBoard() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = EMPTY;
        }
    }
}

// Function to display the current state of the board
void printBoard() {
    cout << "\n";
    // Print the board rows
    for (int i = 0; i < ROWS; i++) {
        cout << "| ";
        for (int j = 0; j < COLS; j++) {
            cout << board[i][j] << " ";
        }
        cout << "|" << endl;
    }
    // Print the bottom border
    cout << "+---------------+" << endl;
    cout << "  0 1 2 3 4 5 6  " << endl; // Column indices
    cout << "\n";
}

// Function to check if a move is valid (within bounds and column not full)
bool isValidMove(int col) {
    // Check bounds (0 to 6)
    if (col < 0 || col >= COLS) {
        return false;
    }
    // Check if the top cell of the column is empty
    if (board[0][col] != EMPTY) {
        return false;
    }
    return true;
}

// Function to place a piece with "gravity" logic
// It finds the lowest empty row in the selected column
void makeMove(int col, char playerSymbol) {
    for (int i = ROWS - 1; i >= 0; i--) {
        if (board[i][col] == EMPTY) {
            board[i][col] = playerSymbol;
            break; // Piece placed, exit loop
        }
    }
}

// Function to check if the current player has won
bool checkWin(char player) {
    // 1. Check Horizontal (-)
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS - 3; c++) {
            if (board[r][c] == player && 
                board[r][c + 1] == player && 
                board[r][c + 2] == player && 
                board[r][c + 3] == player) {
                return true;
            }
        }
    }

    // 2. Check Vertical (|)
    for (int r = 0; r < ROWS - 3; r++) {
        for (int c = 0; c < COLS; c++) {
            if (board[r][c] == player && 
                board[r + 1][c] == player && 
                board[r + 2][c] == player && 
                board[r + 3][c] == player) {
                return true;
            }
        }
    }

    // 3. Check Diagonal (Top-Left to Bottom-Right) (\)
    for (int r = 0; r < ROWS - 3; r++) {
        for (int c = 0; c < COLS - 3; c++) {
            if (board[r][c] == player && 
                board[r + 1][c + 1] == player && 
                board[r + 2][c + 2] == player && 
                board[r + 3][c + 3] == player) {
                return true;
            }
        }
    }

    // 4. Check Anti-Diagonal (Bottom-Left to Top-Right) (/)
    for (int r = 3; r < ROWS; r++) {
        for (int c = 0; c < COLS - 3; c++) {
            if (board[r][c] == player && 
                board[r - 1][c + 1] == player && 
                board[r - 2][c + 2] == player && 
                board[r - 3][c + 3] == player) {
                return true;
            }
        }
    }

    return false;
}

// Function to check if the board is full (Draw condition)
bool isBoardFull() {
    for (int c = 0; c < COLS; c++) {
        if (board[0][c] == EMPTY) {
            return false; // Found an empty spot in the top row
        }
    }
    return true;
}

int main() {
    initBoard();
    
    char currentPlayer = PLAYER_1;
    bool gameRunning = true;
    int turnCount = 1;

    cout << "=== Connect Four Game ===" << endl;
    cout << "Player 1: " << PLAYER_1 << endl;
    cout << "Player 2: " << PLAYER_2 << endl;
    cout << "Enter column number (0-6) to drop your piece." << endl;

    printBoard();

    while (gameRunning) {
        int col;
        cout << "Turn " << turnCount << " - Player " << (currentPlayer == PLAYER_1 ? "1 (X)" : "2 (O)") << "'s move: ";
        
        // Input validation loop
        while (!(cin >> col) || !isValidMove(col)) {
            // Handle non-integer input or invalid column
            if (cin.fail()) {
                cin.clear(); // clear error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard bad input
            }
            cout << "Invalid move! Please enter a column number (0-6) that is not full: ";
        }

        // Place the piece
        makeMove(col, currentPlayer);
        
        // Show board after iteration
        printBoard();

        // Check for win
        if (checkWin(currentPlayer)) {
            cout << "***********************************" << endl;
            cout << "Congratulations! Player " << (currentPlayer == PLAYER_1 ? "1 (X)" : "2 (O)") << " wins!" << endl;
            cout << "***********************************" << endl;
            gameRunning = false;
        } 
        // Check for draw
        else if (isBoardFull()) {
            cout << "It's a draw! The board is full." << endl;
            gameRunning = false;
        } 
        else {
            // Switch player
            currentPlayer = (currentPlayer == PLAYER_1) ? PLAYER_2 : PLAYER_1;
            turnCount++;
        }
    }

    return 0;
}