#include "game.h"
#include <fstream>
#include <iostream>
#include <algorithm>

Game::Game():
    tile1(), tile2(), tile3(),
    mWindow(new sf::RenderWindow(sf::VideoMode(winX, winY), "Tetris")),
    textureToTiles()
{
    srand(time(NULL));
    tB1.loadFromFile("images/start.jpg");
    tB2.loadFromFile("images/tryAgain.jpg");
    tB3.loadFromFile("images/scoreboard.jpg");
    tB4.loadFromFile("images/menu.jpg");
    tn.loadFromFile("images/next.jpg");
    sB1.setTexture(tB1);
    sB2.setTexture(tB2);
    sB3.setTexture(tB3);
    sB4.setTexture(tB4);
    sn.setTexture(tn);
    sB1.setOrigin(230,143);
    sB2.setOrigin(230,143);
    sB3.setOrigin(230,143);
    sB4.setOrigin(230,143);
    sn.setOrigin(143,230);
    sB1.setScale(0.5,0.5);
    sB2.setScale(0.5,0.5);
    sB3.setScale(0.5,0.5);
    sB4.setScale(0.3,0.3);
    sn.setScale(0.4,0.4);
    sB1.setPosition(300,400);
    sB2.setPosition(300,450);
    sB3.setPosition(300,600);
    sB4.setPosition(500,700);
    sn.setPosition(500,120);

    //texture to tiles
    delete textureToTiles;
    textureToTiles = new sf::Texture;
    textureToTiles->loadFromFile("images/tiles.png");

    //tiles
    delete tile1;
    tile1 = new Tile(textureToTiles);
    delete tile2;
    tile2 = new Tile(textureToTiles);
    delete tile3;
    tile3 = new Tile(textureToTiles);
    tile3->setFramePos();

    //loading files
    tBackground.loadFromFile("images/Background.jpg");
    tFrame.loadFromFile("images/frame2.jpg");

    sBackground.setTexture(tBackground);
    sBackground.setTextureRect(sf::IntRect(600,200,600,800));
    sFrame.setTexture(tFrame);
    sFrame.setScale(0.4,0.464);
    sFrame.setPosition(20,20);

    //Text
    font.loadFromFile("Fonts/Amatic-Bold.ttf");
    text1.setFont(font);
    text1.setCharacterSize(200);
    text1.setFillColor(sf::Color::Black);
    text1.setPosition(150,80);
    text1.setString("Tetris");

    text2.setFont(font);
    text2.setCharacterSize(60);
    text2.setFillColor(sf::Color::Black);
    text2.setPosition(40,680);
    text2.setString("Score:");

    text3.setFont(font);
    text3.setCharacterSize(60);
    text3.setFillColor(sf::Color::Black);
    text3.setPosition(140,680);
    text3.setString(std::to_string(score));

    text4.setFont(font);
    text4.setCharacterSize(60);
    text4.setFillColor(sf::Color::Black);
    text4.setPosition(210,200);
    text4.setString("Your score:");

    text5.setFont(font);
    text5.setCharacterSize(60);
    text5.setFillColor(sf::Color::Black);
    text5.setPosition(210,280);
    text5.setString(std::to_string(score));

    text6.setFont(font);
    text6.setCharacterSize(60);
    text6.setFillColor(sf::Color::Black);
    text6.setPosition(170,40);
    text6.setString("Top 10 scores:");

    text7.setFont(font);
    text7.setCharacterSize(50);
    text7.setFillColor(sf::Color::Black);
    text7.setPosition(445,200);
    text7.setString("Next tile");

    text8.setFont(font);
    text8.setCharacterSize(40);
    text8.setFillColor(sf::Color::Black);
    text8.setPosition(445,180);

    //scoreboard
    std::ifstream myfile("scores.txt");
    for(int i=0;i<10;i++){
        int k;
        myfile>>k;
        bestScores.push_back(k);
    }
    bestScores.push_back(0);
    myfile.close();

}

void Game::run()
{
    sf::Time mTimer = sf::Time::Zero;
    sf::Clock mClock;
    while(mWindow->isOpen())
    {
        mTimer += mClock.restart();
        if(mTimer >timePerFrame) {
            mTimer -= timePerFrame;
            update();
            procesEvents();
            render();
        }
    }
}

void Game::procesEvents(){
    sf::Event event;
    if(mWindow->pollEvent(event)){
        if(event.type == sf::Event::Closed) {
            saveScoreBoard();
            mWindow->close();
        }
        if(event.type == sf::Event::MouseButtonPressed){
            if(event.mouseButton.button == sf::Mouse::Left){
                sf::Vector2i mPos = sf::Mouse::getPosition(*mWindow);
                sf::FloatRect bRec;
                switch(gameStatus){
                case 1:
                    bRec = sB1.getGlobalBounds();
                    if(bRec.contains(mPos.x, mPos.y)) {
                        gameStatus = 2;
                        tile1->setBasePos();
                    }
                    bRec = sB3.getGlobalBounds();
                    if(bRec.contains(mPos.x, mPos.y)) gameStatus = 4;
                    break;
                case 2:
                    bRec = sB4.getGlobalBounds();
                    if(bRec.contains(mPos.x, mPos.y)) gameStatus = 1;
                    break;
                case 3:
                    bRec = sB2.getGlobalBounds();
                    if(bRec.contains(mPos.x, mPos.y)) {
                        gameStatus = 2;
                        tile1->setBasePos();
                    }
                    bRec = sB4.getGlobalBounds();
                    if(bRec.contains(mPos.x, mPos.y)) gameStatus = 1;
                    break;
                case 4:
                    bRec = sB4.getGlobalBounds();
                    if(bRec.contains(mPos.x, mPos.y)) gameStatus = 1;
                    break;
                }
            }
        }
        if(event.type == sf::Event::KeyPressed && gameStatus == 2){
            if(event.key.code == sf::Keyboard::Up) moveTile();
            if(event.key.code == sf::Keyboard::Left) moveTile(-1,0);
            if(event.key.code == sf::Keyboard::Right) moveTile(1,0);
            if(event.key.code == sf::Keyboard::Down) downKey = true;
        }
        if(event.type == sf::Event::KeyReleased){
            if(event.key.code == sf::Keyboard::Down) downKey = false;
        }
    }
}

void Game::update(){
    static int downSpeed= false;
    if(gameStatus == 2){
        if(downKey){
            if(downSpeed) {
                downSpeed++;
                if(downSpeed == 40) downSpeed = 0;
            } else moveTile(0,1);
        }
        tick();
        checkLine();
    }
}

void Game::render(){
    mWindow->clear(sf::Color::White);
    mWindow->draw(sBackground);

    switch(gameStatus){
    case 1:
        mWindow->draw(text1);
        mWindow->draw(sB1);
        mWindow->draw(sB3);
        break;
    case 2:
        mWindow->draw(sFrame);
        mWindow->draw(text2);
        mWindow->draw(text3);
        drawField();
        mWindow->draw(sB4);
        mWindow->draw(sn);
        mWindow->draw(text7);
        draw(tile1);
        draw(tile3);
        break;
    case 3:
        mWindow->draw(text4);
        mWindow->draw(text5);
        mWindow->draw(sB2);
        mWindow->draw(sB4);
        break;
    case 4:
        mWindow->draw(sB4);
        mWindow->draw(text6);
        drawScoreBoard();
        break;
    }

    mWindow->display();
}

void Game::draw(Tile *p){
    for(auto &w: p->position){
        p->setPosition(w.first*27 + offSetX, w.second*27+ offSetY);
        mWindow->draw(*p);
    }
}

void Game::drawField(){
    for(int i=1;i<8;i++){
        tile2->change(i);
        for(int x=0;x<fieldX;x++){
            for(int y=0;y<fieldY;y++){
                if(field[x][y]  == i){
                    tile2->setPosition(27*x+offSetX, 27*y+offSetY);
                    mWindow->draw(*tile2);
                }
            }
        }
    }
}

bool Game::moveTile(int x, int y){
    std::pair<int, int> position2[4];
    for(int i=0;i<4;i++){
        position2[i].first = tile1->position[i].first + x;
        position2[i].second = tile1->position[i].second + y;
        if(position2[i].first < 0 || position2[i].first >= fieldX || position2[i].second >= fieldY) return false;
        if(field[position2[i].first][position2[i].second] != 0) return false;
    }
    tile1->move(x,y);
    return true;
}

bool Game::moveTile(){
    std::pair<int, int> position2[4];
    std::pair<int,int> p = tile1->position[1];
    for(int i=0;i<4;i++){
        int x = tile1->position[i].second - p.second;
        int y = tile1->position[i].first - p.first;
        position2[i].first = p.first - x;
        position2[i].second = p.second + y;
        if(position2[i].first < 0 || position2[i].first >= fieldX || position2[i].second > fieldY) return false;
        if(field[position2[i].first][position2[i].second] != 0) return false;
    }
    for(int i=0;i<4;i++){
        tile1->position[i].first = position2[i].first;
        tile1->position[i].second = position2[i].second;
    }
    return  true;
}

void Game::tick(){
    tickTimer += timePerFrame;
    if(tickTimer >= timeToTick){
        tickTimer -= timeToTick;
        if(!moveTile(0,1)){
            for(auto &w: tile1->position){
                field[w.first][w.second] = tile1->color;
            }
            delete tile1;
            tile1 = tile3;
            tile1->setBasePos();
            tile3 = new Tile(textureToTiles);
            tile3->setFramePos();

            if(!moveTile(0,0)){
                gameStatus = 3;
                text5.setString(std::to_string(score));
                checkScoreBoard(score);
                newGame();
            }
        }
    }
}

void Game::checkLine(){
    int k=fieldY-1;
    int nOfRows = 0;
    for (int i=fieldY-1;i>0;i--)
    {
        int count=0;
        for (int j=0;j<fieldX;j++)
        {
            if (field[j][i]) count++;
            field[j][k]=field[j][i];
        }
        if (count<fieldX) {k--;}
        else {nOfRows++;}
    }
    switch (nOfRows) {
    case 1:
        score += 10;
        break;
    case 2:
        score += 25;
        break;
    case 3:
        score += 45;
        break;
    case 4:
        score += 80;
        break;
    }
    text3.setString(std::to_string(score));
}

void Game::newGame(){
    for(int i=0;i<fieldX;i++){
        for(int j=0;j<fieldY;j++){
            field[i][j] = 0;
        }
    }
    score = 0;
    text3.setString(std::to_string(score));
}

void Game::drawScoreBoard(){
    for(int i=0;i<10;){
        text8.setPosition(50 ,120 + i++ *50);
        text8.setString(std::to_string(i));
        mWindow->draw(text8);
    }

    for(int i=0;i<10;){
        text8.setPosition(150 ,120 + i *50);
        text8.setString(std::to_string(bestScores[i++]));
        mWindow->draw(text8);
    }
}

void Game::saveScoreBoard(){
    std::ofstream myFile("scores.txt");
    for(int i=0;i<10;i++){
        myFile<<bestScores[i]<<std::endl;
    }
    myFile.close();
}

void Game::checkScoreBoard(int score){
    bestScores[10] = score;
    std::sort(bestScores.begin(),bestScores.end(), [](int a, int b){return a>b;});

}
