#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib> 
#include <ctime>   
#include <iostream>

enum class ShapeType
{
    Rectangle,
    Triangle,
    Losange,
    Circle
};

enum class Color
{
    Red,
    Blue,
    Green,
    Yellow
};

struct Piece
{
    ShapeType shapeType;
    Color color;
};

class Gameboard
{
private:
    static const int BOARD_WIDTH = 15;
    static const int BOARD_HEIGHT = 1;
    std::vector<Piece> board; 

public:
    float timeSinceLastPiece; 
    sf::Clock clock;
    int selectedPieceIndex;

    Gameboard(/* args */);
    ~Gameboard();
    void initialize_Game();
    void generate_Next_Shape();
    void draw(sf::RenderWindow &window);
    void handleInput(sf::RenderWindow &window);
    bool canPlacePiece(int position, Piece piece);
};


Gameboard::Gameboard() : selectedPieceIndex(3)
{
    srand(time(nullptr));
    timeSinceLastPiece = 0.0f;
}

Gameboard::~Gameboard()
{
}

void Gameboard::initialize_Game()
{
    // Generate the initial shapes
    for (int i = 0; i < 3; ++i) // Only generate three shapes initially
    {
        generate_Next_Shape();
    }
}

void Gameboard::generate_Next_Shape()
{
    ShapeType random_shape = static_cast<ShapeType>(rand() % 4);
    Color random_color = static_cast<Color>(rand() % 4);

    Piece new_piece = {random_shape, random_color};
    board.push_back(new_piece);
    selectedPieceIndex = board.size() - 1; // Set the newly added piece as selected.
}

void Gameboard::draw(sf::RenderWindow &window)
{
    float shapeSize = 30.f;
    float xOffset = 10.f;
    float yOffset = 10.f;

    for (int i = 0; i < board.size(); ++i)
    {
        sf::Shape *shape = nullptr;

        switch (board[i].shapeType)
        {
        case ShapeType::Rectangle:
            shape = new sf::RectangleShape(sf::Vector2f(shapeSize, shapeSize));
            break;
        case ShapeType::Triangle:
            shape = new sf::CircleShape(shapeSize / 2, 3);
            break;
        case ShapeType::Losange:
            shape = new sf::ConvexShape();
            dynamic_cast<sf::ConvexShape *>(shape)->setPointCount(4);
            dynamic_cast<sf::ConvexShape *>(shape)->setPoint(0, sf::Vector2f(0, shapeSize / 2));                 
            dynamic_cast<sf::ConvexShape *>(shape)->setPoint(1, sf::Vector2f(shapeSize / 2, 0));                  
            dynamic_cast<sf::ConvexShape *>(shape)->setPoint(2, sf::Vector2f(shapeSize, shapeSize / 2));         
            dynamic_cast<sf::ConvexShape *>(shape)->setPoint(3, sf::Vector2f(shapeSize / 2, shapeSize));           
            break;
        case ShapeType::Circle:
            shape = new sf::CircleShape(shapeSize / 2); 
            break;
        }

        shape->setPosition(xOffset + i * (shapeSize + 5), yOffset);

        switch (board[i].color)
        {
        case Color::Red:
            shape->setFillColor(sf::Color::Red);
            break;
        case Color::Blue:
            shape->setFillColor(sf::Color::Blue);
            break;
        case Color::Green:
            shape->setFillColor(sf::Color::Green);
            break;
        case Color::Yellow:
            shape->setFillColor(sf::Color::Yellow);
            break;
        }

        if (i == selectedPieceIndex)
        {
            sf::RectangleShape border(sf::Vector2f(shapeSize, shapeSize));
            border.setPosition(xOffset + i * (shapeSize + 5), yOffset);
            border.setFillColor(sf::Color::Transparent);
            border.setOutlineColor(sf::Color::White);
            border.setOutlineThickness(2.f);
            window.draw(border);
        }

        window.draw(*shape);
        delete shape; // Deallocate memory
    }
}

void Gameboard::handleInput(sf::RenderWindow &window)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        if (selectedPieceIndex > 0 && canPlacePiece(selectedPieceIndex - 1, board[selectedPieceIndex]))
        {
            std::swap(board[selectedPieceIndex], board[selectedPieceIndex - 1]);
            selectedPieceIndex--;
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        if (selectedPieceIndex < board.size() - 1 && canPlacePiece(selectedPieceIndex + 1, board[selectedPieceIndex]))
        {
            std::swap(board[selectedPieceIndex], board[selectedPieceIndex + 1]);
            selectedPieceIndex++;
        }
    }
}

bool Gameboard::canPlacePiece(int position, Piece piece)
{
    // Check if the piece can be placed next to any existing piece on the board
    for (int i = 0; i < board.size(); ++i)
    {
        if (i != position)
        {
            // Check if the piece can be placed to the left of any existing piece
            if (position > 0 && i == position - 1)
            {
                if (board[i].color == piece.color || board[i].shapeType == piece.shapeType)
                    return true;
            }
            // Check if the piece can be placed to the right of any existing piece
            if (position < board.size() - 1 && i == position + 1)
            {
                if (board[i].color == piece.color || board[i].shapeType == piece.shapeType)
                    return true;
            }
        }
    }

    return false; // Cannot place adjacent
}



int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Game");

    Gameboard game;
    game.initialize_Game();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        game.handleInput(window);

        float deltaTime = game.clock.restart().asSeconds();
        game.timeSinceLastPiece += deltaTime;

        if (game.timeSinceLastPiece >= 10.0f)
        {
            game.generate_Next_Shape();
            game.timeSinceLastPiece = 0.0f;
        }

        window.clear();
        game.draw(window);
        window.display();
    }

    return 0;
}
