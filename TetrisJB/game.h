#ifndef GAME_H
#define GAME_H
#include <tile.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

class Game
{
public:
    Game();
    void run();
private:
    //window presets
    void update();
    void procesEvents();
    void render();

    static const int winX = 600;
    static const int winY = 800;
    static const int fieldX = 10;
    static const int fieldY = 20;
    int gameStatus = 1;

    //game section
    void draw(Tile *pointerToTile);//to draw tiles
    void drawField();
    void tick();
    bool moveTile(int x, int y);//move tile
    bool moveTile();
    void checkLine();
    void newGame();
    void drawScoreBoard();
    void saveScoreBoard();
    void checkScoreBoard(int score);
    bool downKey = false;
    std::vector<int> bestScores;
    int field[fieldX][fieldY] = {{0}};
    static const int offSetX = 63;
    static const int offSetY = 70;

    sf::Time tickTimer = sf::Time::Zero;
    sf::Time timeToTick = sf::seconds(3.f/10.f);
    int score = 0;

    Tile *tile1, *tile2, *tile3;
    sf::Time timePerFrame = sf::seconds(1.f/60.f);
    sf::RenderWindow *mWindow;// = new sf::RenderWindow(sf::VideoMode(winX,winY),"Tetris");
    sf::Texture *textureToTiles; //= (new sf::Texture())->loadFromFile("images/tiles.png");
    sf::Texture tBackground, tFrame, tB1, tB2, tB3, tB4, tn;
    sf::Sprite sBackground, sFrame, sB1, sB2, sB3, sB4, sn;
    sf::Font font;
    sf::Text text1, text2, text3, text4, text5, text6, text7, text8;
};

#endif // GAME_H
