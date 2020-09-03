
#include "SDLLightGUI.h"

#include "chessRules.h"

LGUI::Sprite* pawnsWhite[8];
LGUI::Sprite* figuresWhite[8];
LGUI::Sprite* pawnsBlack[8];
LGUI::Sprite* figuresBlack[8];

bool computerActive = false;
bool computerSideBlack = true;

int scaleFactorX = 100;
int scaleFactorY = 100;

int oldScaleX = 100;
int oldScaleY = 100;

int WindowHight = 1000;
int WindowWidth = 1000;

void figureToGrid(void** params)
{
    LGUI::Sprite* sprite = (LGUI::Sprite*)params[1];
    LGUI::Window* window = (LGUI::Window*)params[0];
    SDL_Event* motionEvent = (SDL_Event*)params[2];
    if(sprite->getLastMouseX() >= 120 && sprite->getLastMouseY() >= 120)
    {
        SDL_Rect pos = sprite->getPosition();

        float quadFact = 1.23-0.23*(100/(float)scaleFactorX);

        int w = (pos.x-100)/(quadFact*scaleFactorX)+1;
        int h = (pos.y-100)/(quadFact*scaleFactorY)+1;
        // /*pos.x + scaleFactorX*0.2 < 100+quadFact*scaleFactorX*8 && pos.y + scaleFactorY*0.2 < 100+quadFact*scaleFactorY*8 && */
        if(isValidMove(sprite->getId(), w, h, (int)((lastPositions[sprite->getId()].x-100)/(quadFact*oldScaleX)+1), (int)((lastPositions[sprite->getId()].y-100)/(quadFact*oldScaleY)+1), window, false))
        {
            printf("Move %s (%d|%d)-->(%d|%d) from %c%c to %c%c \n", nameOfFigure(sprite->getId()), (int)((lastPositions[sprite->getId()].x-100)/(quadFact*oldScaleX)+1), (int)((lastPositions[sprite->getId()].y-100)/(quadFact*oldScaleY)+1), w, h, 'a' + (char)((lastPositions[sprite->getId()].x-100)/(quadFact*oldScaleX)+1-1),'0' + 9-(char)((lastPositions[sprite->getId()].y-100)/(quadFact*oldScaleY)+1), 'a' + w-1,'0' + 9-h);
            if(isValidMove(sprite->getId(), w, h, (int)((lastPositions[sprite->getId()].x-100)/(quadFact*oldScaleX)+1), (int)((lastPositions[sprite->getId()].y-100)/(quadFact*oldScaleY)+1), window, true))
            {
                if(sprite->getId() >= 8 && sprite->getId() < 24 )
                {
                    sprite->setPosition(7 + w*quadFact*scaleFactorX, h*quadFact*scaleFactorY, false, window);
                }
                else
                {
                    sprite->setPosition(w*quadFact*scaleFactorX, h*quadFact*scaleFactorY, false, window);
                }
            }
            else
            {
                sprite->setPosition(lastPositions[sprite->getId()].x, lastPositions[sprite->getId()].y, true, window);
            }
        }
        else
        {
            //printf("Invalid Move %s (%d|%d)-->(%d|%d) from %c%c to %c%c \n", nameOfFigure(sprite->getId()), (int)((lastPositions[sprite->getId()].x-100)/(quadFact*oldScaleX)+1), (int)((lastPositions[sprite->getId()].y-100)/(quadFact*oldScaleY)+1), w, h, 'a' + (char)((lastPositions[sprite->getId()].x-100)/(quadFact*oldScaleX)+1-1),'0' + 9-(char)((lastPositions[sprite->getId()].y-100)/(quadFact*oldScaleY)+1), 'a' + w-1,'0' + 9-h);
            sprite->setPosition(lastPositions[sprite->getId()].x, lastPositions[sprite->getId()].y, true, window);
        }
        lastPositions[sprite->getId()].x = sprite->getPosition().x;
        lastPositions[sprite->getId()].y = sprite->getPosition().y;
    }
}

void moveFigure(void** params)
{
    LGUI::Sprite* sprite = (LGUI::Sprite*)params[1];
    LGUI::Window* window = (LGUI::Window*)params[0];
    SDL_Event* motionEvent = (SDL_Event*)params[2];
    if(sprite->isLeftButtonDown() && sprite->getLastMouseX() > scaleFactorX+35 && sprite->getLastMouseY() > scaleFactorY+35)
    {
        int mx = motionEvent->motion.x - sprite->getLastMouseX();
        int my = motionEvent->motion.y - sprite->getLastMouseY();
        SDL_Rect pos = sprite->getPosition();
        pos.x += mx;
        pos.y += my;
        if(pos.x + 35 < scaleFactorX*9 && pos.y + 35 < scaleFactorY*9)
        {
            sprite->setPosition(pos.x, pos.y, true, window);
        }
        //sprite->setLastCursorPosition(pos.x, pos.y);
    }

}

void resetGame(void** params)
{
    LGUI::Window* window = (LGUI::Window*)params[0];
    LGUI::RadioBox* computerChoose = (LGUI::RadioBox*)window->getComponent(531);
    LGUI::RadioBox* playerChoose = (LGUI::RadioBox*)window->getComponent(530);
    if(computerChoose)
    {
        LGUI::RadioBox* boxWhite = (LGUI::RadioBox*)window->getComponent(601);
        LGUI::RadioBox* boxBlack = (LGUI::RadioBox*)window->getComponent(602);
        boxWhite->setEnabled(false);
        boxWhite->setHidden(true);
        boxBlack->setEnabled(false);
        boxBlack->setHidden(true);
        boxWhite->setSelected(false);
        boxBlack->setSelected(true);
        computerActive = false;
        computerChoose->setSelected(false);
    }
    computerActive = false;
    whiteMoves = true;
    if(playerChoose)
    {
        playerChoose->setSelected(true);
    }
    float quadFact = 1.23-0.23*(100/(float)scaleFactorX);
    for(int i = 0; i < 8; i++)
    {
        pawnsWhite[i]->setPosition(100 + quadFact*scaleFactorX*0.07 + i*quadFact*scaleFactorX,100+6*quadFact*scaleFactorY,false);
        pawnsWhite[i]->setSize(quadFact*scaleFactorX/2, quadFact*scaleFactorY/2);
        pawnsWhite[i]->setBorder(LGUI::RGBA(0,0,0,255), 0);
        figuresWhite[i]->setPosition(100 + i*quadFact*scaleFactorX,100+7*quadFact*scaleFactorY,false);
        figuresWhite[i]->setSize(quadFact*scaleFactorX/2, quadFact*scaleFactorY/2);
        figuresWhite[i]->setBorder(LGUI::RGBA(0,0,0,255), 0);
        pawnsBlack[i]->setPosition(100 + quadFact*scaleFactorX*0.07 + i*quadFact*scaleFactorX,100+1*quadFact*scaleFactorY,false);
        pawnsBlack[i]->setSize(quadFact*scaleFactorX/2, quadFact*scaleFactorY/2);
        pawnsBlack[i]->setBorder(LGUI::RGBA(0,0,0,255), 0);
        figuresBlack[i]->setPosition(100 + i*quadFact*scaleFactorX,100,false);
        figuresBlack[i]->setSize(quadFact*scaleFactorX/2, quadFact*scaleFactorY/2);
        figuresBlack[i]->setBorder(LGUI::RGBA(0,0,0,255), 0);
        lastPositions[i].x = figuresWhite[i]->getPosition().x;
        lastPositions[i].y = figuresWhite[i]->getPosition().y;
        lastPositions[i+16].x = pawnsBlack[i]->getPosition().x;
        lastPositions[i+16].y = pawnsBlack[i]->getPosition().y;
        lastPositions[i+8].x = pawnsWhite[i]->getPosition().x;
        lastPositions[i+8].y = pawnsWhite[i]->getPosition().y;
        lastPositions[i+24].x = figuresBlack[i]->getPosition().x;
        lastPositions[i+24].y = figuresBlack[i]->getPosition().y;
    }
    for(int i = 0; i < 32; i++)
    {
        alive[i] = true;
        movements[i] = 0;  
    }
}

void choseComputerWhite(void** params)
{
    computerSideBlack = false;
}

void choseComputerBlack(void** params)
{
    computerSideBlack = true;
}

void choseComputerEnemy(void** params)
{
    LGUI::Window* window = (LGUI::Window*)params[0];
    LGUI::RadioBox* computerChoose = (LGUI::RadioBox*)window->getComponent(531);
    if(computerChoose)
    {
        LGUI::RadioBox* boxWhite = (LGUI::RadioBox*)window->getComponent(601);
        LGUI::RadioBox* boxBlack = (LGUI::RadioBox*)window->getComponent(602);
        if(computerChoose->isSelected() && params[1] == computerChoose)
        {
            boxWhite->setEnabled(true);
            boxWhite->setHidden(false);
            boxBlack->setEnabled(true);
            boxBlack->setHidden(false);
            computerActive = true;
            computerSideBlack = true;
        }
        else
        {
            boxWhite->setEnabled(false);
            boxWhite->setHidden(true);
            boxBlack->setEnabled(false);
            boxBlack->setHidden(true);
            computerActive = false;
            computerSideBlack = true;
        }
    }
}

struct ChessMove
{
    int id;
    int x;
    int y;
    float value;
};

bool cmpcmovs(ChessMove& a, ChessMove& b)
{
    if(a.value == b.value)
    {
        if(a.id < 16)
        {
            return a.id > b.id;
        }
        else
        {
            return a.id < b.id;
        }
    }
    return a.value > b.value;
}

void artificicialEnemy(LGUI::Window* window)
{
    if(computerActive && (computerSideBlack && !whiteMoves || !computerSideBlack && whiteMoves))
    {
        std::vector<ChessMove> moves;
        int a = 0;
        int b = 16;
        if(computerSideBlack)
        {
            a = 16;
            b = 32;
        }
        for(int id = a; id < b; id++)
        {
            for(int y = 0; y < 32; y++)
            {
                for(int x = 0; x < 32; x++)
                {
                    if(isValidMove(id, x, y, lastPositions[id].x / scaleFactorX, lastPositions[id].y / scaleFactorY, window, false))
                    {
                        ChessMove tmp;
                        tmp.id = id;
                        tmp.x = x;
                        tmp.y = y;
                        tmp.value = valueOfMove(id, x, y, lastPositions[id].x / scaleFactorX, lastPositions[id].y / scaleFactorY, window);
                        if(tmp.value > -100000)
                        {
                            int tlx = lastPositions[id].x;
                            int tly = lastPositions[id].y;
                            int tlx2;
                            int tly2;
                            int tid = figurAt(x, y);
                            if(tid != -1)
                            {
                                tlx2 = lastPositions[tid].x;
                                tly2 = lastPositions[tid].y;
                                lastPositions[tid].x = -500;
                                lastPositions[tid].y = -500;
                            }
                            lastPositions[id].x = scaleFactorX*x;
                            lastPositions[id].y = scaleFactorY*y;
                            whiteMoves = !whiteMoves;
                            if(a == 0)
                            {
                                for(int e = 16; e < 32; e++)
                                {
                                    if(isValidMove(e, x, y, lastPositions[e].x/scaleFactorX, lastPositions[e].y/scaleFactorY, window, false))
                                    {
                                        tmp.value -= positive(valueOfFigure(tmp.id));
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                for(int e = 0; e < 16; e++)
                                {
                                    if(isValidMove(e, x, y, lastPositions[e].x/scaleFactorX, lastPositions[e].y/scaleFactorY, window, false))
                                    {
                                        tmp.value -= positive(valueOfFigure(tmp.id));
                                        break;
                                    }
                                }
                            }
                            whiteMoves = !whiteMoves;
                            lastPositions[id].x = tlx;
                            lastPositions[id].y = tly;
                            if(tid != -1)
                            {
                                lastPositions[tid].x = tlx2;
                                lastPositions[tid].y = tly2;
                            }
                            moves.push_back(tmp);
                        }
                    }
                }
            }
        }
        if(moves.size() > 0)
        {
            ChessMove max;
            max.value = -1000000.0f;
            for(int i = 0; i < moves.size(); i++)
            {
                if(moves[i].value > max.value)
                {
                    max = moves[i];
                }
            }
            printf("Move %s(%d) (%d|%d)-->(%d|%d) from %c%c to %c%c \n", nameOfFigure(max.id), max.id, lastPositions[max.id].x/scaleFactorX, lastPositions[max.id].y/scaleFactorY, max.x, max.y, 'a' + lastPositions[max.id].x/scaleFactorX-1,'0' + 9-lastPositions[max.id].y/scaleFactorY, 'a' + max.x-1,'0' + 9-max.y);
            if(isValidMove(max.id, max.x, max.y, lastPositions[max.id].x/scaleFactorX, lastPositions[max.id].y/scaleFactorY,window, true))
            {
                //printf("Error computer tried invalid move!\n");
            }
            LGUI::Sprite* sprite = (LGUI::Sprite*)window->getComponent(max.id);
            if(sprite->getId() >= 8 && sprite->getId() < 24 )
            {
                sprite->setPosition(7 + max.x*scaleFactorX, max.y*scaleFactorY, false, window);
            }
            else
            {
                sprite->setPosition(max.x*scaleFactorX, max.y*scaleFactorY, false, window);
            }
            lastPositions[sprite->getId()].x = sprite->getPosition().x;
            lastPositions[sprite->getId()].y = sprite->getPosition().y;
        }
    }
}


void computerPlayerTasks(LGUI::Window* window)
{
    artificicialEnemy(window);
}

int main(int args, char** arg)
{
    std::string title = "Light GUI - Chess (by BeatEngine®)";
    LGUI::Window window(title, 1000, 1000, true);

    window.setResizable(true);
    LGUI::RadioBox* PVP = new LGUI::RadioBox(100, 50, 6, "Player vs. Player", LGUI::RGBA(200, 200, 200, 255), LGUI::RGBA(0, 0, 0, 255), &window, 12);
    LGUI::RadioBox* PVC = new LGUI::RadioBox(100, 80, 6, "Player vs. Computer", LGUI::RGBA(200, 200, 200, 255), LGUI::RGBA(0, 0, 0, 255), &window, 12);
    PVP->setSelected(true);
    PVP->setId(530);
    PVC->setId(531);
    LGUI::RadioBox* radios[] = {PVP, PVC, 0};
    PVC->setOnLeftClick(choseComputerEnemy);
    PVP->setOnLeftClick(choseComputerEnemy);
    LGUI::RadioGroup* radioGroup = new LGUI::RadioGroup(radios);
    radioGroup->setId(540);
    LGUI::RadioBox* CBS = new LGUI::RadioBox(250, 50, 6, "Player is White", LGUI::RGBA(200, 200, 200, 255), LGUI::RGBA(0, 0, 0, 255), &window, 12);
    LGUI::RadioBox* CWS = new LGUI::RadioBox(250, 80, 6, "Player is Black", LGUI::RGBA(200, 200, 200, 255), LGUI::RGBA(0, 0, 0, 255), &window, 12);
    CBS->setSelected(true);
    CBS->setEnabled(false);
    CBS->setHidden(true);
    CBS->setOnLeftClick(choseComputerBlack);
    CWS->setEnabled(false);
    CWS->setHidden(true);
    CWS->setOnLeftClick(choseComputerWhite);
    CWS->setId(601);
    CBS->setId(602);
    LGUI::RadioBox* radiosSide[] = {CBS, CWS, 0};

    LGUI::RadioGroup* radioGroupSide = new LGUI::RadioGroup(radiosSide);
    radioGroupSide->setId(541);

    std::string abso = "./";
    
    
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
        pawnsWhite[i] = new LGUI::Sprite(100, 100,50,50,LGUI::RGBA(0,0,0,255),&window, abso + "res/white/pawn.png");
        pawnsWhite[i]->setPosition(107 + i*100,700,false);
        pawnsWhite[i]->setBorder(LGUI::RGBA(0,0,0,255), 0);
        pawnsWhite[i]->setId(i+8);
        figuresWhite[i]->setPosition(100 + i*100,800,false);
        figuresWhite[i]->setBorder(LGUI::RGBA(0,0,0,255), 0);
        figuresWhite[i]->setId(i);
        pawnsBlack[i] = new LGUI::Sprite(100, 100,50,50,LGUI::RGBA(0,0,0,255),&window, abso + "res/black/pawn.png");
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

    window.addComponent(radioGroup);
    window.addComponent(radioGroupSide);

    while (window.update()) //window main loop
    {
        int w = window.getRect().w;
        int h = window.getRect().h;

        if(w != WindowWidth || h != WindowHight)
        {
            int W, H;
            float mx, my;
            oldScaleX = scaleFactorX;
            oldScaleY = scaleFactorY;
            if(w<h)
            {
                mx = (float)w/WindowWidth;
                my = (float)w/WindowWidth;
                W = WindowWidth * (float)w/WindowWidth;
                H = WindowHight * (float)w/WindowHight;
                scaleFactorX = (int)(100*(float)w/1000);
                scaleFactorY = (int)(100*(float)w/1000);
            }
            else
            {
                mx = (float)h/WindowWidth;
                my = (float)h/WindowWidth;
                W = WindowWidth * (float)h/WindowWidth;
                H = WindowHight * (float)h/WindowHight;
                scaleFactorX = (int)(100*(float)h/1000);
                scaleFactorY = (int)(100*(float)h/1000);
            }
            box->setSize(W-200, H-200);
            background->setSize(W-200, H-200);
            WindowWidth = window.getRect().w;
            WindowHight = window.getRect().h;
            resetGame((void**)(&wptr));

        }

        computerPlayerTasks(&window);
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


