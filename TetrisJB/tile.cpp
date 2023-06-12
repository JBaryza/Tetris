#include "tile.h"
#include <iostream>

Tile::Tile(const sf::Texture *textureWsk)
{
    schemNumber = rand()%7;
    color = rand()%7 +1;
    setTexture(*textureWsk);
    setTextureRect(sf::IntRect(color*18,0,18,18));
    setScale(1.5,1.5);
    for(int i=0;i<4;i++){
       position[i].first = tileSchem[schemNumber][i] % 2 +5;
       position[i].second = tileSchem[schemNumber][i] / 2;
    }
}

void Tile::move(int x, int y){
    for(auto &w: position){
        w.first += x;
        w.second += y;
    }
}

void Tile::change(int col){
    setTextureRect(sf::IntRect(col*18,0,18,18));
}

void Tile::setBasePos(){
    for(int i=0;i<4;i++){
       position[i].first = tileSchem[schemNumber][i] % 2 +5;
       position[i].second = tileSchem[schemNumber][i] / 2;
    }
}

void Tile::setFramePos(){
    for(int i=0;i<4;i++){
       position[i].first = tileSchem[schemNumber][i] % 2 +15;
       position[i].second = tileSchem[schemNumber][i] / 2;
    }
}
