#include "methodes-console.h"

int main() {
    std::vector<Piece> board = {
        {Color::Bleu, Shape::Rond},
        {Color::Jaune, Shape::Carre},
        {Color::Rouge, Shape::Triangle},
        {Color::Vert, Shape::Losange}
    };

    // Seed for random number generation
    std::srand(static_cast<unsigned int>(std::time(0)));

    // Set the game duration (in seconds)
    int gameDuration = 120;

    auto startTime = std::chrono::high_resolution_clock::now();

    // Generate the first "Next" piece
    Piece newPiece{
        static_cast<Color>(rand() % 4),
        static_cast<Shape>(rand() % 4)
    };

    // Game loop
    while (true) {
        // Calculate elapsed time
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();

        // Check if game duration has been exceeded
        if (elapsedTime >= gameDuration || board.size()>15) {
            std::cout << "Game Over! Time's up!" << std::endl;
            break; // End the game loop
        }

        if (board.size()== 0 ) {
            std::cout << "Congratulations! You've won!" << std::endl;
            break; // End the game loop
        }

        // Display the current board
        afficherPlateau(board);

        // Display the next piece
        std::cout << "Next: ";
        afficherPiece(newPiece);
        std::cout << std::endl;

        // Display the elapsed time
        std::cout << "Time: " << elapsedTime << "s / " << gameDuration << "s" << std::endl;

        // Ask the player to insert the piece to the left or right
        std::cout << "Insert piece to the left (l) or right (r), decalage by shape (s) or color (c): "<<std::endl;

        // Handle user input and board modifications
        if (_kbhit()) {
            char key = _getch();  // Get the pressed key

            if (key == 'l') {
                board.insert(board.begin(), newPiece);
                // Generate a new "Next" piece
                newPiece = {
                    static_cast<Color>(rand() % 4),
                    static_cast<Shape>(rand() % 4)
                };
            } else if (key == 'r') {
                board.push_back(newPiece);
                // Generate a new "Next" piece
                newPiece = {
                    static_cast<Color>(rand() % 4),
                    static_cast<Shape>(rand() % 4)
                };
            } else if (key == 'c') {
                afficherPlateau(board);
                swapPlacesRightByColor(board);
            } else if (key == 's') {
                afficherPlateau(board);
                swapPlacesRightByShape(board);
            } else if (key == 'q') {
                break;
            } else {
                std::cout << "Invalid choice. Please enter 'l', 'r', 'c', or 's'." << std::endl;
                continue;  // Skip the rest of the loop and start a new iteration
            }

            if (board.size()>2) {
                checkMatching(board);
            }
        }
        std::cout << "Score = " << score << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Sleep for 100 milliseconds
    }

    return 0;
}