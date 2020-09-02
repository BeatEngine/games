
#include "SDLLightGUI.h"

#include "chessRules.h"

LGUI::Sprite* pawnsWhite[8];
LGUI::Sprite* figuresWhite[8];
LGUI::Sprite* pawnsBlack[8];
LGUI::Sprite* figuresBlack[8];

bool computerActive = false;
bool computerSideBlack = true;



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


        if(pos.x + 20 < 900 && pos.y + 20 < 900 && isValidMove(sprite->getId(), w, h, lastPositions[sprite->getId()].x/100, lastPositions[sprite->getId()].y/100, window, false))
        {
            if(isValidMove(sprite->getId(), w, h, lastPositions[sprite->getId()].x/100, lastPositions[sprite->getId()].y/100, window, true))
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




void computerPlayerTasks(LGUI::Window* window)
{
    if(computerActive && (computerSideBlack && !whiteMoves || !computerSideBlack && whiteMoves))
    {
        std::vector<ChessMove> moves;
        if(computerSideBlack)
        {
            std::vector<SDL_Point> validPositions;
            for(int i = 16; i < 32; i++)
            {
                validPositions = validMoves(i, lastPositions[i].x/100, lastPositions[i].y/100, window);
                for(int p = 0; p < validPositions.size(); p++)
                {
                    ChessMove m;
                    m.id = i;
                    m.x  = validPositions[i].x;
                    m.y  = validPositions[i].y;
                    m.value = valueOfMove(i, validPositions[i].x, validPositions[i].y, lastPositions[i].x/100, lastPositions[i].y/100, window);
                    moves.push_back(m);
                }
            }
        }
        else
        {
            std::vector<SDL_Point> validPositions;
            for(int i = 0; i < 10; i++)
            {
                validPositions = validMoves(i, lastPositions[i].x/100, lastPositions[i].y/100, window);
                for(int p = 0; p < validPositions.size(); p++)
                {
                    ChessMove m;
                    m.id = i;
                    m.x  = validPositions[i].x;
                    m.y  = validPositions[i].y;
                    m.value = valueOfMove(i, validPositions[i].x, validPositions[i].y, lastPositions[i].x/100, lastPositions[i].y/100, window);
                    moves.push_back(m);
                }
            }
        }
        if(moves.size()>0)
        {
            for(int i = 0; i < moves.size(); i++)
            {
                if(moves[i].value <= -100000)
                {
                    moves.erase(moves.begin()+i);
                    i--;
                }
            }
            if(moves.size()>0)
            {
                std::sort(moves.begin(), moves.end(), cmpcmovs);
                printf("%s", nameOfFigure(moves[0].id));
                printf(" moves from %c%c to %c%c\n", 'a'+ moves[0].x/100, '1'+7-moves[0].y/100, 'a'+ lastPositions[moves[0].id].x/100,'1'+ 7-lastPositions[moves[0].id].y/100);
                bool error = !isValidMove(moves[0].id, moves[0].x, moves[0].y, lastPositions[moves[0].id].x/100, lastPositions[moves[0].id].y/100, window, true);
                if(error)
                {
                    printf("Invalid move!\n");
                    int dbg = 0;
                    dbg = 1;
                }
            }
        }
    }
}

int main(int args, char** arg)
{
    std::string title = "Light GUI - Chess (by BeatEngine®)";
    LGUI::Window window(title, 1000, 1000, true);


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


