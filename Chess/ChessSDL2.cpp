
#include "SDLLightGUI.h"

LGUI::Sprite* pawnsWhite[8];
LGUI::Sprite* figuresWhite[8];
LGUI::Sprite* pawnsBlack[8];
LGUI::Sprite* figuresBlack[8];

void moveFigure(void** params)
{
    LGUI::Sprite* sprite = (LGUI::Sprite*)params[1];
    LGUI::Window* window = (LGUI::Window*)params[0];
    SDL_Event* motionEvent = (SDL_Event*)params[2];
    if(sprite->isLeftButtonDown() && sprite->getLastMouseX() >= 0 && sprite->getLastMouseY() >= 0)
    {
        int mx = motionEvent->motion.x - sprite->getLastMouseX();
        int my = motionEvent->motion.y - sprite->getLastMouseY();

        SDL_Rect pos = sprite->getPosition();
        pos.x += mx;
        pos.y += my;
        sprite->setPosition(pos.x, pos.y, window);
        //sprite->setLastCursorPosition(pos.x, pos.y);
    }

}

void resetGame(void** params)
{
    LGUI::Button* button = (LGUI::Button*)params[1];
    LGUI::Window* window = (LGUI::Window*)params[0];
    for(int i = 0; i < 8; i++)
    {
        pawnsWhite[i]->setPosition(107 + i*100,700,false);
        pawnsWhite[i]->setBorder(LGUI::RGBA(0,0,0,255), 0);
        figuresWhite[i]->setPosition(100 + i*100,800,false);
        figuresWhite[i]->setBorder(LGUI::RGBA(0,0,0,255), 0);
        pawnsBlack[i]->setPosition(107 + i*100,200,false);
        pawnsBlack[i]->setBorder(LGUI::RGBA(0,0,0,255), 0);
        figuresBlack[i]->setPosition(100 + i*100,100,false);
        figuresBlack[i]->setBorder(LGUI::RGBA(0,0,0,255), 0);
    }
}

int main(int args, char** arg)
{
    std::string title = "Light GUI - Chess (by BeatEngine®)";
    LGUI::Window window(title, 1000, 1000);

    std::string abso = "./";
    abso = "/home/david/Dokumente/SDL-Projects/Chess/";
    
    LGUI::Sprite* background = new LGUI::Sprite(100,100,800,800,LGUI::RGBA(0,0,0,0), &window, abso + "res/background.png");
    background->setLayer(0);
    
    figuresWhite[0] = new LGUI::Sprite(100, 100,50,50,LGUI::RGBA(0,0,0,255),&window, abso + "res/white/rook.png");
    figuresWhite[1] = new LGUI::Sprite(100, 100,50,50,LGUI::RGBA(0,0,0,255),&window, abso + "res/white/knight.png");
    figuresWhite[2] = new LGUI::Sprite(100, 100,50,50,LGUI::RGBA(0,0,0,255),&window, abso + "res/white/bishop.png");
    figuresWhite[3] = new LGUI::Sprite(100, 100,50,50,LGUI::RGBA(0,0,0,255),&window, abso + "res/white/queen.png");
    figuresWhite[4] = new LGUI::Sprite(100, 100,50,50,LGUI::RGBA(0,0,0,255),&window, abso + "res/white/king.png");
    figuresWhite[5] = new LGUI::Sprite(100, 100,50,50,LGUI::RGBA(0,0,0,255),&window, abso + "res/white/bishop.png");
    figuresWhite[6] = new LGUI::Sprite(100, 100,50,50,LGUI::RGBA(0,0,0,255),&window, abso + "res/white/knight.png");
    figuresWhite[7] = new LGUI::Sprite(100, 100,50,50,LGUI::RGBA(0,0,0,255),&window, abso + "res/white/rook.png");
    
    figuresBlack[0] = new LGUI::Sprite(100, 100,50,50,LGUI::RGBA(0,0,0,255),&window, abso + "res/black/rook.png");
    figuresBlack[1] = new LGUI::Sprite(100, 100,50,50,LGUI::RGBA(0,0,0,255),&window, abso + "res/black/knight.png");
    figuresBlack[2] = new LGUI::Sprite(100, 100,50,50,LGUI::RGBA(0,0,0,255),&window, abso + "res/black/bishop.png");
    figuresBlack[4] = new LGUI::Sprite(100, 100,50,50,LGUI::RGBA(0,0,0,255),&window, abso + "res/black/queen.png");
    figuresBlack[3] = new LGUI::Sprite(100, 100,50,50,LGUI::RGBA(0,0,0,255),&window, abso + "res/black/king.png");
    figuresBlack[5] = new LGUI::Sprite(100, 100,50,50,LGUI::RGBA(0,0,0,255),&window, abso + "res/black/bishop.png");
    figuresBlack[6] = new LGUI::Sprite(100, 100,50,50,LGUI::RGBA(0,0,0,255),&window, abso + "res/black/knight.png");
    figuresBlack[7] = new LGUI::Sprite(100, 100,50,50,LGUI::RGBA(0,0,0,255),&window, abso + "res/black/rook.png");
    

    LGUI::ScrollBox* box = new LGUI::ScrollBox(100,100,800,800,LGUI::RGBA(0, 0, 0, 0), LGUI::RGBA(0, 0, 0, 255), &window);

    box->setLayer(1);

    LGUI::Button* resetButton = new LGUI::Button(400, 50, 200, 30, "Reset", LGUI::RGBA(220, 220, 220, 255), LGUI::RGBA(70, 70, 70, 255), &window, 20);

    resetButton->setOnLeftClick(resetGame);

    window.addComponent(background);
    window.addComponent(box);
    window.addComponent(resetButton);
    for(int i = 0; i < 8; i++)
    {
        pawnsWhite[i] = new LGUI::Sprite(100, 100,50,50,LGUI::RGBA(0,0,0,255),&window, "/home/david/Dokumente/SDL-Projects/Chess/res/white/pawn.png");
        pawnsWhite[i]->setPosition(107 + i*100,700,false);
        pawnsWhite[i]->setBorder(LGUI::RGBA(0,0,0,255), 0);
        figuresWhite[i]->setPosition(100 + i*100,800,false);
        figuresWhite[i]->setBorder(LGUI::RGBA(0,0,0,255), 0);
        pawnsBlack[i] = new LGUI::Sprite(100, 100,50,50,LGUI::RGBA(0,0,0,255),&window, "/home/david/Dokumente/SDL-Projects/Chess/res/black/pawn.png");
        pawnsBlack[i]->setPosition(107 + i*100,200,false);
        pawnsBlack[i]->setBorder(LGUI::RGBA(0,0,0,255), 0);
        figuresBlack[i]->setPosition(100 + i*100,100,false);
        figuresBlack[i]->setBorder(LGUI::RGBA(0,0,0,255), 0);
        pawnsWhite[i]->setOnMouseMove(moveFigure);
        pawnsBlack[i]->setOnMouseMove(moveFigure);
        figuresWhite[i]->setOnMouseMove(moveFigure);
        figuresBlack[i]->setOnMouseMove(moveFigure);
        window.addComponent(pawnsWhite[i]);
        window.addComponent(figuresWhite[i]);
        window.addComponent(pawnsBlack[i]);
        window.addComponent(figuresBlack[i]);
    }

    while (window.update()) //window main loop
    {
        
        window.updateScreen();
    }

    return 0;
}


