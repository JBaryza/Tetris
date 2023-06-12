#ifndef TILE_H
#define TILE_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Tile:public sf::Sprite
{
    friend class Game;
    Tile();
    Tile(const sf::Texture *textureWsk);
    void move(int x, int y);
    void change(int col);
    void setBasePos();
    void setFramePos();

    int schemNumber;//from 0 to 6
    int color;//from 1 to 7
    std::pair<int,int> position[4];


    constexpr const static int tileSchem[7][4] =
    {
        {1,3,5,7},
        {2,4,5,7},
        {3,5,4,6},
        {3,5,4,7},
        {2,3,5,7},
        {3,5,7,6},
        {2,3,4,5}
    };
};

#endif // TILE_H
