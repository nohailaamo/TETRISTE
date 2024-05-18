#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cstdlib> // For random number generation
#include <ctime>   // For seeding random number generator
#include <sstream> // For converting score and time to string

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 600;
const int BOARD_CAPACITY = 15;
const int PIECE_SIZE = 40;
const int PREVIEW_OFFSET_X = 50; // Offset for the preview area
const int PREVIEW_OFFSET_Y = 50;

sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Shape Matcher",sf::Style::Titlebar | sf::Style::Close);
sf::Font font;

enum class Shape
{
    Triangle,
    Circle,
    Diamond,
    Rectangle
}; // Define shapes
enum class Color
{
    Red,
    Green,
    Blue,
    Yellow
}; // Define colors

// Structure to represent a piece on the game board
struct Piece
{
    Shape shape;
    Color color;
};

std::vector<Piece> gameBoard;
Piece nextPiece; // Variable to store the next piece
int score = 0;
sf::Clock sfClock; // Renamed sf::Clock to sfClock

// Define a game over sound buffer and sound
sf::SoundBuffer gameOverBuffer;
sf::Sound gameOverSound;

// Function to generate a random piece
Piece generateRandomPiece()
{
    Piece piece;
    piece.shape = static_cast<Shape>(rand() % 4); // Random shape
    piece.color = static_cast<Color>(rand() % 4); // Random color
    return piece;
}

// Function to initialize the game board with random pieces
void initializeGameBoard()
{
    for (int i = 0; i < 3; ++i)
    {
        gameBoard.push_back(generateRandomPiece());
    }
    nextPiece = generateRandomPiece(); // Generate the first next piece
}

void handleInput()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Left)
            {
                // Handle left arrow key press
                gameBoard.insert(gameBoard.begin(), nextPiece); // Insert the next piece
                if (gameBoard.size() > BOARD_CAPACITY)
                {
                    // Game over
                    // Close the window, indicating game over
                    window.close();
                }
                nextPiece = generateRandomPiece(); // Generate a new next piece
            }
            else if (event.key.code == sf::Keyboard::Right)
            {
                // Handle right arrow key press
                gameBoard.push_back(nextPiece); // Add the next piece to the end
                if (gameBoard.size() > BOARD_CAPACITY)
                {
                    // Game over
                    // Close the window, indicating game over
                    window.close();
                }
                nextPiece = generateRandomPiece(); // Generate a new next piece
            }
        }
    }
}

// Function to check for a sequence of three pieces with the same color or shape
void checkForSequences()
{
    int scoreIncrease = 0;
    for (size_t i = 0; i < gameBoard.size() - 2; ++i)
    {
        if ((gameBoard[i].color == gameBoard[i + 1].color && gameBoard[i + 1].color == gameBoard[i + 2].color) ||
            (gameBoard[i].shape == gameBoard[i + 1].shape && gameBoard[i + 1].shape == gameBoard[i + 2].shape))
        {
            gameBoard.erase(gameBoard.begin() + i, gameBoard.begin() + i + 3);
            scoreIncrease += 15;
        }
    }
    if (scoreIncrease > 0)
    {
        score += scoreIncrease;
    }
}

// Function to update game state
void update()
{
    checkForSequences();
}

void render()
{
    window.clear(sf::Color::White);

    // Check if the game is over
    if (gameBoard.size() > BOARD_CAPACITY)
    {
        // Display game over text
        sf::Text gameOverText;
        gameOverText.setFont(font);
        gameOverText.setString("Game Over!");
        gameOverText.setCharacterSize(48);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setStyle(sf::Text::Bold);
        gameOverText.setPosition(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 25);
        window.draw(gameOverText);

        // Play game over sound
        gameOverSound.play();
    }
    else
    {
      window.clear(sf::Color::White);

    for (size_t i = 0; i < gameBoard.size(); ++i)
    {
        sf::Shape *pieceShape = nullptr;

        // Set position based on index
        sf::Vector2f position(50 + i * PIECE_SIZE * 1.5, WINDOW_HEIGHT / 2);

        // Set color based on piece color
        switch (gameBoard[i].color)
        {
        case Color::Red:
            switch (gameBoard[i].shape)
            {
            case Shape::Triangle:
                pieceShape = new sf::CircleShape(PIECE_SIZE / 2, 3); // Equilateral triangle
                pieceShape->setPosition(position);
                break;
            case Shape::Circle:
                pieceShape = new sf::CircleShape(PIECE_SIZE / 2); // Circle
                pieceShape->setPosition(position);
                break;
            case Shape::Diamond:
                pieceShape = new sf::ConvexShape(); // Diamond
                static_cast<sf::ConvexShape *>(pieceShape)->setPointCount(4);
                static_cast<sf::ConvexShape *>(pieceShape)->setPoint(0, sf::Vector2f(PIECE_SIZE / 2, 0));
                static_cast<sf::ConvexShape *>(pieceShape)->setPoint(1, sf::Vector2f(PIECE_SIZE, PIECE_SIZE / 2));
                static_cast<sf::ConvexShape *>(pieceShape)->setPoint(2, sf::Vector2f(PIECE_SIZE / 2, PIECE_SIZE));
                static_cast<sf::ConvexShape *>(pieceShape)->setPoint(3, sf::Vector2f(0, PIECE_SIZE / 2));
                pieceShape->setPosition(position);
                break;
            case Shape::Rectangle:
                pieceShape = new sf::RectangleShape(sf::Vector2f(PIECE_SIZE, PIECE_SIZE)); // Rectangle
                pieceShape->setPosition(position);
                break;
            }
            pieceShape->setFillColor(sf::Color::Red);
            break;
        case Color::Green:
            switch (gameBoard[i].shape)
            {
            case Shape::Triangle:
                pieceShape = new sf::CircleShape(PIECE_SIZE / 2, 3); // Equilateral triangle
                pieceShape->setPosition(position);
                break;
            case Shape::Circle:
                pieceShape = new sf::CircleShape(PIECE_SIZE / 2); // Circle
                pieceShape->setPosition(position);
                break;
            case Shape::Diamond:
                pieceShape = new sf::ConvexShape(); // Diamond
                static_cast<sf::ConvexShape *>(pieceShape)->setPointCount(4);
                static_cast<sf::ConvexShape *>(pieceShape)->setPoint(0, sf::Vector2f(PIECE_SIZE / 2, 0));
                static_cast<sf::ConvexShape *>(pieceShape)->setPoint(1, sf::Vector2f(PIECE_SIZE, PIECE_SIZE / 2));
                static_cast<sf::ConvexShape *>(pieceShape)->setPoint(2, sf::Vector2f(PIECE_SIZE / 2, PIECE_SIZE));
                static_cast<sf::ConvexShape *>(pieceShape)->setPoint(3, sf::Vector2f(0, PIECE_SIZE / 2));
                pieceShape->setPosition(position);
                break;
            case Shape::Rectangle:
                pieceShape = new sf::RectangleShape(sf::Vector2f(PIECE_SIZE, PIECE_SIZE)); // Rectangle
                pieceShape->setPosition(position);
                break;
            }
            pieceShape->setFillColor(sf::Color::Green);
            break;
        case Color::Blue:
            switch (gameBoard[i].shape)
            {
            case Shape::Triangle:
                pieceShape = new sf::CircleShape(PIECE_SIZE / 2, 3); // Equilateral triangle
                pieceShape->setPosition(position);
                break;
            case Shape::Circle:
                pieceShape = new sf::CircleShape(PIECE_SIZE / 2); // Circle
                pieceShape->setPosition(position);
                break;
            case Shape::Diamond:
                pieceShape = new sf::ConvexShape(); // Diamond
                static_cast<sf::ConvexShape *>(pieceShape)->setPointCount(4);
                static_cast<sf::ConvexShape *>(pieceShape)->setPoint(0, sf::Vector2f(PIECE_SIZE / 2, 0));
                static_cast<sf::ConvexShape *>(pieceShape)->setPoint(1, sf::Vector2f(PIECE_SIZE, PIECE_SIZE / 2));
                static_cast<sf::ConvexShape *>(pieceShape)->setPoint(2, sf::Vector2f(PIECE_SIZE / 2, PIECE_SIZE));
                static_cast<sf::ConvexShape *>(pieceShape)->setPoint(3, sf::Vector2f(0, PIECE_SIZE / 2));
                pieceShape->setPosition(position);
                break;
            case Shape::Rectangle:
                pieceShape = new sf::RectangleShape(sf::Vector2f(PIECE_SIZE, PIECE_SIZE)); // Rectangle
                pieceShape->setPosition(position);
                break;
            }
            pieceShape->setFillColor(sf::Color::Blue);
            break;
        case Color::Yellow:
            switch (gameBoard[i].shape)
            {
            case Shape::Triangle:
                pieceShape = new sf::CircleShape(PIECE_SIZE / 2, 3); // Equilateral triangle
                pieceShape->setPosition(position);
                break;
            case Shape::Circle:
                pieceShape = new sf::CircleShape(PIECE_SIZE / 2); // Circle
                pieceShape->setPosition(position);
                break;
            case Shape::Diamond:
                pieceShape = new sf::ConvexShape(); // Diamond
                static_cast<sf::ConvexShape *>(pieceShape)->setPointCount(4);
                static_cast<sf::ConvexShape *>(pieceShape)->setPoint(0, sf::Vector2f(PIECE_SIZE / 2, 0));
                static_cast<sf::ConvexShape *>(pieceShape)->setPoint(1, sf::Vector2f(PIECE_SIZE, PIECE_SIZE / 2));
                static_cast<sf::ConvexShape *>(pieceShape)->setPoint(2, sf::Vector2f(PIECE_SIZE / 2, PIECE_SIZE));
                static_cast<sf::ConvexShape *>(pieceShape)->setPoint(3, sf::Vector2f(0, PIECE_SIZE / 2));
                pieceShape->setPosition(position);
                break;
            case Shape::Rectangle:
                pieceShape = new sf::RectangleShape(sf::Vector2f(PIECE_SIZE, PIECE_SIZE)); // Rectangle
                pieceShape->setPosition(position);
                break;
            }
            pieceShape->setFillColor(sf::Color::Yellow);
            break;
        }
        if (pieceShape != nullptr)
        {
            window.draw(*pieceShape);
            delete pieceShape;
        }
    }

    // Render next piece preview
    sf::Shape *nextPieceShape = nullptr;
    switch (nextPiece.color)
    {
    case Color::Red:
        switch (nextPiece.shape)
        {
        case Shape::Triangle:
            nextPieceShape = new sf::CircleShape(PIECE_SIZE / 2, 3); // Equilateral triangle
            nextPieceShape->setPosition(sf::Vector2f(PREVIEW_OFFSET_X, PREVIEW_OFFSET_Y));
            break;
        case Shape::Circle:
            nextPieceShape = new sf::CircleShape(PIECE_SIZE / 2); // Circle
            nextPieceShape->setPosition(sf::Vector2f(PREVIEW_OFFSET_X, PREVIEW_OFFSET_Y));
            break;
        case Shape::Diamond:
            nextPieceShape = new sf::ConvexShape(); // Diamond
            static_cast<sf::ConvexShape *>(nextPieceShape)->setPointCount(4);
            static_cast<sf::ConvexShape *>(nextPieceShape)->setPoint(0, sf::Vector2f(PIECE_SIZE / 2, 0));
            static_cast<sf::ConvexShape *>(nextPieceShape)->setPoint(1, sf::Vector2f(PIECE_SIZE, PIECE_SIZE / 2));
            static_cast<sf::ConvexShape *>(nextPieceShape)->setPoint(2, sf::Vector2f(PIECE_SIZE / 2, PIECE_SIZE));
            static_cast<sf::ConvexShape *>(nextPieceShape)->setPoint(3, sf::Vector2f(0, PIECE_SIZE / 2));
            nextPieceShape->setPosition(sf::Vector2f(PREVIEW_OFFSET_X, PREVIEW_OFFSET_Y));
            break;
        case Shape::Rectangle:
            nextPieceShape = new sf::RectangleShape(sf::Vector2f(PIECE_SIZE, PIECE_SIZE)); // Rectangle
            nextPieceShape->setPosition(sf::Vector2f(PREVIEW_OFFSET_X, PREVIEW_OFFSET_Y));
            break;
        }
        nextPieceShape->setFillColor(sf::Color::Red);
        break;
    case Color::Green:
        switch (nextPiece.shape)
        {
        case Shape::Triangle:
            nextPieceShape = new sf::CircleShape(PIECE_SIZE / 2, 3); // Equilateral triangle
            nextPieceShape->setPosition(sf::Vector2f(PREVIEW_OFFSET_X, PREVIEW_OFFSET_Y));
            break;
        case Shape::Circle:
            nextPieceShape = new sf::CircleShape(PIECE_SIZE / 2); // Circle
            nextPieceShape->setPosition(sf::Vector2f(PREVIEW_OFFSET_X, PREVIEW_OFFSET_Y));
            break;
        case Shape::Diamond:
            nextPieceShape = new sf::ConvexShape(); // Diamond
            static_cast<sf::ConvexShape *>(nextPieceShape)->setPointCount(4);
            static_cast<sf::ConvexShape *>(nextPieceShape)->setPoint(0, sf::Vector2f(PIECE_SIZE / 2, 0));
            static_cast<sf::ConvexShape *>(nextPieceShape)->setPoint(1, sf::Vector2f(PIECE_SIZE, PIECE_SIZE / 2));
            static_cast<sf::ConvexShape *>(nextPieceShape)->setPoint(2, sf::Vector2f(PIECE_SIZE / 2, PIECE_SIZE));
            static_cast<sf::ConvexShape *>(nextPieceShape)->setPoint(3, sf::Vector2f(0, PIECE_SIZE / 2));
            nextPieceShape->setPosition(sf::Vector2f(PREVIEW_OFFSET_X, PREVIEW_OFFSET_Y));
            break;
        case Shape::Rectangle:
            nextPieceShape = new sf::RectangleShape(sf::Vector2f(PIECE_SIZE, PIECE_SIZE)); // Rectangle
            nextPieceShape->setPosition(sf::Vector2f(PREVIEW_OFFSET_X, PREVIEW_OFFSET_Y));
            break;
        }
        nextPieceShape->setFillColor(sf::Color::Green);
        break;
    case Color::Blue:
        switch (nextPiece.shape)
        {
        case Shape::Triangle:
            nextPieceShape = new sf::CircleShape(PIECE_SIZE / 2, 3); // Equilateral triangle
            nextPieceShape->setPosition(sf::Vector2f(PREVIEW_OFFSET_X, PREVIEW_OFFSET_Y));
            break;
        case Shape::Circle:
            nextPieceShape = new sf::CircleShape(PIECE_SIZE / 2); // Circle
            nextPieceShape->setPosition(sf::Vector2f(PREVIEW_OFFSET_X, PREVIEW_OFFSET_Y));
            break;
        case Shape::Diamond:
            nextPieceShape = new sf::ConvexShape(); // Diamond
            static_cast<sf::ConvexShape *>(nextPieceShape)->setPointCount(4);
            static_cast<sf::ConvexShape *>(nextPieceShape)->setPoint(0, sf::Vector2f(PIECE_SIZE / 2, 0));
            static_cast<sf::ConvexShape *>(nextPieceShape)->setPoint(1, sf::Vector2f(PIECE_SIZE, PIECE_SIZE / 2));
            static_cast<sf::ConvexShape *>(nextPieceShape)->setPoint(2, sf::Vector2f(PIECE_SIZE / 2, PIECE_SIZE));
            static_cast<sf::ConvexShape *>(nextPieceShape)->setPoint(3, sf::Vector2f(0, PIECE_SIZE / 2));
            nextPieceShape->setPosition(sf::Vector2f(PREVIEW_OFFSET_X, PREVIEW_OFFSET_Y));
            break;
        case Shape::Rectangle:
            nextPieceShape = new sf::RectangleShape(sf::Vector2f(PIECE_SIZE, PIECE_SIZE)); // Rectangle
            nextPieceShape->setPosition(sf::Vector2f(PREVIEW_OFFSET_X, PREVIEW_OFFSET_Y));
            break;
        }
        nextPieceShape->setFillColor(sf::Color::Blue);
        break;
    case Color::Yellow:
        switch (nextPiece.shape)
        {
        case Shape::Triangle:
            nextPieceShape = new sf::CircleShape(PIECE_SIZE / 2, 3); // Equilateral triangle
            nextPieceShape->setPosition(sf::Vector2f(PREVIEW_OFFSET_X, PREVIEW_OFFSET_Y));
            break;
        case Shape::Circle:
            nextPieceShape = new sf::CircleShape(PIECE_SIZE / 2); // Circle
            nextPieceShape->setPosition(sf::Vector2f(PREVIEW_OFFSET_X, PREVIEW_OFFSET_Y));
            break;
        case Shape::Diamond:
            nextPieceShape = new sf::ConvexShape(); // Diamond
            static_cast<sf::ConvexShape *>(nextPieceShape)->setPointCount(4);
            static_cast<sf::ConvexShape *>(nextPieceShape)->setPoint(0, sf::Vector2f(PIECE_SIZE / 2, 0));
            static_cast<sf::ConvexShape *>(nextPieceShape)->setPoint(1, sf::Vector2f(PIECE_SIZE, PIECE_SIZE / 2));
            static_cast<sf::ConvexShape *>(nextPieceShape)->setPoint(2, sf::Vector2f(PIECE_SIZE / 2, PIECE_SIZE));
            static_cast<sf::ConvexShape *>(nextPieceShape)->setPoint(3, sf::Vector2f(0, PIECE_SIZE / 2));
            nextPieceShape->setPosition(sf::Vector2f(PREVIEW_OFFSET_X, PREVIEW_OFFSET_Y));
            break;
        case Shape::Rectangle:
            nextPieceShape = new sf::RectangleShape(sf::Vector2f(PIECE_SIZE, PIECE_SIZE)); // Rectangle
            nextPieceShape->setPosition(sf::Vector2f(PREVIEW_OFFSET_X, PREVIEW_OFFSET_Y));
            break;
        }
        nextPieceShape->setFillColor(sf::Color::Yellow);
        break;
    }

    if (nextPieceShape != nullptr)
    {
        // Adjust position for preview
        nextPieceShape->setPosition(PREVIEW_OFFSET_X, PREVIEW_OFFSET_Y);
        window.draw(*nextPieceShape);
        delete nextPieceShape; // Delete the dynamically allocated shape
    }

    // Render score
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(20, 20);
    window.draw(scoreText);

    // Render time
    sf::Text timeText;
    timeText.setFont(font);
    std::stringstream ss;
    ss << "Time: " << sfClock.getElapsedTime().asSeconds() << "s"; // Changed clock to sfClock
    timeText.setString(ss.str());
    timeText.setCharacterSize(24);
    timeText.setFillColor(sf::Color::Black);
    timeText.setPosition(WINDOW_WIDTH - 150, 20);
    window.draw(timeText);

    
    }

    window.display();
}

int main()
{
    // Load font
    if (!font.loadFromFile("arial.ttf"))
    {
        std::cerr << "Failed to load font" << std::endl;
        return 1;
    }

    // Load game over sound from file
    if (!gameOverBuffer.loadFromFile("game_over.wav"))
    {
        std::cerr << "Failed to load game over sound" << std::endl;
        return 1;
    }

    // Set the buffer for the game over sound
    gameOverSound.setBuffer(gameOverBuffer);

    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Initialize game board
    initializeGameBoard();

    // Main game loop
    while (window.isOpen())
    {
        handleInput();
        update();
        render();
    }

    return 0;
}



