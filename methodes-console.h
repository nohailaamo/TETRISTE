#include <iostream>
#include <map>
#include <algorithm>
#include <conio.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#endif

static int score = 0;

// Enumerations for representing colors and shapes
enum class Color {
    Bleu,
    Jaune,
    Rouge,
    Vert
};

enum class Shape {
    Carre,
    Losange,
    Rond,
    Triangle
};

// Structure to represent a piece
struct Piece {
    Color color;
    Shape shape;
};

// Function to display a piece in the console with color
void afficherPiece(const Piece& piece) {
    const char* colorCode;
    switch (piece.color) {
        case Color::Bleu: colorCode = "\033[34m"; break;  // Blue
        case Color::Jaune: colorCode = "\033[33m"; break; // Yellow
        case Color::Rouge: colorCode = "\033[31m"; break; // Red
        case Color::Vert: colorCode = "\033[32m"; break;  // Green
        default: colorCode = "\033[0m"; // Default color
    }

    const char* shapeChar;
    switch (piece.shape) {
        case Shape::Carre: shapeChar = "■"; break;
        case Shape::Losange: shapeChar = "◊"; break;
        case Shape::Rond: shapeChar = "●"; break;
        case Shape::Triangle: shapeChar = "▲"; break;
        default: shapeChar = "?"; // Default shape
    }

    std::cout << colorCode << shapeChar << " " << "\033[0m"; // Reset color
}

// Function to display the game board in the console
void afficherPlateau(const std::vector<Piece>& board) {
#ifdef _WIN32
    // For Windows, use Windows API to clear the console
    system("cls");
#else
    // For other systems, use ANSI escape codes
    std::cout << "\033[0;0H\033[2J";
#endif

    // Display the board
    for (const auto& piece : board) {
        afficherPiece(piece);
    }
    std::cout << std::endl;
}

// Function to check if the last three pieces on the board have the same color or shape
void checkMatching(std::vector<Piece>& board) {
    size_t i = 0;
    while (i < board.size() - 2) { // Ensure there are at least three elements to compare
        bool matchColor = (board[i].color == board[i + 1].color) && (board[i + 1].color == board[i + 2].color);
        bool matchShape = (board[i].shape == board[i + 1].shape) && (board[i + 1].shape == board[i + 2].shape);

        if (matchColor || matchShape) {
            // Erase three elements starting from position i
            board.erase(board.begin() + i, board.begin() + i + 3);
            // Do not increment i, as we want to check the new set of elements at the same position
            score += 10;
        } else {
            // Only increment i if no elements were erased
            ++i;
        }
    }
}



//################################################################################################

void swapPlacesRightByColor(std::vector<Piece>& board) {
    std::map<Color, std::vector<size_t>> colorIndices;

    // Collect indices of pieces grouped by color
    for (size_t i = 0; i < board.size(); ++i) {
        colorIndices[board[i].color].push_back(i);
    }

    // Perform the rightward swap for each color group with more than one piece
    for (const auto& group : colorIndices) {
        const std::vector<size_t>& indices = group.second;

        // Check if there's more than one piece to swap
        if (indices.size() > 1) {
            // Temporarily store the piece that will move to the last position
            Piece temp = board[indices[0]];

            // Move pieces to the left (in your terms, this is the rightward shift)
            for (size_t i = 0; i < indices.size() - 1; ++i) {
                board[indices[i]] = board[indices[i + 1]];
            }

            // Place the temporarily stored piece in the last position
            board[indices.back()] = temp;
        }
        // If there's only one piece in the group, do nothing (it remains in place)
    }
}


void swapPlacesRightByShape(std::vector<Piece>& board) {
    std::map<Shape, std::vector<size_t>> shapeIndices;

    // Collect indices of pieces grouped by shape
    for (size_t i = 0; i < board.size(); ++i) {
        shapeIndices[board[i].shape].push_back(i);
    }

    // Perform the rightward swap for each shape group with more than one piece
    for (const auto& group : shapeIndices) {
        const std::vector<size_t>& indices = group.second;

        // Check if there's more than one piece to swap
        if (indices.size() > 1) {
            // Temporarily store the piece that will move to the last position
            Piece temp = board[indices[0]];

            // Move pieces to the left (in your terms, this is the rightward shift)
            for (size_t i = 0; i < indices.size() - 1; ++i) {
                board[indices[i]] = board[indices[i + 1]];
            }

            // Place the temporarily stored piece in the last position
            board[indices.back()] = temp;
        }
        // If there's only one piece in the group, do nothing (it remains in place)
    }
}