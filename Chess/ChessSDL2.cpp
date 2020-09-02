
#include "SDLLightGUI.h"

LGUI::Sprite* pawnsWhite[8];
LGUI::Sprite* figuresWhite[8];
LGUI::Sprite* pawnsBlack[8];
LGUI::Sprite* figuresBlack[8];

SDL_Point lastPositions[32] = {0};

bool alive[32] = {1};

int movements[32] = {0};

bool whiteMoves = true;

int figurAt(int x, int y)
{
    for(int i = 0; i < 32; i++)
    {
        if(lastPositions[i].x/100 == x && lastPositions[i].y/100 == y)
        {
            return i;
        }
    }
    return -1;
}

inline static int positive(int x)
{
    if(x < 0)
    {
        return -x;
    }
    return x;
}

bool isValidMove(int id, int x, int y, int lastX, int lastY, LGUI::Window* window)
{
    if(id>=8 && id < 16 && y < lastY && whiteMoves) //Pawn (bottom) forward
    {
        int target = figurAt(x, y);
        if(target != -1)
        {
            if(target >= 16 && (x-1 == lastX || x+1 == lastX)) //Enemy front left or right
            {
                alive[target] = false;
                lastPositions[target].x = 50*target-800;
                lastPositions[target].y = 925;
                window->getComponent(target)->setPosition(50*target-800, 925);
                movements[id]++;
                whiteMoves = !whiteMoves;
                return true;
            }
        }
        else if(x == lastX && (movements[id] == 0 && y >= lastY - 2 || y >= lastY - 1))
        {
            movements[id]++;
            whiteMoves = !whiteMoves;
            return true;
        }
    }
    else if(id>=16 && id < 24 && y > lastY &&!whiteMoves) //Pawn (top) forward
    {
        int target = figurAt(x, y);
        if(target != -1)
        {
            if(target < 16 && (x-1 == lastX || x+1 == lastX)) //Enemy front left or right
            {
                alive[target] = false;
                lastPositions[target].x = 50*target-300;
                lastPositions[target].y = 25;
                window->getComponent(target)->setPosition(50*target-300, 25);
                movements[id]++;
                whiteMoves = !whiteMoves;
                return true;
            }
        }
        else if(x == lastX && (movements[id] == 0 && y <= lastY + 2 || y <= lastY + 1))
        {
            movements[id]++;
            whiteMoves = !whiteMoves;
            return true;
        }
    }
    else if(id >= 0 && id < 8 && whiteMoves) // White figures
    {
        int target = figurAt(x, y);
        if(id == 0 || id == 7)//      Rook
        {
            if(x == lastX || y == lastY)
            {
                if(sqrtf((x-lastX)*(x-lastX)+(y-lastY)*(y-lastY)) >= 1)
                {
                    if(x == lastX)
                    {
                        int tmpc = y;
                        if(y < lastY)
                        {
                            tmpc++;
                            while (tmpc != lastY && tmpc != y)
                            {
                                if(figurAt(x, tmpc) != -1)
                                {
                                    return false;
                                }
                                tmpc++;
                            }
                        }
                        else
                        {
                            tmpc--;
                            while (tmpc != lastY && tmpc != y)
                            {
                                if(figurAt(x, tmpc) != -1)
                                {
                                    return false;
                                }
                                tmpc--;
                            }
                        }
                    }
                    else
                    {
                        int tmpc = x;
                        if(x < lastX)
                        {
                            tmpc++;
                            while (tmpc != lastX && tmpc != x)
                            {
                                if(figurAt(tmpc, y) != -1)
                                {
                                    return false;
                                }
                                tmpc++;
                            }
                        }
                        else
                        {
                            tmpc--;
                            while (tmpc != lastX && tmpc != x)
                            {
                                if(figurAt(tmpc, y) != -1)
                                {
                                    return false;
                                }
                                tmpc--;
                            }
                        }
                    }
                }
                if(target != -1)
                {
                    if(target >= 16)
                    {
                        alive[target] = false;
                        lastPositions[target].x = 50*target-800;
                        lastPositions[target].y = 925;
                        window->getComponent(target)->setPosition(50*target-800, 925);
                        movements[id]++;
                        whiteMoves = !whiteMoves;
                        return true;
                    }
                }
                else
                {
                    movements[id]++;
                    whiteMoves = !whiteMoves;
                    return true;
                }
            }
        }
        else if(id == 1 || id == 6)// Knight
        {
            if(positive(x-lastX) == 1 && positive(y-lastY) == 2 || positive(y-lastY) == 1 && positive(x-lastX) == 2 )
            {
                if(target != -1)
                {
                    if(target >= 16)
                    {
                        alive[target] = false;
                        lastPositions[target].x = 50*target-800;
                        lastPositions[target].y = 925;
                        window->getComponent(target)->setPosition(50*target-800, 925);
                        movements[id]++;
                        whiteMoves = !whiteMoves;
                        return true;
                    }
                }
                else
                {
                    movements[id]++;
                    whiteMoves = !whiteMoves;
                    return true;
                }
            }
        }
        else if(id == 2 || id == 5)// Bishop
        {
            if(sqrtf((x-lastX)*(x-lastX)+(y-lastY)*(y-lastY)) > 1 && positive(x-lastX) == positive(y-lastY))
            {
                int tmpc = 1;
                if(x < lastX && y < lastY)
                {
                    while(tmpc < lastX-x)
                    {
                        if(figurAt(x + tmpc, y + tmpc) != -1)
                        {
                            return false;
                        }
                        tmpc++;
                    }
                }
                else if(x < lastX && y > lastY)
                {
                    while(tmpc < lastX-x)
                    {
                        if(figurAt(x + tmpc, y - tmpc) != -1)
                        {
                            return false;
                        }
                        tmpc++;
                    }
                }
                else if(x > lastX && y > lastY)
                {
                    while(tmpc < x-lastX)
                    {
                        if(figurAt(x - tmpc, y - tmpc) != -1)
                        {
                            return false;
                        }
                        tmpc++;
                    }
                }
                else if(x > lastX && y < lastY)
                {
                    while(tmpc < x-lastX)
                    {
                        if(figurAt(x - tmpc, y + tmpc) != -1)
                        {
                            return false;
                        }
                        tmpc++;
                    }
                }
                
                if(target != -1)
                {
                    if(target >= 16)
                    {
                        alive[target] = false;
                        lastPositions[target].x = 50*target-800;
                        lastPositions[target].y = 925;
                        window->getComponent(target)->setPosition(50*target-800, 925);
                        movements[id]++;
                        whiteMoves = !whiteMoves;
                        return true;
                    }
                }
                else
                {
                    movements[id]++;
                    whiteMoves = !whiteMoves;
                    return true;
                }
            }
        }
        else if(id == 3) //           Queen
        {
            if(sqrtf((x-lastX)*(x-lastX)+(y-lastY)*(y-lastY)) >= 1 && (positive(x-lastX) == positive(y-lastY) || x == lastX || y == lastY))
            {

                int tmpcx = 1;
                int tmpcy = 1;
                if(x == lastX)
                {
                    tmpcx = 0;
                }
                if(y == lastY)
                {
                    tmpcy = 0;
                }
                if(x <= lastX && y <= lastY)
                {
                    while(tmpcx < lastX-x || tmpcy < lastY-y)
                    {
                        if(figurAt(x + tmpcx, y + tmpcy) != -1)
                        {
                            return false;
                        }
                        if(tmpcx > 0)
                        {
                            tmpcx++;
                        }
                        if(tmpcy > 0)
                        {
                            tmpcy++;
                        }
                    }
                }
                else if(x <= lastX && y >= lastY)
                {
                    while(tmpcx < lastX-x || tmpcy < y-lastY)
                    {
                        if(figurAt(x + tmpcx, y - tmpcy) != -1)
                        {
                            return false;
                        }
                        if(tmpcx > 0)
                        {
                            tmpcx++;
                        }
                        if(tmpcy > 0)
                        {
                            tmpcy++;
                        }
                    }
                }
                else if(x >= lastX && y >= lastY)
                {
                    while(tmpcx < x-lastX || tmpcy < y-lastY)
                    {
                        if(figurAt(x - tmpcx, y - tmpcy) != -1)
                        {
                            return false;
                        }
                        if(tmpcx > 0)
                        {
                            tmpcx++;
                        }
                        if(tmpcy > 0)
                        {
                            tmpcy++;
                        }
                    }
                }
                else if(x >= lastX && y <= lastY)
                {
                    while(tmpcx < x-lastX || tmpcy < lastY-y)
                    {
                        if(figurAt(x - tmpcx, y + tmpcy) != -1)
                        {
                            return false;
                        }
                        if(tmpcx > 0)
                        {
                            tmpcx++;
                        }
                        if(tmpcy > 0)
                        {
                            tmpcy++;
                        }
                    }
                }

                if(target != -1)
                {
                    if(target >= 16)
                    {
                        alive[target] = false;
                        lastPositions[target].x = 50*target-800;
                        lastPositions[target].y = 925;
                        window->getComponent(target)->setPosition(50*target-800, 925);
                        movements[id]++;
                        whiteMoves = !whiteMoves;
                        return true;
                    }
                }
                else
                {
                    movements[id]++;
                    whiteMoves = !whiteMoves;
                    return true;
                }
            }
        }
        else if(sqrtf((x-lastX)*(x-lastX)+(y-lastY)*(y-lastY)) >= 1)//                       King
        {
            if(sqrtf((x-lastX)*(x-lastX)+(y-lastY)*(y-lastY)) < 2)
            {
                if(target != -1)
                {
                    if(target >= 16)
                    {
                        alive[target] = false;
                        lastPositions[target].x = 50*target-800;
                        lastPositions[target].y = 925;
                        window->getComponent(target)->setPosition(50*target-800, 925);
                        movements[id]++;
                        whiteMoves = !whiteMoves;
                        return true;
                    }
                }
                else
                {
                    movements[id]++;
                    whiteMoves = !whiteMoves;
                    return true;
                }
            }
        }
    }
    else if(id < 32 && !whiteMoves) // Black figures
    {
        int target = figurAt(x, y);
        if(id == 24 || id == 31)//      Rook
        {
            if(x == lastX || y == lastY)
            {
                if(sqrtf((x-lastX)*(x-lastX)+(y-lastY)*(y-lastY)) >= 1)
                {
                    if(x == lastX)
                    {
                        int tmpc = y;
                        if(y < lastY)
                        {
                            tmpc++;
                            while (tmpc != lastY && tmpc != y)
                            {
                                if(figurAt(x, tmpc) != -1)
                                {
                                    return false;
                                }
                                tmpc++;
                            }
                        }
                        else
                        {
                            tmpc--;
                            while (tmpc != lastY && tmpc != y)
                            {
                                if(figurAt(x, tmpc) != -1)
                                {
                                    return false;
                                }
                                tmpc--;
                            }
                        }
                    }
                    else
                    {
                        int tmpc = x;
                        if(x < lastX)
                        {
                            tmpc++;
                            while (tmpc != lastX && tmpc != x)
                            {
                                if(figurAt(tmpc, y) != -1)
                                {
                                    return false;
                                }
                                tmpc++;
                            }
                        }
                        else
                        {
                            tmpc--;
                            while (tmpc != lastX && tmpc != x)
                            {
                                if(figurAt(tmpc, y) != -1)
                                {
                                    return false;
                                }
                                tmpc--;
                            }
                        }
                    }
                }
                if(target != -1)
                {
                    if(target < 16)
                    {
                        alive[target] = false;
                        lastPositions[target].x = 50*target-300;
                        lastPositions[target].y = 25;
                        window->getComponent(target)->setPosition(50*target-300, 25);
                        movements[id]++;
                        whiteMoves = !whiteMoves;
                        return true;
                    }
                }
                else
                {
                    movements[id]++;
                    whiteMoves = !whiteMoves;
                    return true;
                }
            }
        }
        else if(id == 25 || id == 30)// Knight
        {
            if(positive(x-lastX) == 1 && positive(y-lastY) == 2 || positive(y-lastY) == 1 && positive(x-lastX) == 2 )
            {
                if(target != -1)
                {
                    if(target < 16)
                    {
                        alive[target] = false;
                        lastPositions[target].x = 50*target-300;
                        lastPositions[target].y = 25;
                        window->getComponent(target)->setPosition(50*target-300, 25);
                        movements[id]++;
                        whiteMoves = !whiteMoves;
                        return true;
                    }
                }
                else
                {
                    movements[id]++;
                    whiteMoves = !whiteMoves;
                    return true;
                }
            }
        }
        else if(id == 26 || id == 29)// Bishop
        {
            if(sqrtf((x-lastX)*(x-lastX)+(y-lastY)*(y-lastY)) > 1 && positive(x-lastX) == positive(y-lastY))
            {
                int tmpc = 1;
                if(x < lastX && y < lastY)
                {
                    while(tmpc < lastX-x)
                    {
                        if(figurAt(x + tmpc, y + tmpc) != -1)
                        {
                            return false;
                        }
                        tmpc++;
                    }
                }
                else if(x < lastX && y > lastY)
                {
                    while(tmpc < lastX-x)
                    {
                        if(figurAt(x + tmpc, y - tmpc) != -1)
                        {
                            return false;
                        }
                        tmpc++;
                    }
                }
                else if(x > lastX && y > lastY)
                {
                    while(tmpc < x-lastX)
                    {
                        if(figurAt(x - tmpc, y - tmpc) != -1)
                        {
                            return false;
                        }
                        tmpc++;
                    }
                }
                else if(x > lastX && y < lastY)
                {
                    while(tmpc < x-lastX)
                    {
                        if(figurAt(x - tmpc, y + tmpc) != -1)
                        {
                            return false;
                        }
                        tmpc++;
                    }
                }
                if(target != -1)
                {
                    if(target < 16)
                    {
                        alive[target] = false;
                        lastPositions[target].x = 50*target-300;
                        lastPositions[target].y = 25;
                        window->getComponent(target)->setPosition(50*target-300, 25);
                        movements[id]++;
                        whiteMoves = !whiteMoves;
                        return true;
                    }
                }
                else
                {
                    movements[id]++;
                    whiteMoves = !whiteMoves;
                    return true;
                }
            }
        }
        else if(id == 28) //            Queen
        {
            if(sqrtf((x-lastX)*(x-lastX)+(y-lastY)*(y-lastY)) >= 1 && (positive(x-lastX) == positive(y-lastY) || x == lastX || y == lastY))
            {

                int tmpcx = 1;
                int tmpcy = 1;
                if(x == lastX)
                {
                    tmpcx = 0;
                }
                if(y == lastY)
                {
                    tmpcy = 0;
                }
                if(x <= lastX && y <= lastY)
                {
                    while(tmpcx < lastX-x || tmpcy < lastY-y)
                    {
                        if(figurAt(x + tmpcx, y + tmpcy) != -1)
                        {
                            return false;
                        }
                        if(tmpcx > 0)
                        {
                            tmpcx++;
                        }
                        if(tmpcy > 0)
                        {
                            tmpcy++;
                        }
                    }
                }
                else if(x <= lastX && y >= lastY)
                {
                    while(tmpcx < lastX-x || tmpcy < y-lastY)
                    {
                        if(figurAt(x + tmpcx, y - tmpcy) != -1)
                        {
                            return false;
                        }
                        if(tmpcx > 0)
                        {
                            tmpcx++;
                        }
                        if(tmpcy > 0)
                        {
                            tmpcy++;
                        }
                    }
                }
                else if(x >= lastX && y >= lastY)
                {
                    while(tmpcx < x-lastX || tmpcy < y-lastY)
                    {
                        if(figurAt(x - tmpcx, y - tmpcy) != -1)
                        {
                            return false;
                        }
                        if(tmpcx > 0)
                        {
                            tmpcx++;
                        }
                        if(tmpcy > 0)
                        {
                            tmpcy++;
                        }
                    }
                }
                else if(x >= lastX && y <= lastY)
                {
                    while(tmpcx < x-lastX || tmpcy < lastY-y)
                    {
                        if(figurAt(x - tmpcx, y + tmpcy) != -1)
                        {
                            return false;
                        }
                        if(tmpcx > 0)
                        {
                            tmpcx++;
                        }
                        if(tmpcy > 0)
                        {
                            tmpcy++;
                        }
                    }
                }

                if(target != -1)
                {
                    if(target < 16)
                    {
                        alive[target] = false;
                        lastPositions[target].x = 50*target-300;
                        lastPositions[target].y = 25;
                        window->getComponent(target)->setPosition(50*target-300, 25);
                        movements[id]++;
                        whiteMoves = !whiteMoves;
                        return true;
                    }
                }
                else
                {
                    movements[id]++;
                    whiteMoves = !whiteMoves;
                    return true;
                }
            }
        }
        else if(sqrtf((x-lastX)*(x-lastX)+(y-lastY)*(y-lastY)) >= 1)//                       King
        {
            if(sqrtf((x-lastX)*(x-lastX)+(y-lastY)*(y-lastY)) < 2)
            {
                if(target != -1)
                {
                    if(target < 16)
                    {
                        alive[target] = false;
                        lastPositions[target].x = 50*target-300;
                        lastPositions[target].y = 25;
                        window->getComponent(target)->setPosition(50*target-300, 25);
                        movements[id]++;
                        whiteMoves = !whiteMoves;
                        return true;
                    }
                }
                else
                {
                    movements[id]++;
                    whiteMoves = !whiteMoves;
                    return true;
                }
            }
        }
    }
    return false;
}

void figureToGrid(void** params)
{
    LGUI::Sprite* sprite = (LGUI::Sprite*)params[1];
    LGUI::Window* window = (LGUI::Window*)params[0];
    SDL_Event* motionEvent = (SDL_Event*)params[2];
    if(sprite->getLastMouseX() >= 120 && sprite->getLastMouseY() >= 120)
    {
        SDL_Rect pos = sprite->getPosition();
        int w = pos.x/100;
        int h = pos.y/100;


        if(isValidMove(sprite->getId(), w, h, lastPositions[sprite->getId()].x/100, lastPositions[sprite->getId()].y/100, window) && pos.x + 20 < 900 && pos.y + 20 < 900)
        {
            if(sprite->getId() >= 8 && sprite->getId() < 24 )
            {
                sprite->setPosition(7 + w*100, h*100, false, window);
            }
            else
            {
                sprite->setPosition(w*100, h*100, false, window);
            }
        }
        else
        {
            sprite->setPosition(lastPositions[sprite->getId()].x, lastPositions[sprite->getId()].y, true, window);
        }
        lastPositions[sprite->getId()].x = sprite->getPosition().x;
        lastPositions[sprite->getId()].y = sprite->getPosition().y;
        //sprite->setLastCursorPosition(pos.x, pos.y);
    }
}

void moveFigure(void** params)
{
    LGUI::Sprite* sprite = (LGUI::Sprite*)params[1];
    LGUI::Window* window = (LGUI::Window*)params[0];
    SDL_Event* motionEvent = (SDL_Event*)params[2];
    if(sprite->isLeftButtonDown() && sprite->getLastMouseX() > 135 && sprite->getLastMouseY() > 135)
    {
        int mx = motionEvent->motion.x - sprite->getLastMouseX();
        int my = motionEvent->motion.y - sprite->getLastMouseY();
        SDL_Rect pos = sprite->getPosition();
        pos.x += mx;
        pos.y += my;
        if(pos.x + 35 < 900 && pos.y + 35 < 900)
        {
            sprite->setPosition(pos.x, pos.y, window);
        }
        //sprite->setLastCursorPosition(pos.x, pos.y);
    }

}

void resetGame(void** params)
{
    whiteMoves = true;
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
    for(int i = 0; i < 32; i++)
    {
        alive[i] = true;
        movements[i] = 0;
        lastPositions[i].x = window->getComponent(i)->getPosition().x;
        lastPositions[i].y = window->getComponent(i)->getPosition().y;
    }
}

int main(int args, char** arg)
{
    std::string title = "Light GUI - Chess (by BeatEngine®)";
    LGUI::Window window(title, 1000, 1000, true);

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
    background->setId(500);
    box->setId(501);
    resetButton->setId(502);
    window.addComponent(background);
    window.addComponent(box);
    window.addComponent(resetButton);
    for(int i = 0; i < 8; i++)
    {
        pawnsWhite[i] = new LGUI::Sprite(100, 100,50,50,LGUI::RGBA(0,0,0,255),&window, "/home/david/Dokumente/SDL-Projects/Chess/res/white/pawn.png");
        pawnsWhite[i]->setPosition(107 + i*100,700,false);
        pawnsWhite[i]->setBorder(LGUI::RGBA(0,0,0,255), 0);
        pawnsWhite[i]->setId(i+8);
        figuresWhite[i]->setPosition(100 + i*100,800,false);
        figuresWhite[i]->setBorder(LGUI::RGBA(0,0,0,255), 0);
        figuresWhite[i]->setId(i);
        pawnsBlack[i] = new LGUI::Sprite(100, 100,50,50,LGUI::RGBA(0,0,0,255),&window, "/home/david/Dokumente/SDL-Projects/Chess/res/black/pawn.png");
        pawnsBlack[i]->setPosition(107 + i*100,200,false);
        pawnsBlack[i]->setBorder(LGUI::RGBA(0,0,0,255), 0);
        pawnsBlack[i]->setId(i+16);
        figuresBlack[i]->setPosition(100 + i*100,100,false);
        figuresBlack[i]->setBorder(LGUI::RGBA(0,0,0,255), 0);
        figuresBlack[i]->setId(i+24);
        pawnsWhite[i]->setOnMouseMove(moveFigure);
        pawnsBlack[i]->setOnMouseMove(moveFigure);
        figuresWhite[i]->setOnMouseMove(moveFigure);
        figuresBlack[i]->setOnMouseMove(moveFigure);
        pawnsWhite[i]->setOnMouseLeft(figureToGrid);
        pawnsBlack[i]->setOnMouseLeft(figureToGrid);
        figuresWhite[i]->setOnMouseLeft(figureToGrid);
        figuresBlack[i]->setOnMouseLeft(figureToGrid);
        window.addComponent(pawnsWhite[i]);
        window.addComponent(figuresWhite[i]);
        window.addComponent(pawnsBlack[i]);
        window.addComponent(figuresBlack[i]);
    }
    LGUI::Window* wptr = &window;
    resetGame((void**)(&wptr));

    window.setBackgroundColor(LGUI::RGBA(100, 100, 100, 0));

    while (window.update()) //window main loop
    {
        if(whiteMoves)
        {
            window.setBackgroundColor(LGUI::RGBA(160, 160, 160, 0));
        }
        else
        {
            window.setBackgroundColor(LGUI::RGBA(100, 100, 100, 0));
        }
        window.updateScreen();
    }

    return 0;
}


