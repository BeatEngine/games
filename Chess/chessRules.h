#include <math.h>

#ifndef SDL_h_
    struct SDL_Point
    {
        int x;
        int y;
    };
#endif

SDL_Point lastPositions[32] = {0};

bool alive[32] = {1};

int movements[32] = {0};

bool whiteMoves = true;

static const char* nameOfFigure(int id)
{
    if(id < 8)
    {
        if(id == 0 || id == 7)
        {
            return (const char*)"White rook";
        }
        else if(id == 1 || id == 6)
        {
            return (const char*)"White knight";
        }
        else if(id == 2 || id == 5)
        {
            return (const char*)"White bishop";
        }
        else if(id == 3)
        {
            return (const char*)"White Queen";
        }
        else
        {
            return (const char*)"White King";
        }
    }else if(id < 16)
    {
        return (const char*)"White pawn";
    }
    else if(id < 24)
    {
        return (const char*)"Black pawn";
    }
    else
    {
        if(id == 24 || id == 31)
        {
            return (const char*)"Black rook";
        }
        else if(id == 25 || id == 30)
        {
            return (const char*)"Black knight";
        }
        else if(id == 26 || id == 29)
        {
            return (const char*)"Black bishop";
        }
        else if(id == 28)
        {
            return (const char*)"Black Queen";
        }
        else
        {
            return (const char*)"Black King";
        }
    }
}

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

float valueOfFigure(int id)
{
    if(id < 8)
    {
        if(id == 0 || id == 7) //      rook
        {
            return 460;
        }
        else if(id == 1 || id == 6) // knight
        {
            return 310;
        }
        else if(id == 2 || id == 5) // bishop
        {
            return 320;
        }
        else if(id == 3) //            Queen
        {
            return 900;
        }
        else //                        King
        {
            return 0;
        }
    }else if(id < 16)
    {
        return 100;
    }
    else if(id < 24)
    {
        if(id == 24 || id == 31) //      rook
        {
            return 460;
        }
        else if(id == 25 || id == 30) // knight
        {
            return 310;
        }
        else if(id == 26 || id == 29) // bishop
        {
            return 320;
        }
        else if(id == 27) //            Queen
        {
            return 900;
        }
        else //                        King
        {
            return 0;
        }
    }
    else
    {
        return 100;
    }
}

inline static int positive(int x)
{
    if(x < 0)
    {
        return -x;
    }
    return x;
}



float valueOfMove(int id, int x, int y, int lastX, int lastY, LGUI::Window* window)
{
    if(id>=8 && id < 16 && y < lastY && whiteMoves) //Pawn (bottom) forward
    {
        int target = figurAt(x, y);
        if(target != -1)
        {
            if(target >= 16 && (x-1 == lastX || x+1 == lastX) && y == lastY - 1) //Enemy front left or right
            {
                return valueOfFigure(target);
            }
        }
        else if(x == lastX && (movements[id] == 0 && y == lastY - 2 || y == lastY - 1))
        {
            return 0;
        }
    }
    else if(id>=16 && id < 24 && y > lastY &&!whiteMoves) //Pawn (top) forward
    {
        int target = figurAt(x, y);
        if(target != -1)
        {
            if(target < 16 && (x-1 == lastX || x+1 == lastX) && y == lastY + 1) //Enemy front left or right
            {
                return valueOfFigure(target);
            }
        }
        else if(x == lastX && (movements[id] == 0 && y == lastY + 2 || y == lastY + 1))
        {
            return 0;
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
                        return valueOfFigure(target);
                    }
                }
                else
                {
                    return 0;
                }
            }
        }
        else if(id == 1 || id == 6)// Knight
        {
            if(positive(x-lastX) == 1 && positive(y-lastY) == 2 || positive(y-lastY) == 1 && positive(x-lastX) == 2 )
            {
                if(target != -1)
                {
                    return valueOfFigure(target);
                }
                else
                {
                    return 0;
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
                        return valueOfFigure(target);
                    }
                }
                else
                {
                    return 0;
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
                        return valueOfFigure(target);
                    }
                }
                else
                {
                    return 0;
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
                        return valueOfFigure(target);
                    }
                }
                else
                {
                    return 0;
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
                        return valueOfFigure(target);
                    }
                }
                else
                {
                    return 0;
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
                        return valueOfFigure(target);
                    }
                }
                else
                {
                    return 0;
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
                        return valueOfFigure(target);
                    }
                }
                else
                {
                    return 0;
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
                        return valueOfFigure(target);
                    }
                }
                else
                {
                    return 0;
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
                        return valueOfFigure(target);
                    }
                }
                else
                {
                    return 0;
                }
            }
        }
    }
    return -100000000;
}

bool isValidMove(int id, int x, int y, int lastX, int lastY, LGUI::Window* window, bool execute = true)
{
    if(x == 0 || y == 0)
    {
        return false;
    }
    if(id>=8 && id < 16 && y < lastY && whiteMoves) //Pawn (bottom) forward
    {
        int target = figurAt(x, y);
        if(target != -1)
        {
            if(target >= 16 && (x-1 == lastX || x+1 == lastX)) //Enemy front left or right
            {
                if(execute)
                {
                    alive[target] = false;
                    lastPositions[target].x = 50*target-800;
                    lastPositions[target].y = 925;
                    window->getComponent(target)->setPosition(50*target-800, 925);
                    movements[id]++;
                    whiteMoves = !whiteMoves;
                }
                return true;
            }
        }
        else if(x == lastX && (movements[id] == 0 && y >= lastY - 2 || y >= lastY - 1))
        {
            if(execute)
            {
                movements[id]++;
                whiteMoves = !whiteMoves;
            }
            return true;
        }
    }
    else if(id>=16 && id < 24 && y > lastY && !whiteMoves) //Pawn (top) forward
    {
        int target = figurAt(x, y);
        if(target != -1)
        {
            if(target < 16 && (x-1 == lastX || x+1 == lastX)) //Enemy front left or right
            {
                if(execute)
                {
                    alive[target] = false;
                    lastPositions[target].x = 50*target-300;
                    lastPositions[target].y = 25;
                    window->getComponent(target)->setPosition(50*target-300, 25);
                    movements[id]++;
                    whiteMoves = !whiteMoves;
                }
                return true;
            }
        }
        else if(x == lastX && (movements[id] == 0 && y <= lastY + 2 || y <= lastY + 1))
        {
            if(execute)
            {
                movements[id]++;
                whiteMoves = !whiteMoves;
            }
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
                        if(execute)
                        {
                            alive[target] = false;
                            lastPositions[target].x = 50*target-800;
                            lastPositions[target].y = 925;
                            window->getComponent(target)->setPosition(50*target-800, 925);
                            movements[id]++;
                            whiteMoves = !whiteMoves;
                        }
                        return true;
                    }
                }
                else
                {
                    if(execute)
                    {
                        movements[id]++;
                        whiteMoves = !whiteMoves;
                    }
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
                        if(execute)
                        {
                            alive[target] = false;
                            lastPositions[target].x = 50*target-800;
                            lastPositions[target].y = 925;
                            window->getComponent(target)->setPosition(50*target-800, 925);
                            movements[id]++;
                            whiteMoves = !whiteMoves;
                        }
                        return true;
                    }
                }
                else
                {
                    if(execute)
                    {
                        movements[id]++;
                        whiteMoves = !whiteMoves;
                    }
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
                        if(execute)
                        {
                            alive[target] = false;
                            lastPositions[target].x = 50*target-800;
                            lastPositions[target].y = 925;
                            window->getComponent(target)->setPosition(50*target-800, 925);
                            movements[id]++;
                            whiteMoves = !whiteMoves;
                        }
                        return true;
                    }
                }
                else
                {
                    if(execute)
                    {
                        movements[id]++;
                        whiteMoves = !whiteMoves;
                    }
                    
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
                        if(execute)
                        {
                            alive[target] = false;
                            lastPositions[target].x = 50*target-800;
                            lastPositions[target].y = 925;
                            window->getComponent(target)->setPosition(50*target-800, 925);
                            movements[id]++;
                            whiteMoves = !whiteMoves;
                        }
                        return true;
                    }
                }
                else
                {
                    if(execute)
                    {
                        movements[id]++;
                        whiteMoves = !whiteMoves;
                    }
                    return true;
                }
            }
        }
        else if(id == 4 && sqrtf((x-lastX)*(x-lastX)+(y-lastY)*(y-lastY)) >= 1)//                       King
        {
            if(sqrtf((x-lastX)*(x-lastX)+(y-lastY)*(y-lastY)) < 2)
            {
                if(target != -1)
                {
                    if(target >= 16)
                    {
                        if(execute)
                        {
                            alive[target] = false;
                            lastPositions[target].x = 50*target-800;
                            lastPositions[target].y = 925;
                            window->getComponent(target)->setPosition(50*target-800, 925);
                            movements[id]++;
                            whiteMoves = !whiteMoves;
                        }
                        return true;
                    }
                }
                else
                {
                    if(execute)
                    {
                        movements[id]++;
                        whiteMoves = !whiteMoves;
                    }
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
                        if(execute)
                        {
                            alive[target] = false;
                            lastPositions[target].x = 50*target-300;
                            lastPositions[target].y = 25;
                            window->getComponent(target)->setPosition(50*target-300, 25);
                            movements[id]++;
                            whiteMoves = !whiteMoves;
                        }
                        return true;
                    }
                }
                else
                {
                    if(execute)
                    {
                        movements[id]++;
                        whiteMoves = !whiteMoves;
                    }
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
                        if(execute)
                        {
                            alive[target] = false;
                            lastPositions[target].x = 50*target-300;
                            lastPositions[target].y = 25;
                            window->getComponent(target)->setPosition(50*target-300, 25);
                            movements[id]++;
                            whiteMoves = !whiteMoves;
                        }
                        return true;
                    }
                }
                else
                {
                    if(execute)
                    {
                        movements[id]++;
                        whiteMoves = !whiteMoves;
                    }
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
                        if(execute)
                        {
                            alive[target] = false;
                            lastPositions[target].x = 50*target-300;
                            lastPositions[target].y = 25;
                            window->getComponent(target)->setPosition(50*target-300, 25);
                            movements[id]++;
                            whiteMoves = !whiteMoves;
                        }
                        return true;
                    }
                }
                else
                {
                    if(execute)
                    {
                        movements[id]++;
                        whiteMoves = !whiteMoves;
                    }
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
                        if(execute)
                        {
                            alive[target] = false;
                            lastPositions[target].x = 50*target-300;
                            lastPositions[target].y = 25;
                            window->getComponent(target)->setPosition(50*target-300, 25);
                            movements[id]++;
                            whiteMoves = !whiteMoves;
                        }
                        return true;
                    }
                }
                else
                {
                    if(execute)
                    {
                        movements[id]++;
                        whiteMoves = !whiteMoves;
                    }
                    return true;
                }
            }
        }
        else if(id == 27 && sqrtf((x-lastX)*(x-lastX)+(y-lastY)*(y-lastY)) >= 1)//                       King
        {
            if(sqrtf((x-lastX)*(x-lastX)+(y-lastY)*(y-lastY)) < 2)
            {
                if(target != -1)
                {
                    if(target < 16)
                    {
                        if(execute)
                        {
                            alive[target] = false;
                            lastPositions[target].x = 50*target-300;
                            lastPositions[target].y = 25;
                            window->getComponent(target)->setPosition(50*target-300, 25);
                            movements[id]++;
                            whiteMoves = !whiteMoves;
                        }
                        return true;
                    }
                }
                else
                {
                    if(execute)
                    {
                        movements[id]++;
                        whiteMoves = !whiteMoves;
                    }
                    return true;
                }
            }
        }
    }
    return false;
} 

std::vector<SDL_Point> validMoves(int id, int posX, int posY, LGUI::Window* window)
{
    std::vector<SDL_Point> positions;
    for(int x = 0; x < 32; x++)
    {
        for(int y = 0; y < 32; y++)
        {
            if(x != posX || y != posY)
            {
                if(isValidMove(id, x, y, posX, posY, window, false))
                {
                    SDL_Point res;
                    res.x = x;
                    res.y = y;
                    positions.push_back(res);
                }
            }
        }
    }
    return positions;
}

